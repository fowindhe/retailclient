#include "BalanceView.h"
BalanceView::BalanceView(){
    this->setWindowFlags(Qt::FramelessWindowHint);
    mytype=SLW_BalanceView;
}
BalanceView::~BalanceView(){


}

void BalanceView::init(){

    _background=new QLabel(this);
    QPixmap pix;
    QString filename=getenv("SLWHOME");
    filename+="/res/balancebg.jpg";
    pix.load(filename);
    _background->setPixmap(pix);
    _background->move(0,0);
    _background->lower();
    _titleInfo=new QLabel(this);
    _titleInfo->setText("合计：1张   销售票数：1张  兑奖票数：1张");
    _titleInfo->setFont(QFont("Timers",13,QFont::Bold));
    _titleInfo->setStyleSheet("color:white");
    _titleInfo->move(34,166);

    _amountInfo=new QLabel(this);
    _amountInfo->setText("应收：           元");
    _amountInfo->setFont(QFont("Timers",16,QFont::Bold));
    _amountInfo->setStyleSheet("color:white");
    _amountInfo->move(697,253);

    _detailInfo=new QLabel(this);
    _detailInfo->setText("代金券   ：1元\n体彩支付：1元\n当面付   ：1元\n\n现   金 ：                   元");
    _detailInfo->setFont(QFont("Timers",14,QFont::Bold));
    _detailInfo->setStyleSheet("color:white");
    _detailInfo->move(697,310);
    _balanceInfo=new QLabel(this);
    _balanceInfo->setText("找零：");
    _balanceInfo->setFont(QFont("Timers",15,QFont::Bold));
    _balanceInfo->setStyleSheet("color:white");

    _balanceInfo->move(741,489);
    _MoneyLabel=new QLabel(this);
    _MoneyLabel->setText("8.00");
    _MoneyLabel->setFont(QFont("Timers",19,QFont::Bold));
    _MoneyLabel->setStyleSheet("color:white");
    _MoneyLabel->move(766,250);



    _returnButton=new BaseButton(this);
    _returnButton->set_ButtonLabel_size(116,52);
    _returnButton->InitFast(":/res/cb_manage_return_up.png", ":/res/cb_manage_return_dn.png",NULL, NULL, NULL);
    _returnButton->set_ButtonFlag(Btn_Click);

    _returnButton->move(762,700);
    _returnButton->show();

    _prePageButton=new BaseButton(this);
    _prePageButton->set_ButtonLabel("上一页");
    _prePageButton->set_ButtonFlag(Btn_Click);
    _prePageButton->set_ButtonLabel_size(114,33);
    _prePageButton->InitFast(":/res/cb_mail_page_up.png",
                             ":/res/cb_mail_page_dn.png",
                             NULL, NULL, NULL);

    _prePageButton->move(189,555);

    _nextPageButton=new BaseButton(this);
    _nextPageButton->set_ButtonLabel("下一页");
    _nextPageButton->set_ButtonFlag(Btn_Click);
    _nextPageButton->set_ButtonLabel_size(114,33);
    _nextPageButton->InitFast(":/res/cb_mail_page_up.png",
                              ":/res/cb_mail_page_dn.png",
                              NULL, NULL, NULL);

    _nextPageButton->move(334,555);



    _clearCashButton=new BaseButton(this);
    _clearCashButton->set_ButtonLabel("清  除");
    _clearCashButton->set_ButtonFlag(Btn_Click);
    _clearCashButton->set_ButtonLabel_size(114,33);
    _clearCashButton->InitFast(":/res/cb_mail_page_up.png",
                               ":/res/cb_mail_page_dn.png",
                               NULL, NULL, NULL);

    _clearCashButton->move(711,534);


    _clearBalanceButton=new BaseButton(this);
    _clearBalanceButton->set_ButtonFlag(Btn_Click);
    _clearBalanceButton->set_ButtonLabel("结  算");
    _clearBalanceButton->set_ButtonLabel_size(114,33);
    _clearBalanceButton->InitFast(":/res/cb_mail_page_up.png",
                                  ":/res/cb_mail_page_dn.png",
                                  NULL, NULL, NULL);

    _clearBalanceButton->move(850,534);

    _prePageButton->show();
    _nextPageButton->show();
    _clearBalanceButton->show();
    _clearCashButton->show();

    _pageNoLabel=new QLabel(this);
    _pageNoLabel->setText("第1页/共1页");
    _pageNoLabel->setFont(QFont("Timers",12,QFont::Bold));
    _pageNoLabel->move(261,520);


    _cashButton = new BaseButton(this);
    _cashNumFrame = new CommitNumFrame(this);
    _cashButton ->set_ButtonFlag(Btn_Normal);
    _cashButton->upFont = "Microsoft YaHei";
    _cashButton->upFontSize = 15;
    _cashButton->upColor = Qt::black;
    _cashButton->dnFont = "Microsoft YaHei";
    _cashButton->dnFontSize = 15;
    _cashButton->dnColor = Qt::black;
    _cashButton->set_ButtonLabel_Right();
    //    _cashButton->setf
    //    _multipleButton->set_ButtonLabel_size(74, 25);
    _cashButton->set_ButtonLabel_size(151,31);
    _cashStr="0";
    _cashButton->set_ButtonLabel(_cashStr + ".00    ");
    _cashButton->InitFast(":/res/ba_cashbtn_up.png",
                          ":/res/ba_cashbtn_dn.png",
                          NULL, NULL, NULL);
    _cashButton->move(788, 409);
    _cashButton->show();
    _cashNumFrame->init();
    _cashNumFrame->move(788, 409+31);
    _cashNumFrame->Show();

    _ticketTitle=new QLabel(this);
    _ticketTitle->setText("玩法               交易类型              票数               金额");
    _ticketTitle->setFont(QFont("Timers",12,QFont::Bold));
    //    _ticketTitle->setStyleSheet("color:white");
    _ticketTitle->move(88,243);
    QStringList  name;
    name.append("玩法");
    name.append("票数");
    name.append("交易类型");
    name.append("金额");
    _treeView = new QTreeWidget(this);
    _treeView->setStyleSheet("QTreeView::branch {image:none;}");
    _treeView->setColumnCount(4);
    _treeView->setColumnWidth(0,150);
    _treeView->setColumnWidth(1,150);
    _treeView->setColumnWidth(2,150);
    _treeView->setColumnWidth(3,150);
    _treeView->setFont(FontFactory::MicrosoftYaHei(12));
    _treeView->setHeaderLabels(name);
    _treeView->setHeaderHidden(true);
    _treeView->setTextElideMode(Qt::ElideMiddle);
    _treeView->setFrameShape(QFrame::NoFrame);
    _treeView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _treeView->setSelectionMode(QAbstractItemView::NoSelection);
    _treeView->setFocusPolicy(Qt::NoFocus);
    _treeView->resize(616-11,458-264+50);
    _treeView->move(11,264);
    _treeView->show();
    tmpTickets.clear();
    for(int i=0;i<30;i++){

        tmpTickets.append("超级大乐透,1,销售,10");

    }

    //    _treeView=new QTreeView(this);
    //    _treeView->

}
void BalanceView::Connect(){
    TransactionView::Connect();
    connect(_returnButton,SIGNAL(clicked(int)),this,SLOT(on_returnButton_clicked(int)));
    connect(_cashButton, SIGNAL(clicked(int)), this, SLOT(on_cashButton_clicked(int)));
    connect(_cashNumFrame->NumBox, SIGNAL(ButtonBoxClicked(int,int)), this, SLOT(on_Numbox_changed(int,int)));
    connect(_clearCashButton, SIGNAL(clicked(int)), this, SLOT(on_clearCashButton_clicked(int)));
    connect(_clearBalanceButton, SIGNAL(clicked(int)), this, SLOT(on_clearBalanceButton_clicked(int)));
    connect(_prePageButton, SIGNAL(clicked(int)), this, SLOT(on_prePageButton_clicked(int)));
    connect(_nextPageButton, SIGNAL(clicked(int)), this, SLOT(on_nextPageButton_clicked(int)));



}

