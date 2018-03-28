#include "TransactionView.h"
#include "logmanage.h"

static char amountTitle_[] = "金    额";

TransactionView::TransactionView(void) : _amountTitle(amountTitle_)
{
    _amount = 0;
}

TransactionView::~TransactionView(void)
{}

void TransactionView::Init()
{
    SlwView::init();

    _balanceButton = new BaseButton(this);
    _balanceButton->move(1000-329, 9);
    _balanceButton->set_ButtonLabel_size(511, 41, 6, 10);
    _balanceButton->upFont = "AR PL KaitiM GB";
    _balanceButton->upFontSize = 12;
    _balanceButton->dnFont = "AR PL KaitiM GB";
    _balanceButton->dnFontSize = 12;
    _balanceButton->resize(329, 40);

    _balanceButton->upColor = Qt::yellow;
    _balanceButton->dnColor = Qt::white;
    _balanceButton->InitFast(":/res/clear_balance_btn_up.png",
                            ":/res/clear_balance_btn_dn.png",
                            ":/res/clear_balance_btn_up.xpm", NULL, NULL);
    _balanceButton->set_ButtonFlag(Btn_Click);
    _balanceButton->raise();
    _balanceButton->show();

    _clearingButton = new BaseButton(this);
    _clearingButton->move(1000-329-64, 9);
    _clearingButton->set_ButtonLabel_size(115, 52, 6, 10);
    _clearingButton->upFont = "AR PL KaitiM GB";
    _clearingButton->upFontSize = 12;
    _clearingButton->dnFont = "AR PL KaitiM GB";
    _clearingButton->dnFontSize = 12;
    _clearingButton->resize(64, 40);

    _clearingButton->upColor = Qt::yellow;
    _clearingButton->dnColor = Qt::white;
    _clearingButton->InitFast(":/res/balance_btn_up.png",
                            ":/res/balance_btn_dn.png",
                            ":/res/balance_btn_dn.png", NULL, NULL);
    _clearingButton->set_ButtonFlag(Btn_Click);
    _clearingButton->raise();
    _clearingButton->show();

    init();
}

void TransactionView::Connect()
{
    SlwView::Connect();
    connect(_balanceButton, SIGNAL(clicked(int)), this, SLOT(on_balanceButton_clicked(int)));
    connect(_clearingButton,SIGNAL(clicked(int)),this,SLOT(on_clearingButton_clicked(int)));

}

void TransactionView::Disconnect()
{
    SlwView::Disconnect();
    disconnect(_balanceButton, 0, 0, 0);
    disconnect(_clearingButton,0,0,0);
}

void TransactionView::on_balanceButton_clicked(int val)
{
    Vlog("[SumButtonClicked][合计：%.2f][销售票数：%d][兑奖票数：%d]", RP->_moneyCount, RP->_ticketCount, RP->_payCount);
    RP->_moneyCount = 0;
    RP->_ticketCount = 0;
    RP->_payCount = 0;
    RP->_voucerMoneyCount = 0;
    RP->_JcVouerMoneyCount=0;
    RP->_EPayMoneyCount=0;
    RP->_balanceTickets.clear();
    Update();
    if(GetController().GetCurrentViewType()==SLW_BalanceView)
    {
        GetController().Navigate(SLW_BalanceView,&GetController(),RP);

    }
}

void TransactionView::on_clearingButton_clicked(int val)
{
    if(GetController().GetCurrentViewType()==SLW_BalanceView)
    {
       return;
    }
    else GetController().Navigate(SLW_BalanceView,&GetController(),RP);
}

void TransactionView::OnShow ( bool keepState )
{
    if (!keepState)
    {
        _amount = 0;
    }
    Update();
}

void TransactionView::Update()
{
    SlwView::Show();
    char buf[128];
    char buf1[128];
    if(RP->_curViewType == SLW_GAMEVIEW||RP->_curViewType==SLW_BalanceView)
    {
        _balanceButton->upFontSize = 15;
        _balanceButton->dnFontSize = 15;
        snprintf(buf, sizeof(buf), "应收:%8.2f ", (double)RP->_moneyCount);
        sprintf(RP->countBuf,"%8.2f",_amount);
    }
    else
    {
        _balanceButton->upFontSize = 12;
        _balanceButton->dnFontSize = 12;
        if(_amount == -1)
        {
            snprintf(buf, sizeof(buf), "%s:超过上限 应收:%8.2f ", _amountTitle, (double)RP->_moneyCount);
            sprintf(RP->countBuf,"    ----");
        }
        else if(_amount == -2)
        {
            snprintf(buf, sizeof(buf), "%s:场次过多 应收:%8.2f ", _amountTitle, (double)RP->_moneyCount);
            sprintf(RP->countBuf,"    ----");
        }
        else if(_amount < 0)
        {
            snprintf(buf, sizeof(buf), "%s:超过上限 应收:%8.2f ", _amountTitle,(double)RP->_moneyCount);
            sprintf(RP->countBuf,"    ----");
        }
        else
        {
            snprintf(buf, sizeof(buf), "%s:%8.2f 应收:%8.2f ", _amountTitle, _amount, (double)RP->_moneyCount);
            sprintf(RP->countBuf,"%8.2f",_amount);
        }
    }
    snprintf(buf1, sizeof(buf1), "%8.2f\n%8.2f",_amount, (double)RP->_moneyCount);

    sprintf(RP->sumBuf,"%8.2f",(double)RP->_moneyCount);
    _balanceButton->set_ButtonLabel_Left();
    _balanceButton->set_ButtonLabel(buf);
    //_balanceButton->set_ButtonLabel("");
    _balanceButton->raise();
    _balanceButton->show();
    this->show();
}
