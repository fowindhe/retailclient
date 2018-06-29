
#include "ReceiptPrinter.h"
#include "TicketPrintSupport.h"
#include <iostream>
#include "Iconv.h"
#include "Dictionary.h"
using namespace std;
//using boost::format;
TicketPrintSupport::TicketPrintSupport ( )
    : errorCode (0)
{
    _rateflag = false;
}

TicketPrintSupport::~TicketPrintSupport ( )
{
}
 void TicketPrintSupport::formatLine(string &src,int lineCnt=1){
    if(src.size()>42*lineCnt){
         QString tmpsa=QString::fromStdString(src);
        tmpsa=tmpsa.remove('\n');
         int tmpcount=0;
        for(int i=0;i<tmpsa.size();i++){
            ushort uni=tmpsa.at(i).unicode();
            if(uni >127 ){
                tmpcount+=2;
            }
            else{
                tmpcount++;
            }
            if(tmpcount>41*lineCnt)
            {
                tmpcount=i;
                break;
            }
        }
        src=tmpsa.mid(0,tmpcount).toStdString();
    }
}
int TicketPrintSupport::PrintGbkFromUtf8Ln (ReceiptPrinter* printer, const char* utf8Str, char* msg )
{
    Iconv	iconv("UTF-8", "GBK");
    int	r;
    string gbkStr;
    r = iconv.Convert(utf8Str, gbkStr);
    gbkStr.append("\n");
    r = printer->PrintNormal(0, gbkStr.c_str(), gbkStr.size(), msg);
    return r;
}

int TicketPrintSupport::PrintGbkFromUtf8 (ReceiptPrinter* printer, const char* utf8Str, char* msg )
{
    Iconv	iconv("UTF-8", "GBK");
    int	r;
    string gbkStr;
    r = iconv.Convert(utf8Str, gbkStr);
    r = printer->PrintNormal(0, gbkStr.c_str(), gbkStr.size(), msg);
    return r;
}

int TicketPrintSupport::PrintLine ( ReceiptPrinter* printer, char* msg )
{
    int r;
    //    char	scrollCmd[] = {0x1D, 0x5C, 0xEC, 0xFF};
    //    r = printer->PrintNormal(0, scrollCmd, sizeof(scrollCmd), msg);	// scroll back 20 units
    //    if (r)
    //        return r;


    //    const char	lineSeperator[] = "───────────────────────";
    r = printer->SetFontNormal(msg);
    r=printer->SetFont(0x00, 0x10, 0x01,msg);
    r = printer->SetFontBold(msg);
    r=printer->SetFontMagnify(1,1,msg);
    if (r)
        return r;

    r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);
    QString line = "------------------------------------------";
    qDebug()<<"linesize = "<<line.size()<<endl;
    r = PrintGbkFromUtf8Ln(printer,line.toStdString().c_str(), msg);
    if (r)
        return r;

    //    r = printer->PrintNormal(0, scrollCmd, sizeof(scrollCmd), msg);	// scroll back 20 units
    //    if (r)
    //        return r;

    return Err::SUCCEEDED;
}

