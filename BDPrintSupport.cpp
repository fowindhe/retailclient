#include "ReceiptPrinter.h"
#include "BDPrintSupport.h"
#include <boost/format.hpp>
#include "Iconv.h"
#include "Dictionary.h"
#include "TicketPrintSupport.h"

using boost::format;
BDPrintSupport::BDPrintSupport ( )
    : errorCode (0)
{
    _rateflag = false;
}

BDPrintSupport::~BDPrintSupport ( )
{}

int BDPrintSupport::PrintGbkFromUtf8Ln(ReceiptPrinter* printer, const char *utf8Str, char *msg, AlignCenterType alignCenter)
{
    if(alignCenter==10)
    {
        int r;
        r = printer->PrintNormal(0, utf8Str, strlen(utf8Str), msg);
        if(r)
            return r;
        r = printer->PrintNormal(0, "\n", strlen("\n"), msg);
        return r;
    }
    Iconv	iconv("UTF-8", "GBK");
    int	r;
    string	gbkStr;
    r = iconv.Convert(utf8Str, gbkStr);
    if (r)
        return r;
    gbkStr.append("\n");

    r = printer->PrintNormal(0, gbkStr.c_str(), gbkStr.size(), msg);

    return r;
}

int BDPrintSupport::PrintGbkFromUtf8Ln(ReceiptPrinter *printer, const char *utf8Str, char *msg)
{
    Iconv	iconv("UTF-8", "GBK");
    int	r;
    string	gbkStr;
    r = iconv.Convert(utf8Str, gbkStr);
    if (r)
        return r;
    gbkStr.append("\n");

    r = printer->PrintNormal(0, gbkStr.c_str(), gbkStr.size(), msg);

    return r;
}

int BDPrintSupport::PrintGbkFromUtf8(ReceiptPrinter *printer, const char *utf8Str, char *msg)
{
    Iconv	iconv("UTF-8", "GBK");
    int	r;
    string gbkStr;
    r = iconv.Convert(utf8Str, gbkStr);
    if (r)
        return r;
    r = printer->PrintNormal(0, gbkStr.c_str(), gbkStr.size(), msg);

    return r;
}

int BDPrintSupport::PrintLine ( ReceiptPrinter* printer, char* msg )
{
    int	r;
    char	scrollCmd[] = {0x1D, 0x5C, 0xEC, 0xFF};
    r = printer->PrintNormal(0, scrollCmd, sizeof(scrollCmd), msg);	// scroll back 20 units
    if (r)
        return r;

    // print ticket code and ticket password
    //   const char	lineSeperator[] = "───────────────────────";
    const char	lineSeperator[] = "------------------------------------------";
    r = PrintGbkFromUtf8Ln(printer, lineSeperator, msg);
    if (r)
        return r;

    r = printer->PrintNormal(0, scrollCmd, sizeof(scrollCmd), msg);	// scroll back 20 units
    if (r)
        return r;

    return Err::SUCCEEDED;
}

QString BDPrintSupport::InsertSpace(string src,int len)
{
    Iconv iconv("UTF-8", "GBK");
    Iconv iconv1("GBK","UTF-8");
    string dstr1="";
    string dstr="";
    int spacenum=0;
    dstr1=src;
    iconv.Convert(dstr1,dstr);
    if(dstr.size()<len)
        spacenum=len-dstr.size();
    dstr.insert(dstr.end(),spacenum,' ');
    iconv1.Convert(dstr,dstr1);
    return QString::fromStdString(dstr1);
}