void BalanceView::Disconnect(){

    disconnect(_returnButton,0,0,0);
    disconnect(_cashButton, 0, 0, 0);
    disconnect(_prePageButton, 0, 0, 0);
    disconnect(_nextPageButton, 0, 0, 0);
    disconnect(_clearCashButton, 0, 0, 0);
    disconnect(_clearBalanceButton, 0, 0, 0);
    disconnect(_cashNumFrame->NumBox, 0, 0, 0);
    TransactionView::Disconnect();
}

void BalanceView::OnShow(bool keepState){
    Reset();
    SetAmount(0);
    Show();
    TransactionView::OnShow(true);
}

void BalanceView::Show(){
    SlwView::Show();
    this->resize(1024,768);
    this->show();
    _background->show();
    _returnButton->show();
    _titleInfo->show();
    _detailInfo->show();
    _MoneyLabel->show();
    _balanceInfo->show();
    _amountInfo->show();
    _pageNoLabel->show();
    _prePageButton->show();
    _nextPageButton->show();
    _clearBalanceButton->show();
    _clearCashButton->show();
    _cashButton->raise();
    _cashButton->show();
    _cashNumFrame->Hide();
    _ticketitems.clear();
    _pageNo=0;
    for(int i=0;i<tmpTickets.size();i++){
        BalanceTicket tmpBt;
        tmpBt.name=tmpTickets[i].split(",")[0];
        tmpBt.ticketCounts=tmpTickets[i].split(",")[1].toInt();
        tmpBt.dealtype=tmpTickets[i].split(",")[2];
        tmpBt.money=tmpTickets[i].split(",")[3].toInt();
        RP->_balanceTickets.append(tmpBt);
    }
    for(int i=0;i<RP->_balanceTickets.size();i++){
        if(i==10)
            break;
        QTreeWidgetItem *item=new QTreeWidgetItem(_treeView);
        item->setTextAlignment(0, Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        item->setText(0,RP->_balanceTickets[i].name);
        item->setTextAlignment(1, Qt::AlignHCenter|Qt::AlignVCenter|Qt::AlignCenter);
        item->setText(1,"    "+RP->_balanceTickets[i].dealtype);
        item->setTextAlignment(2, Qt::AlignRight|Qt::AlignRight|Qt::AlignRight);
        item->setText(2,QString::number(RP->_balanceTickets[i].ticketCounts)+"               ");
        item->setTextAlignment(3, Qt::AlignRight|Qt::AlignRight|Qt::AlignRight);
        item->setText(3,QString::number(RP->_balanceTickets[i].money)+"               ");
        if(i%2==1){
            item->setBackgroundColor(0,QColor(239, 245, 253));
            item->setBackgroundColor(1,QColor(239, 245, 253));
            item->setBackgroundColor(2,QColor(239, 245, 253));
            item->setBackgroundColor(3,QColor(239, 245, 253));
        }
        _ticketitems.append(item);

    }
    _treeView->addTopLevelItems(_ticketitems);
    _treeView->show();
    if(RP->_balanceTickets.size()%10==0)
        _pageNoLabel->setText(QString("第%1页/共%2页").arg(_pageNo+1).arg(RP->_balanceTickets.size()/10));
    else     _pageNoLabel->setText(QString("第%1页/共%2页").arg(_pageNo+1).arg(RP->_balanceTickets.size()/10)+1);

}
void BalanceView::Reset(){
    return;
}
void BalanceView::on_returnButton_clicked(int Val){
    GetController().Navigate(SLW_GAMEVIEW,&GetController(),RP);

    return;

}
void BalanceView::on_cashButton_clicked(int val){
    if(_cashButton->get_ButtonFlag() == Btn_Normal)
    {
        _cashStr="0";
        _cashNumFrame->raise();
        _cashNumFrame->Show();
    }
    else
    {
        if(_cashStr==""||_cashStr=="0"){
            _cashStr="0";

        }
        _cashNumFrame->lower();
        _cashNumFrame->Hide();
    }
    _cashButton->set_ButtonLabel(_cashStr+".00    ");
    _cashButton->SelectNumber(_cashButton->get_ButtonFlag());
    _cashButton->update();
    _cashButton->show();
    return;
}


void BalanceView::on_Numbox_changed(int Id, int Val)
{

    if(Val+1 == 10)
    {
        Val = -2;
    }
    else if(Val+1 == 11)
    {
        Val = -1;
    }
    else if(Val+1 == 12)
    {
        Val = -3;
    }
    if((Val + 1) == -1)
    {
        if(_cashStr.length() > 0)
        {
            _cashStr = _cashStr.mid(0, _cashStr.length() - 1);
        }
        if(_cashStr == ""||_cashStr=="0")
            _cashButton->set_ButtonLabel("0.00    ");
        else
            _cashButton->set_ButtonLabel(_cashStr + ".00    ");
    }
    else  if(Val+1 == -2)
    {
        _cashNumFrame->hide();
        _cashButton->set_ButtonFlag(Btn_Normal);
        _cashButton->update();
    }
    else
    {
        if(_cashStr == "" && Val == -1)
            return;
        if(_cashStr.length() < 8)
        {
            if(_cashStr=="0")
                _cashStr=QString::number(Val + 1);
            else _cashStr += QString::number(Val + 1);
        }
        _cashButton->set_ButtonLabel(_cashStr + ".00    ");
    }
    _cashButton->update();
    _cashButton->show();
    return;
}

void BalanceView::on_prePageButton_clicked(int Val){

}
void BalanceView::on_nextPageButton_clicked(int Val){


}
void BalanceView::on_clearBalanceButton_clicked(int Val){

}

void BalanceView::on_clearCashButton_clicked(int Val){
    _cashStr="0";
    _cashButton->set_ButtonLabel(_cashStr+".00    ");
    _cashButton->update();
    _cashButton->show();


}