int TicketPrintSupport::Print ( ReceiptPrinter* printer, char* msg )
{
    int	 r;
    QString  rsinfo;
    QString numflag []={"①","②","③","④","⑤","⑥","⑦","⑧","⑨","⑩"};
    string sn;
    struct sStyle
    {
        int front;
        int back;
        int fontWidth;
        int fontHeight;
    };
    typedef struct sStyle showStyle;
    string space="";
    int spaceNum;
    int i,j;
    r = printer->NewLineHeight(24, msg);
    if (r)
        return r;
    r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg);
    if (r)
        return r;
    r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);
    if (r)
        return r;
    string	title =gameName;
    string	gbkStr;
    string   tmpstr;
    bool	smallTitle = gbkStr.size() > 24;
    if (smallTitle)
    {
        r = printer->SetFontBold(msg);
        if (r)
            return r;
    }
    else
    {
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
    }
    r = PrintGbkFromUtf8Ln(printer, title.c_str(), msg);
    if (r)
        return r;
    r=printer->MarkFeed(2,msg);
    if (r)
        return r;
    r=printer->SetFontNormal(msg);
    if (r)
        return r;
    if (!smallTitle)
    {
        r = printer->SetFontMagnify(1, 1, msg);
        if (r)
            return r;
    }
    r = printer->SetFontNormal(msg);
    if (r)
        return r;

    r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);
    if (r)
        return r;
    tmpstr=ticketCode.substr(0,6)+"-"+ticketCode.substr(6,6)+"-"+ticketCode.substr(12,6)+
            "-"+ticketCode.substr(18,ticketCode.length()-18)+" "+password+"     "+SecurityCode;
    r = PrintGbkFromUtf8Ln(printer,
                           tmpstr.c_str()
                           , msg);
    if (r)
        return r;
    r = printer->PageModePrint(PrintCommand::PTR_PM_CHECK_ERROR, msg); //C00139
    if(r)
        return r;
    r = printer->SetFontNormal(msg);

    spaceNum=46-drawNo.size()-16-12;
    for(i=0;i<spaceNum;i++){
        space+=" ";
    }
    r=PrintGbkFromUtf8(printer,("第 "+drawNo+"期    共").c_str(),msg);

    r=printer->SetFontMagnify(2,1,msg);

    r=PrintGbkFromUtf8(printer,QString::number(draws).toStdString().c_str(),msg);

    r=printer->SetFontNormal(msg);

    r=printer->SetFontMagnify(1,1,msg);

    r=PrintGbkFromUtf8Ln(printer,("期"+space+
                                  drawDate.substr(2,2)+"/"+drawDate.substr(5,2)+
                                  "/"+drawDate.substr(8,2)+
                                  "开奖").c_str(),msg);

    space="";
    spaceNum=46-terminalId.size()-termSerialNo_.size()-19;
    for(i=0;i<spaceNum;i++){
        space+=" ";
    }
    if(GameID==51)
    {
        r = PrintGbkFromUtf8Ln(printer,
                               (terminalId.toStdString()+" "+termSerialNo_.toStdString()+space+purchaseTime.substr(2,purchaseTime.size())).c_str()
                               , msg);
    }
    else
    {
        r = PrintGbkFromUtf8Ln(printer,
                               (terminalId.toStdString()+" "+termSerialNo_.toStdString()+space+purchaseTime.substr(2,2)+"/"+purchaseTime.substr(5,2)+"/"+purchaseTime.substr(8,2)
                                +" "+purchaseTime.substr(11,2)+":"+purchaseTime.substr(14,2)+":"+purchaseTime.substr(17,2)).c_str()
                               , msg);
    }

    printer->MarkFeed(2,msg);
    printer->SetFontBold(msg);

    printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

    r=PrintGbkFromUtf8(printer,(selectionType+"  ").toStdString().c_str(),msg);

    r=printer->SetFontNormal(msg);

    r=printer->SetFontMagnify(2,1,msg);

    r=PrintGbkFromUtf8(printer,QString::number(multiple).toStdString().c_str(),msg);

    r=printer->SetFontMagnify(1,1,msg);

    r=PrintGbkFromUtf8(printer,QString("倍  合计").toStdString().c_str(),msg);

    r=printer->SetFontMagnify(2,1,msg);

    r=PrintGbkFromUtf8(printer,QString::number(amount).toStdString().c_str(),msg);

    r=printer->SetFontMagnify(1,1,msg);

    r=PrintGbkFromUtf8Ln(printer,QString("元").toStdString().c_str(),msg);
    printer->MarkFeed(2,msg);

    r = printer->NewLineHeight(80, msg);

    r=printer->SetFontBold(msg);

    if(GameID==42)
    {
        if(selectType==0)
        {
            r = PrintGbkFromUtf8Ln(printer, "        前区                   后区", msg);

            r=printer->SetFontNormal(msg);

            //            r=printer->SetFontMagnify(2,1,msg);
            r=printer->SetFont(0x01, 0x10, 0x01,msg);

            r = printer->SetFontBold(msg);

            for(i=0;i<selections.size();i++)
            {
                r=PrintGbkFromUtf8Ln(printer,QString(numflag[i]+selections[i]).toStdString().c_str(),msg);
            }
        }
        else
        {
            QString  type1[]={"前区胆","前区拖","后区胆","后区拖"};
            QString  type2[]={"前区 ","前区  ","后区","后区  "};
            QString  rsinfo="";
            for(j=0;j<4;j++){
                rsinfo="";
                if(selections[j].size()==0)
                    continue;
                if(selections[0]==""&&selections[2]=="")
                {
                    rsinfo+=type2[j];
                }
                else
                {
                    rsinfo+=type1[j];
                }
                r=printer->SetFontNormal(msg);

                r=printer->SetFont(0x01, 0x10, 0x01,msg);

                r = printer->SetFontBold(msg);

                r=printer->SetFontMagnify(1,1,msg);

                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

                r=printer->SetFontMagnify(2,1,msg);

                rsinfo="";
                for(i=0;i<selections[j].size()/21+1;i++){
                    if((i+1)*21<selections[j].size()){
                        if(i==0)
                            rsinfo+=selections[j].mid(i*21,21)+"\n";
                        else rsinfo+="   "+selections[j].mid(i*21,21)+"\n";

                    }
                    else {
                        if(i!=0)
                            rsinfo+="   "+selections[j].mid(i*21,selections[j].size());
                        else rsinfo+=selections[j].mid(i*21,selections[j].size());
                    }
                }

                rsinfo+="\n";
                r=printer->SetAlign(PrintCommand::ALIGN_LEFT,msg);

                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

            }
        }

    }
    else if(GameID==51)
    {
        rsinfo="";
        r=printer->SetFontNormal(msg);

        r=printer->SetFontMagnify(2,1,msg);

        if(selectType==0){
            if(subtype==1){
                for(i=0;i<selections.size();i++){
                    if(selections[i].size()>22){
                        r=printer->SetFont(0x01, 0x00, 0x01,msg);

                    }
                    else{
                        r=printer->SetFont(0x00, 0x00, 0x01,msg);
                    }
                    r=printer->SetFontMagnify(2,1,msg);
                    r=PrintGbkFromUtf8Ln(printer,QString(numflag[i]+" "+selections[i]).toStdString().c_str(),msg);

                }
            }
            else if(subtype==7||subtype==5||subtype==8||subtype==6){
                for(i=0;i<selections[0].size()/21+1;i++){
                    if((i+1)*21<selections[0].size()){
                        rsinfo+=selections[0].mid(i*21,21)+"\n";
                    }
                    else {
                        rsinfo+=selections[0].mid(i*21,selections[0].size());
                        for(j=0;j<(i+1)*21-selections[0].size();j++){
                            rsinfo+=" ";
                        }
                        rsinfo+="\n";
                    }
                }
                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

            }

        }
        else{
            QString  type1[]={"胆:   ","拖:   "};
            for(j=0;j<selections.size();j++){
                rsinfo="";

                r=printer->SetFontMagnify(1,1,msg);

                if(subtype==4)
                    rsinfo+="第"+numflag[j]+"位";
                else{
                    if(selections[j].size()==0)
                        continue;
                    else{
                        if(subtype==3){
                            if(selections[0].size()!=0)
                                rsinfo+=type1[j];
                        }
                    }
                }
                if(selectionType.contains("复式票")&&!selectionType.contains("定位")){
                    rsinfo+="      ";
                }
                if(selectionType.contains("单式票")&&selectionType.contains("任选一")){
                    rsinfo+="      ";
                }
                r=printer->SetFont(0x00, 0x00, 0x00,msg);

                r=printer->SetFontMagnify(1,1,msg);

                r=printer->SetAlign(PrintCommand::ALIGN_LEFT,msg);

                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

                rsinfo="";
                for(i=0;i<selections[j].size()/24+1;i++){
                    if((i+1)*24<selections[j].size()){
                        if(i==0)
                            rsinfo+=selections[j].mid(i*24,24)+"\n";
                        else rsinfo+="    "+selections[j].mid(i*24,24)+"\n";
                    }
                    else {
                        if(i!=0)
                            rsinfo+="    "+selections[j].mid(i*24,selections[j].size());
                        else rsinfo+=selections[j].mid(i*24,selections[j].size());
                    }
                }
                rsinfo+="\n";
                r=printer->SetFont(0x01, 0x00, 0x00,msg);

                r=printer->SetFontMagnify(2,1,msg);

                r=printer->SetAlign(PrintCommand::ALIGN_LEFT,msg);

                qDebug()<<rsinfo;
                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

            }
        }
    }
    else if(GameID==1||GameID==3||GameID==4)
    {
        r=printer->SetFontNormal(msg);

        r=printer->SetFontMagnify(2,1,msg);

        r=printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

        rsinfo="";
        if(selectionType=="直选单式票"||selectionType=="组选票"||selectionType=="单式票"){
            for(i=0;i<selections.size();i++){
                rsinfo+=numflag[i]+"  ";
                rsinfo+=selections[i]+"\n";
            }
        }
        else if(selectionType=="直选复式票"||selectionType=="复式票"){
            for(i=0;i<selections.size();i++){
                rsinfo+="第"+numflag[i]+"位：";
                rsinfo+=selections[i]+"\n";

            }
        }
        else{
            QStringList tmpqsl=selections[0].split("\n");
            if(tmpqsl.size()==1){
                rsinfo+=selections[0];
            }
            else{
                printer->SetAlign(PrintCommand::ALIGN_LEFT,msg);
                for(i=0;i<tmpqsl.size();i++)
                    rsinfo+="  "+tmpqsl[i]+"\n";
            }
        }
        r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

    }
    else if(GameID == 61 || GameID == 63 || GameID == 65 || GameID == 66)
    {
        r = printer->SetFontNormal(msg);

        r = printer->SetFontMagnify(1, 1, msg);

        r=printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

        for(i = 0; i < selections.size(); i++)
        {
            r = PrintGbkFromUtf8(printer,selections[i].toStdString().c_str(),msg);

        }
        r=PrintGbkFromUtf8Ln(printer,QString("\n").toStdString().c_str(),msg);
    }
    else if(GameID == 100000) //演示
    {
        r = PrintGbkFromUtf8(printer,"\n",msg);
        if (r)
            return r;
        r=printer->SetFont(0x00, 0x00, 0x00,msg);
        if (r)
            return r;
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
        for(i = 0; i < selections.size(); i++)
        {
            r = PrintGbkFromUtf8(printer,selections[i].toStdString().c_str(),msg);
            if (r)
                return r;
        }
    }
    r=printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

    r=printer->SetFont(0x00, 0x00, 0x01,msg);

    r=printer->SetFontMagnify(1,1,msg);

    r = printer->MarkFeed(2, msg);

    //    r=PrintGbkFromUtf8Ln(printer, ad.c_str(),msg);

    //    r=PrintGbkFromUtf8Ln(printer, shopAddress.c_str(),msg);
    cout<<"ad="<<ad<<"adsize="<<ad.size()<<"shopad="<<shopAddress<<"shopsize="<<shopAddress.size()<<endl;
    formatLine(ad);
    r=PrintGbkFromUtf8Ln(printer, ad.c_str(),msg);
    formatLine(shopAddress);

    cout<<"ad2222="<<ad<<"adsize="<<ad.size()<<"shop222ad="<<shopAddress<<"shopsize="<<shopAddress.size()<<endl;

    r=PrintGbkFromUtf8Ln(printer, shopAddress.c_str(),msg);

    r = printer->MarkFeed(2, msg);

    r=printer->PrintBarCode(0,barCode,barlen,PrintCommand::PTR_BCS_PDF417, 0, 0, 0, 0, msg);
    //    if (r)
    //        return r;
    //    char *tmp;
    //    barCode;
    //    strcpy(tmp,barCode,sizeof(barCode));
    //    r=printer->PrintBarCode(0,barCode.c_str(),strlen(barCode.c_str()),PrintCommand::PTR_BCS_PDF417, 0, 0, 0, 0, msg);
    r = printer->PageModePrint(PrintCommand::PTR_PM_PAGE_MODE, msg);
    r = printer->CutPaper(99, msg);



    return Err::SUCCEEDED;
}