int BDPrintSupport::newPrint ( ReceiptPrinter* printer, char* msg )
{

    int	r;
    int mypos;
    int changeFlag=0;
    r = printer->NewLineHeight(24, msg);
    if (r)
        return r;

    r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);
    if (r)
        return r;


    QSettings localConfig(QString("../config/print_update.ini"),QSettings::IniFormat);
    localConfig.beginGroup(QString("PRINTER"));
    if(localConfig.value(QString("update"), "").toString() == "1")
    {
        switch(GameID)
        {
        case 41:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["快中彩"]);
            break;
        default:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["单场"]);
            break;
        }
    }
    else
    {
        string	title;
        if(GameID == 41)
            title = "体彩·快中彩";
        else
            title = "体彩·单场竞猜 ";
        Iconv	iconv("UTF-8", "GBK");
        string	gbkStr;
        r = iconv.Convert(title, gbkStr);
        if (r)
            return r;
        //        r = printer->SetFontBold(msg);
        //        if (r)
        //            return r;
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
        r = PrintGbkFromUtf8Ln(printer, title.c_str(), msg);
        if (r)
            return r;
    }
    if (r)
        return r;
    localConfig.endGroup();


    r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg);//C00122 //C00138
    if (r)
        return r;

    r = printer->SetFontNormal(msg);
    if (r)
        return r;
    r=printer->SetFontMagnify(1,1,msg);
    if (r)
        return r;

    QString _gameName = QString::fromStdString(gameName);
    int _spaceNum = 0;
    QString _space ="";
    QString _printbuf = "";
    drawDate = drawDate.substr(0,10);
    //  cout<<"drawDate = "<<drawDate<<endl;
    if(GameID == 41)
    {
        _spaceNum = 42 - QString::fromStdString(drawNo).size() - QString::fromStdString(drawDate).size() -12;
        for(int i = 0; i < _spaceNum; i++)
            _space += " ";
        _printbuf = "第" + QString::fromStdString(drawNo) + "期" + _space + QString::fromStdString(drawDate).left(4) + "年" + QString::fromStdString(drawDate).mid(5,2) + "月"
                + QString::fromStdString(drawDate).right(2) + "日开奖";
        r = PrintGbkFromUtf8Ln(printer,_printbuf.toStdString().c_str(), msg);
        if (r)
            return r;
    }
    else
    {
        _spaceNum = 42 - _gameName.size()*2 - QString::fromStdString(drawNo).size() - 4;
        for(int i = 0; i < _spaceNum; i++)
            _space += " ";
        _printbuf = QString::fromStdString(gameName) + _space + "第" + QString::fromStdString(drawNo) + "期";
        r = PrintGbkFromUtf8Ln(printer,_printbuf.toStdString().c_str(), msg);
        if (r)
            return r;
    }

    r = PrintGbkFromUtf8Ln(printer,str(format("%s  %s") % ticketCode % password).c_str(), msg);
    if (r)
        return r;

    r = PrintLine(printer, msg);
    if (r)
        return r;

    QString space1 = "";
    QString space2 = "";
    if(GameID == 41)
    {
        if(draws > 1)
            _spaceNum = 42 - QString::fromStdString(selectionType).size()*2 - QString::number(draws).size() - QString::number(multiple).size() - QString::number(amount).size() -10;
        else
            _spaceNum = 42 - QString::fromStdString(selectionType).size()*2 - QString::number(multiple).size() - QString::number(amount).size() - 8;
        for(int i = 0; i < _spaceNum/2; i++)
        {
            space1 += " ";
        }
        for(int i = 0; i < _spaceNum - _spaceNum/2; i++)
        {
            space2 += " ";
        }
        if(space1 == "")
            space1 = " ";
        if(space2 == "")
            space2 = " ";
        if(draws > 1)
            _printbuf = QString::fromStdString(selectionType) + space1 + QString::number(draws) + "期" + QString::number(multiple) + "倍" + space2 + "合计" + QString::number(amount) + "元\n";
        else
            _printbuf = QString::fromStdString(selectionType) + space1 + QString::number(multiple) + "倍" + space2 + "合计" + QString::number(amount) + "元\n";
    }
    else
    {
        _chuanname = QString::fromStdString(selectionType).split(" ");
        for(int i = 0; i < _chuanname.size(); i++)
        {
            if(_chuanname[i] == "")
                _chuanname.removeAt(i);
        }
        //        qDebug() <<"_chuannamesize = "<< _chuanname.size();
        //        qDebug()<< _chuanname<<endl;
        if(_chuanname.size() == 1)
        {
            _spaceNum = 42 - QString::fromStdString(selectionType).size()  - QString::number(multiple).size() - QString::number(amount).size() - 18;
            for(int i = 0; i < _spaceNum/2; i++)
            {
                space1 += " ";
            }
            for(int i = 0; i < _spaceNum - _spaceNum/2; i++)
            {
                space2 += " ";
            }
            if(space1 == "")
                space1 = " ";
            if(space2 == "")
                space2 = " ";
            _printbuf = "过关方式 " + QString::fromStdString(selectionType) + space1 + QString::number(multiple) + "倍" + space2 + "合计" + QString::number(amount) + "元\n";
        }
        else
        {
            if(_chuanname.size() <= 5)
            {
                QString _tmpstr = "";
                for(int i = 0; i < _chuanname.size(); i++)
                {
                    if(i == _chuanname.size() - 1)
                        _tmpstr += _chuanname[i];
                    else
                        _tmpstr += _chuanname[i] + "  ";
                }
                QString _tmp = "过关方式 " + _tmpstr;
                _printbuf = InsertSpace(_tmp.toStdString(), 43) + "\n";
            }
            else if(5 < _chuanname.size() &&  _chuanname.size() <= 10)
            {
                QString _tmpstr = "";
                for(int i = 0; i < 5; i++)
                {
                    if(i == 5 - 1)
                        _tmpstr += _chuanname[i];
                    else
                        _tmpstr += _chuanname[i] + "  ";
                }
                QString _tmp = "过关方式 " + _tmpstr;
                _printbuf = InsertSpace(_tmp.toStdString(), 43) + "\n";
                _tmpstr = "";
                for(int i = 5; i < _chuanname.size(); i++)
                {
                    if(i == _chuanname.size() - 1)
                        _tmpstr += _chuanname[i];
                    else
                        _tmpstr += _chuanname[i] + "  ";
                }
                _tmp = "         " + _tmpstr;
                _printbuf += InsertSpace(_tmp.toStdString(), 43) + "\n";
            }
            else if(_chuanname.size() > 10)
            {
                QString _tmpstr = "";
                for(int i = 0; i < 5; i++)
                {
                    if(i == 5 - 1)
                        _tmpstr += _chuanname[i];
                    else
                        _tmpstr += _chuanname[i] + "  ";
                }
                QString _tmp = "过关方式 " + _tmpstr;
                _printbuf = InsertSpace(_tmp.toStdString(), 43) + "\n";
                _tmpstr = "";
                for(int i = 5; i < 10; i++)
                {
                    if(i == 10 - 1)
                        _tmpstr += _chuanname[i];
                    else
                        _tmpstr += _chuanname[i] + "  ";
                }
                _tmp = "         " + _tmpstr;
                _printbuf += InsertSpace(_tmp.toStdString(), 43) + "\n";
                _tmpstr = "";
                for(int i = 10; i < _chuanname.size(); i++)
                {
                    if(i == _chuanname.size() - 1)
                        _tmpstr += _chuanname[i];
                    else
                        _tmpstr += _chuanname[i] + "  ";
                }
                _tmp = "         " + _tmpstr;
                _printbuf += InsertSpace(_tmp.toStdString(), 43) + "\n";
            }
            _spaceNum = 42  - QString::number(multiple).size() - QString::number(amount).size() - 12;
            for(int i = 0; i < _spaceNum; i++)
            {
                space1 += " ";
            }
            QString _tmp1 = space1 + QString::number(multiple) + "倍    " +  "合计" + QString::number(amount) + "元";
            _printbuf += InsertSpace(_tmp1.toStdString(), 43) + "\n";
        }
    }

    r = PrintGbkFromUtf8Ln(printer,_printbuf.toStdString().c_str(), msg);
    if (r)
        return r;

    mypos = 0;
    switch(GameID)
    {
    case 41:
        if(selectType == Invest_Mul)
            mypos = sizeof("金胆: ") ;
        break;
    case 200:
    case 210:
    case 220:
    case 230:
    case 240:
    case 250:
    case 311:
    case 270:
    case 110:
    case 301:
        changeFlag=1;
        mypos=0;
        break;
    default:
        mypos=0;
        break;
    }

    r = printer->NewLineHeight(64, msg);
    if (r)
        return r;
    r = printer->SetAlign(PrintCommand::ALIGN_LEFT, msg);
    if (r)
        return r;
    r = printer->SetLeftMargin(25);
    if (r)
        return r;
    if(GameID == 41)
    {
        r = printer->SetFont(0x01, 0x10, 0x01,msg);
        if (r)
            return r;
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
    }
    else
    {
        r = printer->SetFontNormal(msg);
        if (r)
            return r;
        r = printer->SetFontMagnify(1, 1, msg);
        if (r)
            return r;
    }

    int selectionsLineNo=2;
    selectionsLineNo+= selections.size();
    cout<<selections.size()<<endl;
    if(GameID==41){
        if(selectionsLineNo==3){
            printer->MarkFeed(3,msg);
            selectionsLineNo+=2;
        }
        else if(selectionsLineNo<7){
            printer->MarkFeed(2,msg);
            selectionsLineNo+=1;
        }
    }
    for (list<string>::iterator i = selections.begin(); i != selections.end(); i++)
    {
        r = printer->SetAlign(PrintCommand::ALIGN_LEFT, msg);
        if (r)
            return r;
        if (i->size() == 0)
            continue;
        bool	blank = true;
        for (unsigned pos = 0; pos != i->size(); pos++)
        {
            if ((*i)[pos] != ' ')
            {
                blank = false;
                break;
            }
        }
        if (blank)
            continue;
        if( (GameID == 41) )
        {
            if(selectType == Invest_Mul)
            {
                r = printer->SetFontNormal(msg);
                if (r)
                    return r;
                r = printer->SetFontMagnify(1, 1, msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8(printer, (i->substr(0,mypos-2)).c_str(), msg);
                if (r)
                    return r;
                r = printer->SetFont(0x01, 0x10, 0x01,msg);
                if (r)
                    return r;
                r = printer->SetFontMagnify(2, 1, msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8Ln(printer,  (i->substr(mypos -2,i->size())).c_str(), msg);
                if (r)
                    return r;
            }
            else
            {
                QString buf = QString::fromStdString(*i);
                buf = buf.right(buf.size()- 1);
                r = printer->SetFontMagnify(1, 1, msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8(printer, buf.left(1).toStdString().c_str(), msg);
                if (r)
                    return r;
                r = printer->SetFont(0x01, 0x10, 0x01,msg);
                if (r)
                    return r;
                r = printer->SetFontMagnify(2, 1, msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8(printer, buf.mid(2,21).toStdString().c_str(), msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8Ln(printer, buf.right(6).toStdString().c_str(), msg);
                if (r)
                    return r;

            }
        }
        else
        {
            r = printer->SetFontBold(msg);
            if (r)
                return r;
            r = PrintGbkFromUtf8Ln(printer,  (i->substr(mypos,i->size())).c_str(), msg);
            if (r)
                return r;
        }
    }
    r = printer->SetFont(0x00, 0x00, 0x01,msg);
    if (r)
        return r;
    r = printer->SetFontMagnify(1, 1, msg);
    if (r)
        return r;
    r = printer->NewLineHeight(24, msg);
    if (r)
        return r;
    r = printer->SetFontNormal(msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8Ln(printer, " ", msg);
    if (r)
        return r;
    r = printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
    if (r)
        return r;
    r = printer->SetPrintArea(0);
    if (r)
        return r;
    if(GameID==41){
        if(selectionsLineNo<7)
        {
            printer->MarkFeed(8-selectionsLineNo, msg);
            PrintLine(printer,msg);
            selectionsLineNo=8;
        }
        else{
            if(selectionsLineNo<9){
                PrintLine(printer,msg);
                selectionsLineNo++;
            }
        }
    }
//    r = PrintLine(printer, msg);
    if (r)
        return r;
    int adLineNo=0;
//    if(this->_rateflag)
//    {
//        r = PrintGbkFromUtf8Ln(printer, (_rate + "元").toStdString().c_str(),msg);
//        adLineNo++;
//        if (r)
//        {
//            return r;
//        }
//    }
    if(selectionsLineNo+adLineNo<11)
    {
        TicketPrintSupport::formatLine(shopAddress,1);
        r=PrintGbkFromUtf8Ln(printer, shopAddress.c_str(),msg);
        adLineNo+=1;
    }
    if(selectionsLineNo+adLineNo<11){
        if(this->_rateflag)
        {
            PrintGbkFromUtf8Ln(printer, (_rate + "元").toStdString().c_str(),msg);
        }
    }
    if(selectionsLineNo+adLineNo<11){
        TicketPrintSupport::formatLine(ad,3-adLineNo);
        r=PrintGbkFromUtf8Ln(printer, ad.c_str(),msg);
    }
//    r = PrintGbkFromUtf8Ln(printer, ad.c_str(), msg);
//    if (r)
//        return r;
//    r = PrintGbkFromUtf8Ln(printer, shopAddress.c_str(), msg);//C00087
//    if (r)
//        return r;

    r = PrintGbkFromUtf8Ln(printer, (terminalno.toStdString() + "  " + termSerialNo.toStdString() + "   " + purchaseTime).c_str(), msg);
    if (r)
        return r;
    r = printer->PageModePrint(PrintCommand::PTR_PM_CHECK_ERROR, msg); //C00139
    if(r)
        return r;
    r = PrintGbkFromUtf8Ln(printer, " ", msg);
//    r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg);
     r = printer->PageModePrint(PrintCommand::PTR_PM_PAGE_MODE, msg);
    if (errorCode == 0)
    {
        r = printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
        r = printer->PrintBarCode(0, barCode.c_str(), barCode.size()
                                  , PrintCommand::PTR_BCS_PDF417, 0, 0, 0, 0, msg);
    }
    else
    {
        r = PrintLine(printer, msg);

        r = PrintGbkFromUtf8Ln(printer,
                               str(format("%-32s%s")  % ticketCode % password).c_str()
                               , msg);

        r = PrintGbkFromUtf8Ln(printer,
                               str(format("%d: %s")  % errorCode % errorMsg).c_str()
                               , msg);

        errorCode = 0;
        errorMsg.clear();
    }
    r = printer->CutPaper(99, msg);

    return Err::SUCCEEDED;
}

int BDPrintSupport::Print ( ReceiptPrinter* printer, char* msg )
{
    int	r;
    struct sStyle
    {
        int front;
        int back;
        int fontWidth;
        int fontHeight;
    };
    typedef struct sStyle showStyle;
    int mypos;//C00146
    int changeFlag=0;//C00146
    r = printer->NewLineHeight(24, msg);
    if (r)
        return r;
    //C00122.end

    // enter page mode


    // set alignment
    r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);
    if (r)
        return r;

    const char*	prefixStr;
    if (demo)
        prefixStr = "演示";
    //        else if (preProduction)
    //                prefixStr = "试运行";
    else
        prefixStr = "体彩";
    string	title = str(format("%s[%s]") % prefixStr % gameName);

    Iconv	iconv("UTF-8", "GBK");
    string	gbkStr;
    r = iconv.Convert(title, gbkStr);
    if (r)
        //return Err::Msg(r, msg);
        return r;
    bool	smallTitle = gbkStr.size() > 24;

    if (smallTitle)
    {
        r = printer->SetFontBold(msg);
        if (r)
            return r;
    }
    else
    {
        // set font double size
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
    }
    // print game nameString
    r = PrintGbkFromUtf8Ln(printer, title.c_str(), msg
                           , smallTitle ? NORMAL_WIDTH_ALIGN_CENTER : DOUBLE_WIDTH_ALIGN_CENTER);
    if (r)
        return r;

    if (!smallTitle)
    {
        // set font normal size
        r = printer->SetFontMagnify(1, 1, msg);
        if (r)
            return r;
    }
    r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg);//C00122 //C00138
    if (r)
        return r;
    // print line seperator
    r = PrintLine(printer, msg);
    if (r)
        return r;

    // set font bold
    r = printer->SetFontBold(msg);
    if (r)
        return r;

    r = printer->SetAlign(PrintCommand::ALIGN_LEFT, msg);//C00122
    if (r)//C00122
        return r;//C00122
    // print ticket code and ticket password
    r = PrintGbkFromUtf8Ln(printer,
                           str(format("%s   %s") % ticketCode % password).c_str()
                           , msg);
    if (r)
        return r;
    // set font normal
    r = printer->SetFontNormal(msg);
    if (r)
        return r;
    //C00122.start
    string space;
    int spaceNum;
    spaceNum=18-str(format("%s") %amount).size()-str(format("%s") %draws).size()-str(format("%s") %multiple).size()-drawNo.size();
    if(spaceNum<0)
        spaceNum=0;
    space.append(spaceNum/3,' ');
    drawDate=drawDate.substr(0,10);
    if(gameName == "胜平负" || gameName == "上下盘单双数" || gameName == "总进球数" || gameName == "半全场胜平负" || gameName == "单场比分" || gameName == "下半场比分" || gameName == "胜负过关")
        r = PrintGbkFromUtf8Ln(printer,
                               str(format("销售时间%s") %purchaseTime).c_str()
                               , msg);
    else
        r = PrintGbkFromUtf8Ln(printer,
                               str(format("销售时间%s 开奖日期%s") %purchaseTime %drawDate).c_str()
                               , msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         str(format("第")).c_str()
                         , msg);
    if (r)
        return r;

    r = printer->SetFontMagnify(2, 1, msg);
    if (r)
        return r;
    r = printer->SetFontBold(msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         drawNo.c_str()
                         , msg);
    if(r)
        return r;
    r = printer->SetFontMagnify(1, 1, msg);
    if (r)
        return r;
    r = printer->SetFontNormal(msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         str(format("期")).c_str()
                         , msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         space.c_str()
                         , msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         space.c_str()
                         , msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         str(format("共")).c_str()
                         , msg);
    if (r)
        return r;
    r = printer->SetFontMagnify(2, 1, msg);
    if (r)
        return r;
    r = printer->SetFontBold(msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         str(format("%s") %draws).c_str()
                         , msg);
    if(r)
        return r;

    r = printer->SetFontMagnify(1, 1, msg);
    if (r)
        return r;
    r = printer->SetFontNormal(msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         str(format("期")).c_str()
                         , msg);
    if (r)
        return r;

    r = printer->SetFontMagnify(2, 1, msg);
    if (r)
        return r;
    r = printer->SetFontBold(msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         space.c_str()
                         , msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         str(format("%s") %multiple).c_str()
                         , msg);
    if(r)
        return r;
    r = printer->SetFontMagnify(1, 1, msg);
    if (r)
        return r;
    r = printer->SetFontNormal(msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         str(format("倍")).c_str()
                         , msg);
    if (r)
        return r;
    r = printer->SetFontMagnify(2, 1, msg);
    if (r)
        return r;
    r = printer->SetFontBold(msg);
    if (r)
        return r;
    if(spaceNum%3==2)
        space.append(1,' ');
    if(spaceNum%3==0)
        space.erase(0,1);
    r = PrintGbkFromUtf8(printer,
                         space.c_str()
                         , msg);
    if(r)
        return r;
    r = PrintGbkFromUtf8(printer,
                         str(format("%s") %amount).c_str()
                         , msg);
    if(r)
        return r;
    r = printer->SetFontMagnify(1, 1, msg);
    if (r)
        return r;
    r = printer->SetFontNormal(msg);
    if (r)
        return r;
    r = PrintGbkFromUtf8Ln(printer,
                           str(format("元")).c_str()
                           , msg);
    if (r)
        return r;
    //C00122.end
    /*
#if 1
        // print drawNo, draws and purchase time
        r = printer->PrintGbkFromUtf8Ln(
                str(format("%s期 共%d期 %d倍 %d元%|32t|%s") % drawNo % draws %multiple % amount % purchaseTime).c_str()
                , msg);
        if (r)
                return r;
#else
        // print drawNo, draws and purchase time
        r = printer->PrintGbkFromUtf8Ln(
                str(format("第%s期 共计%d期 %|31t|%s") % drawNo % draws % purchaseTime).c_str()
                , msg);
        if (r)
                return r;

        // print multiple, amount and draw date
        r = printer->PrintGbkFromUtf8Ln(
                str(format("%d倍 %|8t|金额:%d元 %|30t|%s开奖") % multiple % amount % drawDate).c_str()
                , msg);
        if (r)
                return r;
#endif
*///C00122

    // print line seperator
    r = PrintLine(printer, msg);
    if (r)
        return r;

    // set font bold
    r = printer->SetFontBold(msg);
    if (r)
        return r;

    int	lineCnt = 0;
    // print selection type
    r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);//C00122
    if (r)//C00122
        return r;//C00122
    if (selectionType.size() != 0)
    {
        r = PrintGbkFromUtf8Ln(printer, selectionType.c_str(), msg, NORMAL_WIDTH_ALIGN_CENTER);
        if (r)
            return r;
        ++lineCnt;
    }
    r = PrintGbkFromUtf8Ln(printer, "", msg, NORMAL_WIDTH_ALIGN_CENTER);//C00146
    if (r)//C00146
        return r;//C00146
    //C00122.start
    r = printer->SetAlign(PrintCommand::ALIGN_LEFT, msg);
    if (r)
        return r;
    // print selection details
    //C00146.start
    mypos=0;
    switch(GameID)
    {
    case 21:
    case 24:
        if(selectType==Invest_Mul)
            mypos=6;
        break;
    case 41:
        if(selectType==Invest_Mul)
            mypos=sizeof("金胆: ")-3;
        break;
    case 35:
    case 42:
        if(selectType==Invest_Mul)//C00149
            mypos=6;
        break;
    case 61:
    case 62:
    case 63:
    case 65:
    case 66:
        changeFlag=1;
        mypos=0;
        break;
    case 200:
    case 210:
    case 220:
    case 230:
    case 240:
    case 250:
    case 311:
    case 270:
    case 110:
    case 301:
        changeFlag=1;
        mypos=0;
        break;
    default:
        mypos=0;
        break;
    }
    //C00146.end
    if(GameID==35)
    {
        r = printer->NewLineHeight(64, msg);
        if (r)
            return r;
        r = printer->SetFontNormal(msg); //C00146
        if (r) //C00146
            return r; //C00146
        //if(selectionType=="单式票")//C00146
        {
            for (list<string>::iterator i = selections.begin(); i != selections.end(); i++)
            {
                if (i->size() == 0)
                    continue;
                if(selectType==Invest_Sig )
                {
                    r = printer->SetFontMagnify(2, 1, msg);
                    if (r)
                        return r;
                    r = PrintGbkFromUtf8Ln(printer, i->c_str(), msg);
                    if (r)
                        return r;
                }
                else
                {
                    r = printer->SetFontMagnify(1, 1, msg);
                    if (r)
                        return r;
                    //C00163.start
                    if(i->find("拖",0)!=string::npos || i->find("胆",0)!=string::npos)
                    {
                        r = PrintGbkFromUtf8(printer, (i->substr(0,mypos)).c_str(), msg);
                        if (r)
                            return r;
                    }
                    else
                    {
                        r = PrintGbkFromUtf8(printer, (i->substr(0,mypos-3)).c_str(), msg);
                        if (r)
                            return r;
                    }
                    r = printer->SetFontMagnify(2, 1, msg);
                    if (r)
                        return r;
                    if(i->find("拖",0)!=string::npos || i->find("胆",0)!=string::npos)
                    {
                        r = PrintGbkFromUtf8Ln(printer,  (i->substr(mypos,i->size())).c_str(), msg);
                        if (r)
                            return r;
                    }
                    else
                    {
                        r = PrintGbkFromUtf8Ln(printer,  (i->substr(mypos-2,i->size())).c_str(), msg);
                        if (r)
                            return r;
                    }
                    //C00163.end
                }
                //C00146.end
                lineCnt++;
            }
        }
        /*//else
                {
                        string s1="胆:";
                        string s2="拖:";
                        string selection1;
                        string select;
                        string space="        ";
                        int j=0,flag=0;
                        for (list<string>::iterator i = selections.begin(); i != selections.end(); i++)
                        {
        printf("\ncount-----9\n");
                                if (i->size() == 0)
                                        continue;
                                j=i->find(s1,0);
                                if(j>=0)
                                {
                                        selection1=i->substr(j+6);
                                        flag=1;
                                        lineCnt++;
                                }
                                else
                                {
                                        j=i->find(s2,0);
                                        if(j>=0)
                                        {
                                                select=i->substr(j+6,i->size());
                                                string::size_type idx=0;
                                                while((idx=select.find(space,0))!=string::npos)
                                                {
                                                        select.erase(idx,space.size());
                                                }
                                                int count=0;
                                                for (unsigned pos = 0; pos != select.size(); pos++)
                                                {
                                                        if (select[pos] == ' ')
                                                        {
                                                                count++;
                                                                if(count==7)
                                                                {
                                                                        select.insert(pos+1,3,' ');
                                                                        pos+=4;
                                                                        count=0;
                                                                        lineCnt++;
                                                                }
                                                        }
                                                }
                                        }
                                        else
                                        {
                                                select+=i->substr(4,i->size());
                                        }
                                }
                        }
                        r = printer->SetFontNormal(msg);
                        if (r)
                                return r;
        printf("\ncount-----10\n");
                        if(flag)
                        {
                                r = printer->PrintGbkFromUtf8((s1+=" ").c_str(), msg);
                                if (r)
                                        return r;
                                r = printer->SetFontMagnify(2, 1, msg);
                                if (r)
                                        return r;
                                r = printer->PrintGbkFromUtf8Ln(selection1.c_str(), msg);
                                if (r)
                                        return r;
                        }
                        r = printer->SetFontMagnify(1, 1, msg);
                        if (r)
                                return r;
                        r = printer->PrintGbkFromUtf8((s2+=" ").c_str(), msg);
                        if (r)
                                return r;
                        r = printer->SetFontMagnify(2, 1, msg);
                        if (r)
                                return r;
                        r = printer->PrintGbkFromUtf8Ln(select.c_str(), msg);
                        if (r)
                                return r;
                }
                r = printer->SetFontMagnify(1, 1, msg);
                if (r)
                        return r;*/
    }
    else if(GameID==42)
    {
        int one=1;
        r = printer->SetFontNormal(msg);
        if (r)
            return r;
        r = printer->SetFontMagnify(1, 1, msg);
        if (r)
            return r;
        r = printer->NewLineHeight(64, msg);
        if (r)
            return r;
        for (list<string>::iterator i = selections.begin(); i != selections.end(); i++)
        {
            if (i->size() == 0)
                continue;
            bool	blank = true;
            for (unsigned pos = 0; pos != i->size(); pos++)
            {
                if ((*i)[pos] != ' ')
                {
                    blank = false;
                    break;
                }
            }
            if (blank)
                continue;
            if(selectType==Invest_Sig)//C00138 //C00146
            {
                if(one)
                {
                    one=0;
                    r = PrintGbkFromUtf8Ln(printer, "              前区                  后区", msg);
                    if (r)
                        return r;
                }
                r = printer->SetFontMagnify(2, 1, msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8(printer, (i->substr(1,3)).c_str(), msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8Ln(printer, (i->substr(9,19)+i->substr(i->size()-6,i->size())).c_str(), msg,PRIVATE);
                if (r)
                    return r;
                r = printer->SetFontMagnify(1, 1, msg);
                if (r)
                    return r;
            }
            else
            {
                //C00146.start
                r = printer->SetFontMagnify(1, 1, msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8(printer, (i->substr(0,mypos)).c_str(), msg);
                if (r)
                    return r;
                r = printer->SetFontMagnify(2, 1, msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8Ln( printer, (i->substr(mypos,i->size())).c_str(), msg);
                if (r)
                    return r;
                //C00146.end
            }
            ++lineCnt;
        }
        r = printer->NewLineHeight(24, msg);
        if (r)
            return r;
        r = printer->SetFontNormal(msg);
        if (r)
            return r;
        r = printer->SetFontMagnify(1, 1, msg);
        if (r)
            return r;
    }

    else
    {
        //C00146.start
        r = printer->NewLineHeight(64, msg);
        if (r)
            return r;
        r = printer->SetFontNormal(msg);
        if (r)
            return r;
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
        //C00146.end
        for (list<string>::iterator i = selections.begin(); i != selections.end(); i++)
        {
            if (i->size() == 0)
                continue;
            bool	blank = true;
            for (unsigned pos = 0; pos != i->size(); pos++)
            {
                if ((*i)[pos] != ' ')
                {
                    blank = false;
                    break;
                }
            }
            if (blank)
                continue;
            //C00146.start
            if( (GameID==21 || GameID==41 || GameID==24 || GameID==42) && selectType==Invest_Mul)
            {
                r = printer->SetFontMagnify(1, 1, msg);
                if (r)
                    return r;
                r = PrintGbkFromUtf8(printer, (i->substr(0,mypos)).c_str(), msg);
                if (r)
                    return r;
            }
            r = printer->SetFontMagnify(2, 1, msg);
            if (r)
                return r;
            if(changeFlag==1)
            {
                r = printer->SetFontMagnify(1, 1, msg);
                if (r)
                    return r;
            }
            //C00146.end
            r = PrintGbkFromUtf8Ln(printer,  (i->substr(mypos,i->size())).c_str(), msg);
            if (r)
                return r;
            if( (GameID==21 || GameID==24) && selectType==Invest_Mul)
            {
                int r=i->find("拖",0);
                if(r>=0)
                    mypos--;
            }
            ++lineCnt;
        }
        r = printer->SetFontMagnify(1, 1, msg);//C00146
        if (r)//C00146
            return r;//C00146
        lineCnt-=2;
    }
    //C00122.end
    r = printer->NewLineHeight(24, msg);
    if (r)
        return r;
    r = printer->SetFontNormal(msg);
    if (r)
        return r;
    r = printer->SetFontMagnify(1,1,msg);
    if (r)
        return r;
    /*
        for(int i=0;i<8-lineCnt;i++)
        {
                r = printer->PrintGbkFromUtf8Ln(" ", msg);
                if (r)
                        return r;
        }*/ //C00140
    //set font normal
    //C00122.start
    r = PrintGbkFromUtf8Ln(printer, " ", msg);
    if (r)
        return r;
    //C00122.end
    // print line seperator
    r = PrintLine(printer, msg);
    if (r)
        return r;
    // print shop address
    r = printer->SetPrintArea(0);
    if (r)
        return r;
    r = printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
    if (r)
        return r;
    if(this->_rateflag)
    {
        r = PrintGbkFromUtf8Ln(printer, (_rate + "元").toStdString().c_str(),msg);
        if (r)
        {
            return r;
        }
    }
    r = PrintGbkFromUtf8Ln(printer, str(format("%s-%s %s") % shopId % terminalId % shopAddress ).c_str(), msg);//C00087
    if (r)
        return r;

    r = PrintLine(printer, msg);//C00122
    if (r)//C00122
        return r;//C00122

    // print ad
    r = PrintGbkFromUtf8Ln(printer, ad.c_str(), msg);
    if (r)
        return r;
    r = printer->PageModePrint(PrintCommand::PTR_PM_CHECK_ERROR, msg); //C00139
    if(r) //C00139 //C00170
        return r; //C00139 //C00170
    r = PrintGbkFromUtf8Ln(printer, " ", msg);//C00122
    //if (r)//C00122 //C00139
    //return r;//C00122 //C00139
    r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg);
    if (errorCode == 0)
    {
        // print bar code
        r = printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
        r = printer->PrintBarCode(0, barCode.c_str(), barCode.size()
                                  , PrintCommand::PTR_BCS_PDF417, 0, 0, 0, 0, msg);
        //if (r) //C00139
        //return r; //C00139
    }
    else
    {
        // print line seperator
        r = PrintLine(printer, msg);
        //if (r) //C00139
        //return r; //C00139

        r = PrintGbkFromUtf8Ln(printer,
                               str(format("%-32s%s")  % ticketCode % password).c_str()
                               , msg);
        //if (r) //C00139
        //return r; //C00139

        r = PrintGbkFromUtf8Ln(printer,
                               str(format("%d: %s")  % errorCode % errorMsg).c_str()
                               , msg);
        //if (r) //C00139
        //return r; //C00139

        errorCode = 0;
        errorMsg.clear();
    }
    // leave page mode and print the ticket out
    //        r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg); //C00138
    //if (r) //C00139
    //return r; //C00139
    //        r = printer->MarkFeed(6, msg);//C00122 //C00140
    //if (r) //C00139
    //return r; //C00139
    r = printer->CutPaper(99, msg);
    //if (r) //C00139
    //return r; //C00139
    //C00138.start
    //        r = printer->PageModePrint(PrintCommand::PTR_PM_BYONLY, msg);
    //if (r) //C00139
    //return r; //C00139
    //C00138.end
    return Err::SUCCEEDED;
}

int BDPrintSupport::PrintSample ( ReceiptPrinter* printer, char* msg )
{
    demo = true;
    gameName = "XXXXX";
    ticketCode = "XXXXXXX-XXXXX-XXXXXX-XXXXXX";
    password = "XXXXXXXXXXXXX";
    drawNo = "00000";
    draws = 0;
    purchaseTime = "XXXX-XX-XX XX:XX:XX";
    multiple = 0;
    amount = 0;
    drawDate = "XXXX-XX-XX XX:XX";
    selectionType = "XXXXXX";
    selections.push_back("① XX+XX+XX+XX+XX+XX+XX");
    selections.push_back("② XX+XX+XX+XX+XX+XX+XX");
    selections.push_back("③ XX+XX+XX+XX+XX+XX+XX");
    selections.push_back("④ XX+XX+XX+XX+XX+XX+XX");
    selections.push_back("⑤ XX+XX+XX+XX+XX+XX+XX");
    ad = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    shopId = 9999999;
    terminalId = 9999999;
    shopAddress= "XXXXXXXXXXXXXXXXXXXX";
    barCode = "0000000-00000-000000-000000,xxxxxxxxxxxxx";

    return Print(printer, msg);
}
