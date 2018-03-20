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
    filename+="/res/refund.jpg";
    pix.load(filename);
    _background->setPixmap(pix);
    _background->move(0,0);
    _background->lower();
    _titleInfo=new QLabel(this);
    _titleInfo->setText("合计：1张   销售票数：1张  兑奖票数：1张");
    _titleInfo->setFont(QFont("Timers",12,QFont::Bold));
    _titleInfo->setStyleSheet("color:white");
    _titleInfo->move(34,156);
    _amountInfo=new QLabel(this);
    _amountInfo->setText("应收：           元");
    _amountInfo->move(697,253);
    _amountInfo->setFont(QFont("Timers",16,QFont::Bold));
    _detailInfo=new QLabel(this);
    _detailInfo->setText("代金券：1元\n体彩支付：1元\n当面付：1元\n现金：        元");
    _detailInfo->setFont(QFont("Timers",12,QFont::Bold));
    _detailInfo->move(697,288);
    _balanceInfo=new QLabel(this);
    _balanceInfo->setText("找零：");
    _balanceInfo->setFont(QFont("Timers",16,QFont::Bold));
    _balanceInfo->move(741,489);
    _MoneyLabel=new QLabel(this);
    _MoneyLabel->setText("8.00");
    _MoneyLabel->setFont(QFont("Timers",20,QFont::Bold));

    _MoneyLabel->move(766,250);
    _returnButton=new BaseButton(this);
    _returnButton->InitFast(":/res/cb_refund_return_up.png",
                            ":/res/cb_refund_return_dn.png",
                            NULL, NULL, NULL);
    _returnButton->move(762,710);

    _prePageButton=new BaseButton(this);
    _prePageButton->set_ButtonLabel("上一页");
    _prePageButton->set_ButtonLabel_size(114,33);
    _prePageButton->InitFast(":/res/cb_mail_page_up.png",
                             ":/res/cb_mail_page_dn.png",
                             NULL, NULL, NULL);

    _prePageButton->move(189,534);

    _nextPageButton=new BaseButton(this);
    _nextPageButton->set_ButtonLabel("下一页");
    _nextPageButton->set_ButtonLabel_size(114,33);
    _nextPageButton->InitFast(":/res/cb_mail_page_up.png",
                              ":/res/cb_mail_page_dn.png",
                              NULL, NULL, NULL);

    _nextPageButton->move(334,534);



    _clearCashButton=new BaseButton(this);
    _clearCashButton->set_ButtonLabel("清  除");
    _clearCashButton->set_ButtonLabel_size(114,33);
    _clearCashButton->InitFast(":/res/cb_mail_page_up.png",
                               ":/res/cb_mail_page_dn.png",
                               NULL, NULL, NULL);

    _clearCashButton->move(711,534);


    _clearBalanceButton=new BaseButton(this);
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
    _pageNoLabel->move(261,490);


    _cashButton = new BaseButton(this);
    _cashNumFrame = new CommitNumFrame(this);
    _cashButton ->set_ButtonFlag(Btn_Normal);
    _cashButton->upFont = "Microsoft YaHei";
    _cashButton->upFontSize = 11;
    _cashButton->upColor = Qt::black;
    _cashButton->dnFont = "Microsoft YaHei";
    _cashButton->dnFontSize = 11;
    _cashButton->dnColor = Qt::black;
    //    _multipleButton->set_ButtonLabel_size(74, 25);
    _cashButton->set_ButtonLabel_size(97,65);
    _cashStr="1";
    _cashButton->set_ButtonLabel(_cashStr + "元 ");
    _cashButton->InitFast(":/res/rect_mul_up.png",
                          ":/res/rect_mul_dn.png",
                          NULL, NULL, NULL);
    _cashButton->move(798, 429);
    _cashButton->show();
    _cashNumFrame->init();
    _cashNumFrame->move(798, 429+65);
    _cashNumFrame->Show();

}
void BalanceView::Connect(){
    TransactionView::Connect();
    connect(_returnButton,SIGNAL(clicked(int)),this,SLOT(on_returnButton_clicked(int)));
    connect(_cashButton, SIGNAL(clicked(int)), this, SLOT(on_cashButton_clicked(int)));
    connect(_cashNumFrame->NumBox, SIGNAL(ButtonBoxClicked(int,int)), this, SLOT(on_Numbox_changed(int,int)));

}

void BalanceView::Disconnect(){

    disconnect(_returnButton,0,0,0);
    disconnect(_cashButton, 0, 0, 0);
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
        _cashStr="";
        _cashButton->set_ButtonLabel(_cashStr + " 元 ");
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

    //    _multipleButton->set_ButtonLabel(_multipleStr + "倍");
    _cashButton->set_ButtonLabel(_cashStr + "元 ");
    _cashButton->SelectNumber(_cashButton->get_ButtonFlag());
    _cashButton->update();
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
        if(_cashStr == "")
            _cashButton->set_ButtonLabel("0.00");
        else
            _cashButton->set_ButtonLabel(_cashStr + ".00");
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
            _cashStr += QString::number(Val + 1);
        }
        _cashButton->set_ButtonLabel(_cashStr + ".00");
    }
    _cashButton->update();
    _cashButton->show();
    return;
}