int TicketPrintSupport::newPrintTicketNo( ReceiptPrinter* printer, char* msg )
{
    int	 r;
    QString title = QString::fromStdString(gameName);
    string tmpstr;
    printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

    QSettings localConfig(QString("../config/print_update.ini"),QSettings::IniFormat);
    localConfig.beginGroup(QString("PRINTER"));
    if(localConfig.value(QString("update"), "").toString() == "1")
    {
        switch(GameID)
        {
        case 1:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["排列3"]);
            break;
        case 3:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["排列5"]);
            break;
        case 4:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["7星彩"]);
            break;
        case 42:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["超级大乐透"]);
            break;
        case 51:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["11选5"]);
            break;
        case 61:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["14场胜负"]);
            break;
        case 63:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["任选9场胜负"]);
            break;
        case 65:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["6场半全场胜负"]);
            break;
        case 66:
            r = printer->PrintLoadLogo(0,0,Dictionary::Logo["4场进球"]);
            break;
        default:
            break;
        }
    }
    else
    {
        r = printer->NewLineHeight(24, msg);
        if (r)
            return r;
        r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);
        if (r)
            return r;
        string	gbkStr;
        bool	smallTitle = gbkStr.size() > 24;
        if (smallTitle)
        {
            r = printer->SetFontBold(msg);
            if (r)
                return r;
        }
        else
        {
            r = printer->SetFontMagnify(2, 1, msg);
            if (r)
                return r;
        }
        r = PrintGbkFromUtf8Ln(printer, (title + "\n").toStdString().c_str(), msg);
        if (r)
            return r;
    }
    if (r)
        return r;
    localConfig.endGroup();
    string space = "";
    int spaceNum;
    int i;

    r = printer->NewLineHeight(24, msg);
    if (r)
        return r;
    r=printer->SetFontNormal(msg);
    if (r)
        return r;
    r=printer->SetFontMagnify(1,1,msg);
    if (r)
        return r;

    r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);

    spaceNum = 43 - drawNo.size() - 23;
    for(i=0;i<spaceNum;i++)
    {
        space += " ";
    }
    QString printbuf = "第" + QString::fromStdString(drawNo) + "期" + QString::fromStdString(space) + QString::fromStdString(drawDate.substr(0,4)) + "年";
    printbuf += QString::fromStdString(drawDate.substr(5,2)) + "月" + QString::fromStdString(drawDate.substr(8,2)) + "日开奖";

    r=PrintGbkFromUtf8Ln(printer,printbuf.toStdString().c_str(),msg);

    tmpstr=ticketCode.substr(0,6)+"-"+ticketCode.substr(6,6)+"-"+ticketCode.substr(12,6)+"-"+ticketCode.substr(18,ticketCode.length()-18)+" "+password+"  "+SecurityCode;
    r = PrintGbkFromUtf8Ln(printer,tmpstr.c_str(), msg);
    if (r)
        return r;
    r = printer->PageModePrint(PrintCommand::PTR_PM_CHECK_ERROR, msg); //C00139
    if(r)
        return r;
    r = printer->NewLineHeight(14, msg);
    printer->MarkFeed(1,msg);
    if(r)
        return r;

    r = PrintLine(printer,msg);
    if(r)
        return r;

    return Err::SUCCEEDED;
}

int TicketPrintSupport::newPrintTicketInfo( ReceiptPrinter* printer, char* msg )
{
    cout<<" TicketPrintSupport::newPrintTicketInfo( ReceiptPrinter* printer, char* msg )"<<endl;
    int	 r;
    QString  rsinfo;
    QString numflag []={"①","②","③","④","⑤","⑥","⑦","⑧","⑨","⑩"};
    QString numflag1 []={"(1)","(2)","(3)","(4)","(5)","(6)","(7)","(8)","(9)","(10)"};
    struct sStyle
    {
        int front;
        int back;
        int fontWidth;
        int fontHeight;
    };
    typedef struct sStyle showStyle;
    string space="";
    QString  space1="";
    QString space2="";
    int spaceNum;
    int i,j;
    r = printer->SetFontNormal(msg);
    r=printer->SetFontMagnify(1,1,msg);
    r = printer->NewLineHeight(24, msg);
    printer->MarkFeed(1,msg);
    printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
    r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg);
    if (r)
        return r;

    //    QString tmp = selectionType + QString::number(multiple) + "倍" + "合计" + QString::number(amount) + "元";
    //    qDebug()<<"tmp = "<<tmp<<endl;
    //    qDebug()<<"tmpsize = "<<tmp.size()<<endl;

    if(GameID == 42 && selectionType.contains("追加投注"))
    {
        spaceNum = 42 - selectionType.size()*2 - QString::number(multiple).size() - QString::number(amount).size() - 7;
    }
    else if(GameID == 51 && draws > 1)
    {
        spaceNum = 42 - selectionType.size()*2 - QString::number(multiple).size() - QString::number(amount).size() - QString::number(draws).size() - 10;
    }
    else
        spaceNum = 42 - selectionType.size()*2 - QString::number(multiple).size() - QString::number(amount).size() - 8;//42为最大宽度 8为4个汉字的长度
    for(i = 0; i < spaceNum/2; i++)
    {
        space1+=" ";
    }
    for(i = 0; i < spaceNum - spaceNum/2; i++)
    {
        space2+=" ";
    }
    QString PrintBuf = "";
    if(GameID == 51 && draws > 1)
        PrintBuf = selectionType + space1 +  QString::number(draws) +"期"+ QString::number(multiple) + "倍" + space2 + "合计" + QString::number(amount) + "元";
    else
        PrintBuf = selectionType + space1 + QString::number(multiple) + "倍" + space2 + "合计" + QString::number(amount) + "元";

    r=PrintGbkFromUtf8(printer,(PrintBuf).toStdString().c_str(),msg);

    printer->MarkFeed(2,msg);
    r = printer->NewLineHeight(80, msg);
    r = printer->SetFontBold(msg);
    int selectionsLineNo=2;
    if(GameID == 42)
    {
        if(selectType == 0)
        {

            for(int x = 0; x < selections.size(); x++)
            {
                selections[x].replace(' ','s');
                selections[x].replace('+',' ');
                selections[x].replace('s','+');
                selections[x].insert(selections[x].size() - 6,"  ");
                selections[x].insert(selections[x].size() - 5,"  ");
            }
            printer->SetFont(0x01, 0x10, 0x01,msg);
            printer->SetFontMagnify(2,1,msg);
            selectionsLineNo+= selections.size();
            if(selectionsLineNo==3){
                printer->MarkFeed(3,msg);
                selectionsLineNo+=2;
            }
            else if(selectionsLineNo<7){
                printer->MarkFeed(2,msg);
                selectionsLineNo+=1;
            }
            for(i=0;i<selections.size();i++)
            {
                if(selections.size() == 1)
                {
                    PrintGbkFromUtf8Ln(printer,QString(selections[i]).toStdString().c_str(),msg);
                }
                else
                {
                    printer->SetFontMagnify(1,1,msg);
                    PrintGbkFromUtf8(printer,QString(numflag[i]).toStdString().c_str(),msg);
                    printer->SetFontMagnify(2,1,msg);
                    PrintGbkFromUtf8Ln(printer,QString(selections[i]).toStdString().c_str(),msg);
                }
            }
        }
        else
        {
            QString  type1[]={"前区胆","前区拖","后区胆","后区拖"};
            QString  type2[]={" ","前区  "," ","后区  "};
            QString  rsinfo="";
            if(selections[0] == "" && selections[2] == "")
            {
                printer->MarkFeed(2,msg);
                selectionsLineNo+=1;
            }
            else{
                if(selections[1].size()<22&&selections[3].size()<22){
                    printer->MarkFeed(2,msg);
                    selectionsLineNo+=1;
                }
            }
            for(i = 0; i < 4; i++)
            {
                rsinfo="";
                if(selections[0] == "" && selections[2] == "")
                {
                    rsinfo+=type2[i];
                }
                else
                {
                    rsinfo+=type1[i];
                }
                printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
                printer->SetFontNormal(msg);
                printer->SetFontMagnify(1,1,msg);
                PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);
                printer->SetFont(0x01, 0x10, 0x01,msg);
                printer->SetFontMagnify(2,1,msg);

                QList <QString> tmp;
                tmp = selections[i].split(" ");
                for(int i = 0; i < tmp.size(); i++)
                {
                    if(tmp[i] == "")
                        tmp.removeAt(i);
                }

                for(int i = 0; i < tmp.size(); i++)
                {
                    PrintGbkFromUtf8(printer,(tmp[i] + " ").toStdString().c_str(),msg);
                    if((i + 1)%7 == 0)
                    {
                        if((i + 1) != tmp.size())
                        {
                            PrintGbkFromUtf8(printer,"\n",msg);
                            PrintGbkFromUtf8(printer,"    ",msg);
                            selectionsLineNo+=1;
                        }
                        else
                        {
                            PrintGbkFromUtf8(printer,"\n",msg);
                            selectionsLineNo+=1;

                        }

                    }
                }
                if(tmp.size()%7 != 0 || tmp.size() == 0)
                {
                    for(int x = 0; x < 7 - tmp.size()%7; x++)
                    {
                        r=PrintGbkFromUtf8(printer,"   ",msg);
                    }
                    PrintGbkFromUtf8(printer,"\n",msg);
                    selectionsLineNo+=1;
                }
            }
        }
        if(selectionsLineNo<7)
        {
            printer->MarkFeed(8-selectionsLineNo, msg);
            PrintLine(printer,msg);
            selectionsLineNo=8;
        }
        else{
            if(selectionsLineNo<9||_giftnum==""){
                PrintLine(printer,msg);
                selectionsLineNo++;
            }
        }
        //        if(selections.size() == 1)
        //        {
        //            printer->MarkFeed(3, msg);
        //            PrintLine(printer,msg);
        //        }
        //        else
        //            PrintLine(printer,msg);
    }
    else if(GameID == 51)
    {
        rsinfo="";
        printer->SetFontMagnify(2,1,msg);
        printer->SetFont(0x01, 0x10, 0x01,msg);
        printer->SetFontBold(msg);
        if(selectType == 0)
        {
             selectionsLineNo+= selections.size();
            if(subtype == 1)
            {
//                if(selections.size() == 1 || selectionType.contains("任选一"))
//                {
//                    printer->MarkFeed(3, msg);
//                    selectionsLineNo+=2;
//                }
//                else
//                {
//                    printer->MarkFeed(2, msg);
//                }
                if(selectionsLineNo==3||selectionType.contains("任选一")){
                    printer->MarkFeed(3,msg);
                    selectionsLineNo+=2;
                }
                else if(selectionsLineNo>3&&selectionsLineNo<7){
                    printer->MarkFeed(2,msg);
                    selectionsLineNo+=1;
                }
                for(i = 0; i < selections.size(); i++)
                {
                    printer->SetFontMagnify(2,1,msg);
                    selections[i].replace("+"," ");
                    printer->SetFontMagnify(1,1,msg);
                    if(selections.size() != 1)
                        PrintGbkFromUtf8(printer,(numflag[i]+ " ").toStdString().c_str(),msg);
                    printer->SetFontMagnify(2,1,msg);
                    PrintGbkFromUtf8Ln(printer,selections[i].toStdString().c_str(),msg);
                }
            }
            else if(subtype == 7 || subtype == 5 || subtype == 8 || subtype == 6)
            {
                printer->MarkFeed(2,msg);
                selectionsLineNo+=1;
                printer->SetFont(0x01, 0x10, 0x01,msg);
                printer->SetFontMagnify(2,1,msg);
                printer->SetFontBold(msg);
                for(i=0;i<selections[0].size()/21+1;i++)
                {
                    if((i+1)*21<selections[0].size())
                    {
                        rsinfo += selections[0].mid(i*21,21)+"\n";
                        selectionsLineNo+=1;
                    }
                    else
                    {
                        rsinfo += selections[0].mid(i*21,selections[0].size());
                        for(j = 0;j < (i+1)*21 - selections[0].size(); j++)
                        {
                            rsinfo+=" ";
                        }
                        rsinfo+="\n";
                        selectionsLineNo+=1;
                    }
                }
//                printer->MarkFeed(2, msg);
                PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);
            }
            else if(subtype == 14)
            {
                printer->SetFont(0x01, 0x10, 0x01,msg);
                printer->SetFontMagnify(2,1,msg);
                printer->SetFontBold(msg);
                rsinfo += selections[0];
                rsinfo += "\n";
                selectionsLineNo+=1;
                PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);
            }
        }
        else
        {
            QString  type1[] = {"胆码: ","拖码: "};
            for(j = 0; j < selections.size(); j++)
            {
                rsinfo = "";
                printer->SetFontMagnify(1,1,msg);
                if(subtype==4)
                    rsinfo+="第"+numflag[j]+"位";
                else
                {
                    if(selections[j].size() == 0)
                        continue;
                    else
                    {
                        if(subtype == 3)
                        {
                            if(selections[0].size() != 0)
                            {
                                rsinfo += type1[j];
                            }
                        }
                    }
                }
                if(selectionType.contains("复式票")&&!selectionType.contains("定位"))
                {
                    rsinfo += "      ";
                }
                if(selectionType.contains("单式票")&&selectionType.contains("任选一"))
                {
                    rsinfo += "      ";
                }
                printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
                printer->MarkFeed(2, msg);
                selectionsLineNo+=1;
                PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

                rsinfo = "";
                for(i = 0; i < selections[j].size()/24 + 1; i++)
                {
                    if(selections[j].size() == 24 && i == 1)
                        continue;
                    if((i+1)*24 < selections[j].size())
                    {
                        if(i==0)
                        {
                            rsinfo += selections[j].mid(i*24,24);
                            for(int x = 0; x < 24 - selections[j].mid(i*24,24).size(); x++)
                                rsinfo += " ";
                            rsinfo += "\n";
                            selectionsLineNo+=1;
                        }
                        else
                        {
                            rsinfo+="     "+selections[j].mid(i*24,24);
                            for(int x = 0; x < 24 - selections[j].mid(i*24,24).size(); x++)
                                rsinfo += " ";
                        }
                    }
                    else
                    {
                        if(i != 0)
                        {
                            if(selectionType.contains("定位复式票"))
                                rsinfo += "    " + selections[j].mid(i*24,selections[j].size());
                            else if(selectionType.contains("复式票"))
                                rsinfo += "   " + selections[j].mid(i*24,selections[j].size());
                            else
                                rsinfo += "    " + selections[j].mid(i*24,selections[j].size());
                            for(int x = 0; x < 24 - selections[j].mid(i*24,selections[j].size()).size(); x++)
                            {
                                rsinfo += " ";
                            }
                        }
                        else
                        {
                            rsinfo += selections[j].mid(i*24,selections[j].size());
                            for(int x = 0; x < 24 - selections[j].mid(i*24,selections[j].size()).size(); x++)
                                rsinfo += " ";
                        }
                    }
                }
                printer->SetFontMagnify(2,1,msg);
                printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
                qDebug()<<rsinfo;
                PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);
            }
            PrintGbkFromUtf8(printer,"\n",msg);
            selectionsLineNo+=1;
        }

        if(selectionsLineNo<7)
        {
            printer->MarkFeed(8-selectionsLineNo, msg);
            PrintLine(printer,msg);
            selectionsLineNo=8;
        }
        else{
            PrintLine(printer,msg);
            selectionsLineNo=8;
        }
//        if(selections.size() == 1 || selectionType.contains("任选一"))
//        {
//            r = printer->MarkFeed(3, msg);
//            PrintLine(printer,msg);
//        }
//        else
//            PrintLine(printer,msg);
    }
    else if(GameID == 4 || GameID == 1 || GameID == 3)
    {
        r=printer->SetFontNormal(msg);
        r=printer->SetFontMagnify(2,1,msg);
        r=printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
        r=printer->SetFont(0x01, 0x10, 0x01,msg);
        r=printer->SetFontBold(msg);
        selectionsLineNo+= selections.size();
        if(selectionsLineNo==3){
            printer->MarkFeed(3,msg);
            selectionsLineNo+=2;
        }
        else if(selectionsLineNo<7){
            printer->MarkFeed(2,msg);
            selectionsLineNo+=1;
        }
        if(selections.size() == 1)
        {
            r=printer->SetFontMagnify(2,1,msg);
//            r = printer->MarkFeed(3, msg);
            if(selectionType == "直选组合复式票" || selectionType == "组选6复式票" || selectionType == "组选3复式票" || selectionType == "组选和值票" || selectionType == "直选和值票" )
            {
                QList <QString> tmp = selections[0].split(" ");
                qDebug() << tmp<<endl;
                for(int i = 0; i < tmp.size(); i++)
                {
                    tmp[i].remove("\n");
                    if(tmp[i] == "")
                        tmp.removeAt(i);
                }
                for(int i = 0; i < tmp.size(); i++)
                {
                    r=printer->SetFontMagnify(2,1,msg);
                    if(tmp[i].toInt() < 10)
                        r=PrintGbkFromUtf8(printer, (" " + tmp[i] + "  ").toStdString().c_str(),msg);
                    else
                        r=PrintGbkFromUtf8(printer,(tmp[i] + "  ").toStdString().c_str(),msg);
                    if((i + 1) %5 == 0)
                    {
                        r=PrintGbkFromUtf8(printer,"\n",msg);
                    }
                }
                for(int x = 0; x < 5 - tmp.size()%5; x++)
                {
                    r=PrintGbkFromUtf8(printer,"    ",msg);
                }
            }
            else
            {
                for(int i = 0; i < selections[0].size(); i++)
                {
                    r=printer->SetFontMagnify(2,1,msg);
                    r=PrintGbkFromUtf8(printer,selections[0].left(i+1).right(1).toStdString().c_str(),msg);
                    r=printer->SetFontMagnify(1,1,msg);
                    r=PrintGbkFromUtf8(printer," ",msg);
                }
            }

            r=PrintGbkFromUtf8(printer,"\n",msg);
            selectionsLineNo+=1;
        }
        else
        {
            if(selectionType == "定位复式票" || selectionType == "直选复式票" || selectionType == "直选定位复式票")
            {
                for(int i = 0; i < selections.size(); i++)
                {
                    selections[i].remove(".");
                    r=printer->SetFontMagnify(1,1,msg);
                    QString tmp = "第"+numflag[i]+"位：";
                    r=PrintGbkFromUtf8(printer,tmp.toStdString().c_str(),msg);
                    r=printer->SetFontMagnify(2,1,msg);
                    for(int j = 0; j < selections[i].size(); j++)
                    {
                        r=printer->SetFontMagnify(2,1,msg);
                        r=PrintGbkFromUtf8(printer,selections[i].left(j+1).right(1).toStdString().c_str(),msg);
                        r=printer->SetFontMagnify(1,1,msg);
                        r=PrintGbkFromUtf8(printer,"  ",msg);
                    }
                    int _space = 10 - selections[i].size();
                    for(int x = 0; x <  _space; x++)
                    {
                        r=printer->SetFontMagnify(2,1,msg);
                        r=PrintGbkFromUtf8(printer," ",msg);
                        r=printer->SetFontMagnify(1,1,msg);
                        r=PrintGbkFromUtf8(printer,"  ",msg);
                    }
                    r=PrintGbkFromUtf8(printer,"\n",msg);
                }
            }
            else if(selectionType == "直选单式票" || selectionType == "组选票" || selectionType == "单式票")
            {
                for(int i = 0; i < selections.size(); i++)
                {
                    r=printer->SetFontMagnify(1,1,msg);
                    r=PrintGbkFromUtf8(printer,(numflag[i] + "   ").toStdString().c_str(),msg);
                    r=printer->SetFontMagnify(2,1,msg);
                    for(int j = 0; j < selections[i].size(); j++)
                    {
                        r=printer->SetFontMagnify(2,1,msg);
                        r=PrintGbkFromUtf8(printer,selections[i].left(j+1).right(1).toStdString().c_str(),msg);
                        r=printer->SetFontMagnify(1,1,msg);
                        r=PrintGbkFromUtf8(printer," ",msg);
                    }
                    r=PrintGbkFromUtf8(printer,"\n",msg);
//                    selectionsLineNo+=1;
                }
            }
        }
        if(selectionsLineNo<7)
        {
            printer->MarkFeed(8-selectionsLineNo, msg);
            PrintLine(printer,msg);
            selectionsLineNo=8;
        }
        else{
                PrintLine(printer,msg);
                selectionsLineNo++;

        }
//        if(selections.size() == 1)
//        {
//            printer->MarkFeed(3, msg);
//            PrintLine(printer,msg);
//        }
//        else
//            PrintLine(printer,msg);
    }
    else if(GameID == 66)
    {
        printer->SetFontNormal(msg);
        printer->SetFontMagnify(1, 1, msg);
        printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
        QString Fillchar = ".  .  .  .  .  .  .  .  ";
        if(selectionType == "单式票")
        {
            if(selectionsLineNo==3){
                printer->MarkFeed(3,msg);
                selectionsLineNo+=2;
            }
            else if(selectionsLineNo<7){
                printer->MarkFeed(2,msg);
                selectionsLineNo+=1;
            }
            PrintGbkFromUtf8(printer,selections[0].toStdString().c_str(),msg);
            printer->SetFontBold(msg);
            selectionsLineNo+=selections.size();

            for(i = 1; i < selections.size(); i++)
            {
                PrintGbkFromUtf8(printer,selections[i].toStdString().c_str(),msg);
            }
            PrintGbkFromUtf8(printer,"\n",msg);
            for(int x = selections.size() - 1; x < 5; x++)
            {
                PrintGbkFromUtf8(printer,numflag[x].toStdString().c_str(),msg);
                PrintGbkFromUtf8Ln(printer,Fillchar.toStdString().c_str(),msg);
            }
        }
        else
        {
            printer->SetFontBold(msg);
            QList <QString> _selections;
            _selections = selections[1].split('\n');
            //            qDebug()<<"selections:"<<_selections.size()<<endl;
            for(int i = 1; i < _selections.size(); i++)
            {
                _selections[i] = _selections[i].right(24);
                _selections[i].replace("-","*");
                //                qDebug()<<"size = "<<_selections[i].size()<<endl;
                //                cout<<"_selections = "<<_selections[i].toStdString()<<endl;
            }
            QString _printBuf = "";
            int index = 0;
            for(int i = 0; i < _oldSoccerTeam.size(); i++)
            {
                _printBuf += numflag1[i] + " " + _oldSoccerTeam[i] + " ";
                for(int j = 1; j < 5; j++)
                {
                    _printBuf += _selections[j].mid(index,2);
                }
                _printBuf += " ";
                if(i != 0 && (i+1)%2 == 0)
                    _printBuf += "\n";
                index += 3;
            }
            PrintGbkFromUtf8Ln(printer,_printBuf.toStdString().c_str(),msg);
        }
        PrintLine(printer,msg);
    }
    else if(GameID == 61 || GameID == 63 || GameID == 65)
    {
        printer->SetFontNormal(msg);
        printer->SetFontMagnify(1, 1, msg);
        printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
        QString Fillchar;
        if(GameID == 65)
            Fillchar = ".  .  .  .  .  .  .  .  .  .  .  .  ";
        else
            Fillchar = ".  .  .  .  .  .  .  .  .  .  .  .  .  .  ";
        if(selectionType == "单式票")
        {

            PrintGbkFromUtf8(printer,selections[0].toStdString().c_str(),msg);
            printer->SetFontBold(msg);
            for(i = 1; i < selections.size(); i++)
            {
                PrintGbkFromUtf8(printer,selections[i].toStdString().c_str(),msg);
            }
            PrintGbkFromUtf8(printer,"\n",msg);
            for(int x = selections.size() - 1; x < 5; x++)
            {
                PrintGbkFromUtf8(printer,numflag[x].toStdString().c_str(),msg);
                PrintGbkFromUtf8Ln(printer,Fillchar.toStdString().c_str(),msg);
            }
        }
        else
        {
            PrintGbkFromUtf8(printer,selections[0].toStdString().c_str(),msg);
            printer->SetFontBold(msg);
            for(i = 1; i < selections.size(); i++)
            {
                selections[i].replace("-",".");
                r = PrintGbkFromUtf8(printer,selections[i].toStdString().c_str(),msg);
            }
            PrintGbkFromUtf8(printer,"\n",msg);
        }
        PrintLine(printer,msg);
    }
    else if(GameID == 100000) //演示
    {
        r = PrintGbkFromUtf8(printer,"\n",msg);
        if (r)
            return r;
        r=printer->SetFont(0x00, 0x00, 0x00,msg);
        if (r)
            return r;
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
        for(i = 0; i < selections.size(); i++)
        {
            r = PrintGbkFromUtf8(printer,selections[i].toStdString().c_str(),msg);
            if (r)
                return r;
        }
    }
    printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);
    printer->SetFont(0x00, 0x00, 0x01,msg);
    printer->SetFontNormal(msg);
    printer->SetFontMagnify(1,1,msg);
    printer->SetPrintArea(0);
    int adLineNo=0;
    if(GameID == 42 && _giftnum != "")
    {
        printer->SetFont(0x01, 0x00, 0x01,msg);
        printer->SetFontBold(msg);
        PrintGbkFromUtf8(printer,QString("乐善码 ").toStdString().c_str(),msg);
        printer->SetFont(0x01, 0x10, 0x01,msg);
        printer->SetFontMagnify(2,1,msg);
        PrintGbkFromUtf8Ln(printer, _giftnum.toStdString().c_str(),msg);
        printer->SetFont(0x00, 0x00, 0x01,msg);
        printer->SetFontNormal(msg);
        printer->SetFontMagnify(1,1,msg);
        adLineNo+=1;
    }
    if(selectionsLineNo+adLineNo<11)
    {
        formatLine(shopAddress);
        r=PrintGbkFromUtf8Ln(printer, shopAddress.c_str(),msg);
        adLineNo+=1;
    }
    if(selectionsLineNo+adLineNo<11){
        if(this->_rateflag)
        {
            PrintGbkFromUtf8Ln(printer, (_rate + "元").toStdString().c_str(),msg);
        }
    }
    //    PrintGbkFromUtf8Ln(printer, ad.c_str(),msg);
    //    PrintGbkFromUtf8Ln(printer, shopAddress.c_str(),msg);
    if(selectionsLineNo+adLineNo<11){
        cout<<"before===="<<ad<<endl;
        formatLine(ad,3-adLineNo);
        cout<<"after*****"<<ad<<endl;

        r=PrintGbkFromUtf8Ln(printer, ad.c_str(),msg);
    }
    space="";
    spaceNum=42-terminalId.size()-termSerialNo_.size()-19;
    for(i=0;i<spaceNum;i++)
    {
        space+=" ";
    }
    if(GameID==51)
    {
        r = PrintGbkFromUtf8Ln(printer, (terminalId.toStdString()+" "+termSerialNo_.toStdString()+space+purchaseTime.substr(2,purchaseTime.size())).c_str(),msg);
    }
    else
    {
        r = PrintGbkFromUtf8Ln(printer,
                               (terminalId.toStdString()+" "+termSerialNo_.toStdString()+space+purchaseTime.substr(2,2)+"/"+purchaseTime.substr(5,2)+"/"+purchaseTime.substr(8,2)
                                +" "+purchaseTime.substr(11,2)+":"+purchaseTime.substr(14,2)+":"+purchaseTime.substr(17,2)).c_str()
                               , msg);
    }

//    r = printer->MarkFeed(1, msg);
    r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg);
    if (r)
        return r;
    r=printer->PrintBarCode(0,barCode,barlen,PrintCommand::PTR_BCS_PDF417, 0, 0, 0, 0, msg);
    r = printer->PageModePrint(PrintCommand::PTR_PM_PAGE_MODE, msg);
    r = printer->CutPaper(99, msg);

    return Err::SUCCEEDED;
}


int TicketPrintSupport::PrintTicketNo( ReceiptPrinter* printer, char* msg )
{
    int	 r;
    struct sStyle
    {
        int front;
        int back;
        int fontWidth;
        int fontHeight;
    };
    typedef struct sStyle showStyle;

    r = printer->NewLineHeight(24, msg);
    if (r)
        return r;
    r = printer->SetAlign(PrintCommand::ALIGN_CENTER, msg);
    if (r)
        return r;

    string	title =gameName;
    string	gbkStr;
    string   tmpstr;
    bool	smallTitle = gbkStr.size() > 24;
    if (smallTitle)
    {
        r = printer->SetFontBold(msg);
        if (r)
            return r;
    }
    else
    {
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
    }
    r = PrintGbkFromUtf8Ln(printer, title.c_str(), msg);
    if (r)
        return r;
    r=printer->MarkFeed(2,msg);
    if (r)
        return r;
    r=printer->SetFontNormal(msg);
    if (r)
        return r;
    if (!smallTitle)
    {
        r = printer->SetFontMagnify(1, 1, msg);
        if (r)
            return r;
    }
    r = printer->SetFontNormal(msg);
    if (r)
        return r;

    r = printer->PageModePrint(PrintCommand::PTR_PM_YTDONLY, msg);
    if (r)
        return r;
    r = printer->SetAlign(PrintCommand::ALIGN_LEFT, msg);
    if (r)
        return r;
    tmpstr=ticketCode.substr(0,6)+"-"+ticketCode.substr(6,6)+"-"+ticketCode.substr(12,6)+
            "-"+ticketCode.substr(18,ticketCode.length()-18)+" "+password+"     "+SecurityCode;
    r = PrintGbkFromUtf8Ln(printer,
                           tmpstr.c_str()
                           , msg);
    if (r)
        return r;
    r = printer->PageModePrint(PrintCommand::PTR_PM_CHECK_ERROR, msg); //C00139
    if(r)
        return r;

    return Err::SUCCEEDED;
}

int TicketPrintSupport::PrintTicketInfo( ReceiptPrinter* printer, char* msg )
{
        cout<<" TicketPrintSupport::PrintTicketInfo"<<endl;
    int	 r;
    QString  rsinfo;
    QString numflag []={"①","②","③","④","⑤","⑥","⑦","⑧","⑨","⑩"};
    string sn;
    struct sStyle
    {
        int front;
        int back;
        int fontWidth;
        int fontHeight;
    };
    typedef struct sStyle showStyle;
    string space="";
    int spaceNum;
    int i,j;
    r = printer->SetFontNormal(msg);

    spaceNum=46-drawNo.size()-16-12;
    for(i=0;i<spaceNum;i++){
        space+=" ";
    }
    r=PrintGbkFromUtf8(printer,("第 "+drawNo+"期    共").c_str(),msg);

    r=printer->SetFontMagnify(2,1,msg);

    r=PrintGbkFromUtf8(printer,QString::number(draws).toStdString().c_str(),msg);

    r=printer->SetFontNormal(msg);

    r=printer->SetFontMagnify(1,1,msg);

    r=PrintGbkFromUtf8Ln(printer,("期"+space+
                                  drawDate.substr(2,2)+"/"+drawDate.substr(5,2)+
                                  "/"+drawDate.substr(8,2)+
                                  "开奖").c_str(),msg);

    space="";
    spaceNum=46-terminalId.size()-termSerialNo_.size()-19;
    for(i=0;i<spaceNum;i++){
        space+=" ";
    }
    if(GameID==51)
    {
        r = PrintGbkFromUtf8Ln(printer,
                               (terminalId.toStdString()+" "+termSerialNo_.toStdString()+space+purchaseTime.substr(2,purchaseTime.size())).c_str()
                               , msg);
    }
    else
    {
        r = PrintGbkFromUtf8Ln(printer,
                               (terminalId.toStdString()+" "+termSerialNo_.toStdString()+space+purchaseTime.substr(2,2)+"/"+purchaseTime.substr(5,2)+"/"+purchaseTime.substr(8,2)
                                +" "+purchaseTime.substr(11,2)+":"+purchaseTime.substr(14,2)+":"+purchaseTime.substr(17,2)).c_str()
                               , msg);
    }

    printer->MarkFeed(2,msg);
    printer->SetFontBold(msg);

    printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

    r=PrintGbkFromUtf8(printer,(selectionType+"  ").toStdString().c_str(),msg);

    r=printer->SetFontNormal(msg);

    r=printer->SetFontMagnify(2,1,msg);

    r=PrintGbkFromUtf8(printer,QString::number(multiple).toStdString().c_str(),msg);

    r=printer->SetFontMagnify(1,1,msg);

    r=PrintGbkFromUtf8(printer,QString("倍  合计").toStdString().c_str(),msg);

    r=printer->SetFontMagnify(2,1,msg);

    r=PrintGbkFromUtf8(printer,QString::number(amount).toStdString().c_str(),msg);

    r=printer->SetFontMagnify(1,1,msg);

    r=PrintGbkFromUtf8Ln(printer,QString("元").toStdString().c_str(),msg);
    printer->MarkFeed(2,msg);

    r = printer->NewLineHeight(80, msg);

    r=printer->SetFontBold(msg);

    if(GameID==42)
    {
        if(selectType==0)
        {
            r = PrintGbkFromUtf8Ln(printer, "        前区                   后区", msg);

            r=printer->SetFontNormal(msg);

            r=printer->SetFontMagnify(2,1,msg);

            for(i=0;i<selections.size();i++)
            {
                r=PrintGbkFromUtf8Ln(printer,QString(numflag[i]+selections[i]).toStdString().c_str(),msg);
            }
        }
        else
        {
            QString  type1[]={"前区胆","前区拖","后区胆","后区拖"};
            QString  type2[]={"前区 ","前区  ","后区","后区  "};
            QString  rsinfo="";
            for(j=0;j<4;j++)
            {
                rsinfo="";
                if(selections[j].size()==0)
                    continue;
                if(selections[0]==""&&selections[2]=="")
                {
                    rsinfo+=type2[j];
                }
                else
                {
                    rsinfo+=type1[j];
                }
                r=printer->SetFontNormal(msg);

                r=printer->SetFont(0x00, 0x00, 0x00,msg);

                r=printer->SetFontMagnify(1,1,msg);

                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

                r=printer->SetFontMagnify(2,1,msg);

                rsinfo="";
                for(i=0;i<selections[j].size()/21+1;i++)
                {
                    if((i+1)*21<selections[j].size())
                    {
                        if(i==0)
                            rsinfo+=selections[j].mid(i*21,21)+"\n";
                        else rsinfo+="   "+selections[j].mid(i*21,21)+"\n";

                    }
                    else
                    {
                        if(i!=0)
                            rsinfo+="   "+selections[j].mid(i*21,selections[j].size());
                        else rsinfo+=selections[j].mid(i*21,selections[j].size());
                    }
                }

                rsinfo+="\n";
                r=printer->SetAlign(PrintCommand::ALIGN_LEFT,msg);

                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

            }
        }

    }
    else if(GameID==51)
    {
        rsinfo="";
        r=printer->SetFontNormal(msg);

        r=printer->SetFontMagnify(2,1,msg);

        if(selectType==0){
            if(subtype==1){
                for(i=0;i<selections.size();i++){
                    if(selections[i].size()>22){
                        r=printer->SetFont(0x01, 0x00, 0x01,msg);

                    }
                    else{
                        r=printer->SetFont(0x00, 0x00, 0x01,msg);
                    }
                    r=printer->SetFontMagnify(2,1,msg);
                    r=PrintGbkFromUtf8Ln(printer,QString(numflag[i]+" "+selections[i]).toStdString().c_str(),msg);

                }
            }
            else if(subtype==7||subtype==5||subtype==8||subtype==6){
                for(i=0;i<selections[0].size()/21+1;i++){
                    if((i+1)*21<selections[0].size()){
                        rsinfo+=selections[0].mid(i*21,21)+"\n";
                    }
                    else {
                        rsinfo+=selections[0].mid(i*21,selections[0].size());
                        for(j=0;j<(i+1)*21-selections[0].size();j++){
                            rsinfo+=" ";
                        }
                        rsinfo+="\n";
                    }
                }
                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);
            }
            else if(subtype==14){
                rsinfo+=selections[0];
                rsinfo+="\n";
                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);
            }
        }
        else{
            QString  type1[]={"胆:   ","拖:   "};
            for(j=0;j<selections.size();j++){
                rsinfo="";

                r=printer->SetFontMagnify(1,1,msg);

                if(subtype==4)
                    rsinfo+="第"+numflag[j]+"位";
                else{
                    if(selections[j].size()==0)
                        continue;
                    else{
                        if(subtype==3){
                            if(selections[0].size()!=0)
                                rsinfo+=type1[j];
                        }
                    }
                }
                if(selectionType.contains("复式票")&&!selectionType.contains("定位")){
                    rsinfo+="      ";
                }
                if(selectionType.contains("单式票")&&selectionType.contains("任选一")){
                    rsinfo+="      ";
                }
                r=printer->SetFont(0x00, 0x00, 0x00,msg);

                r=printer->SetFontMagnify(1,1,msg);

                r=printer->SetAlign(PrintCommand::ALIGN_LEFT,msg);

                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

                rsinfo="";
                for(i=0;i<selections[j].size()/24+1;i++){
                    if((i+1)*24<selections[j].size()){
                        if(i==0)
                            rsinfo+=selections[j].mid(i*24,24)+"\n";
                        else rsinfo+="    "+selections[j].mid(i*24,24)+"\n";
                    }
                    else {
                        if(i!=0)
                            rsinfo+="    "+selections[j].mid(i*24,selections[j].size());
                        else rsinfo+=selections[j].mid(i*24,selections[j].size());
                    }
                }
                rsinfo+="\n";
                r=printer->SetFont(0x01, 0x00, 0x00,msg);

                r=printer->SetFontMagnify(2,1,msg);

                r=printer->SetAlign(PrintCommand::ALIGN_LEFT,msg);

                qDebug()<<rsinfo;
                r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

            }
        }
    }
    else if(GameID==1||GameID==3||GameID==4)
    {
        r=printer->SetFontNormal(msg);

        r=printer->SetFontMagnify(2,1,msg);

        r=printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

        rsinfo="";
        if(selectionType=="直选单式票"||selectionType=="组选票"||selectionType=="单式票"){
            for(i=0;i<selections.size();i++){
                rsinfo+=numflag[i]+"  ";
                rsinfo+=selections[i]+"\n";
            }
        }
        else if(selectionType == "定位复式票" || selectionType == "直选复式票" || selectionType == "直选定位复式票")
        {
            for(i=0;i<selections.size();i++)
            {
                rsinfo+="第"+numflag[i]+"位：";
                rsinfo+=selections[i]+"\n";
            }
        }
        else
        {
            QStringList tmpqsl=selections[0].split("\n");
            if(tmpqsl.size()==1){
                rsinfo+=selections[0];
            }
            else{
                printer->SetAlign(PrintCommand::ALIGN_LEFT,msg);
                for(i=0;i<tmpqsl.size();i++)
                    rsinfo+="  "+tmpqsl[i]+"\n";
            }
        }
        r=PrintGbkFromUtf8(printer,rsinfo.toStdString().c_str(),msg);

    }
    else if(GameID == 61 || GameID == 63 || GameID == 65 || GameID == 66)
    {
        r = printer->SetFontNormal(msg);

        r = printer->SetFontMagnify(1, 1, msg);

        r=printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

        for(i = 0; i < selections.size(); i++)
        {
            r = PrintGbkFromUtf8(printer,selections[i].toStdString().c_str(),msg);

        }
        r=PrintGbkFromUtf8Ln(printer,QString("\n").toStdString().c_str(),msg);
    }
    else if(GameID == 100000) //演示
    {
        r = PrintGbkFromUtf8(printer,"\n",msg);
        if (r)
            return r;
        r=printer->SetFont(0x00, 0x00, 0x00,msg);
        if (r)
            return r;
        r = printer->SetFontMagnify(2, 1, msg);
        if (r)
            return r;
        for(i = 0; i < selections.size(); i++)
        {
            r = PrintGbkFromUtf8(printer,selections[i].toStdString().c_str(),msg);
            if (r)
                return r;
        }
    }
    r=printer->SetAlign(PrintCommand::ALIGN_CENTER,msg);

    r=printer->SetFont(0x00, 0x00, 0x01,msg);

    r=printer->SetFontMagnify(1,1,msg);

    r = printer->MarkFeed(1, msg);
    if(GameID == 42 && _giftnum != "")
    {
        printer->SetFont(0x01, 0x00, 0x01,msg);
        printer->SetFontBold(msg);
        PrintGbkFromUtf8(printer,QString("乐善码 ").toStdString().c_str(),msg);
        printer->SetFont(0x01, 0x10, 0x01,msg);
        printer->SetFontMagnify(2,1,msg);
        PrintGbkFromUtf8Ln(printer, _giftnum.toStdString().c_str(),msg);
        printer->SetFont(0x00, 0x00, 0x01,msg);
        printer->SetFontNormal(msg);
        printer->SetFontMagnify(1,1,msg);
    }
    if(this->_rateflag)
    {
        PrintGbkFromUtf8Ln(printer, (_rate + "元").toStdString().c_str(),msg);
    }
    formatLine(ad);
    r=PrintGbkFromUtf8Ln(printer, ad.c_str(),msg);
    formatLine(shopAddress);


    r=PrintGbkFromUtf8Ln(printer, shopAddress.c_str(),msg);

    r = printer->MarkFeed(2, msg);

    r=printer->PrintBarCode(0,barCode,barlen,PrintCommand::PTR_BCS_PDF417, 0, 0, 0, 0, msg);
    r = printer->PageModePrint(PrintCommand::PTR_PM_PAGE_MODE, msg);
    r = printer->CutPaper(99, msg);



    return Err::SUCCEEDED;
}

int TicketPrintSupport::LoadLogo(ReceiptPrinter* printer, char* msg )
{
    int ret = 0;
    char *imageList[] = {"./logo/BJ11x5Image.bmp","./logo/CSLC_SUPERLOTTO.bmp","./logo/CSLC_FB6MHFHAD.bmp","./logo/CSLC_FB14MHAD.bmp",
                         "./logo/CSLC_FB14P9MHAD.bmp","./logo/CSLC_FB4MCRS.bmp","./logo/CSLC_HK.bmp","./logo/CSLC_NUM.bmp","./logo/CSLC_P3.bmp","./logo/CSLC_P5.bmp",
                         "./logo/BD_DC.bmp","./logo/BD_KZC.bmp"};
    ret = printer->LoadDiskLogo(12,imageList);
    if(ret)
    {
        return ret;
    }
    if(ret == 0)
    {
        for(int i = 0; i < 12; i++)
            ret = printer->PrintLoadLogo(0,0,i);
        if(ret)
        {
            printer->CutPaper(99, msg);
            return ret;
        }
    }

    printer->CutPaper(99, msg);

    return ret;
}
int TicketPrintSupport::PrintSample ( ReceiptPrinter* printer, char* msg )
{
    int ret = 0;
    demo = true;
    gameName = "演示<XXXXX>";
    ticketCode = "\nXXXXXXXXXXXXXXXXXXXXXXXX";
    password = "XXXXXX";
    drawNo = "00000";
    GameID = 100000;
    draws = 0;
    purchaseTime = "XXXXXXXXXXXXXXXXXXXXXX";
    multiple = 0;
    amount = 0;
    drawDate = "XXXXXXXXXXXX";
    selectionType = "XXXXXX";
    selections.push_back("① XX+XX+XX+XX+XX+XX+XX\n");
    selections.push_back("② XX+XX+XX+XX+XX+XX+XX\n");
    selections.push_back("③ XX+XX+XX+XX+XX+XX+XX\n");
    selections.push_back("④ XX+XX+XX+XX+XX+XX+XX\n");
    selections.push_back("⑤ XX+XX+XX+XX+XX+XX+XX\n");
    ad = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    terminalId = "99-999999-999";
    // shopId = "9999999";
    // shopAddress= "XXXXXXXXXXXXXXXXXXXX";
    barCode = "0000000-00000-000000-000000,xxxxxxxxxxxxx";
    barlen = sizeof(barCode);
    ret = this->Print(printer, msg);

    return ret;
}
