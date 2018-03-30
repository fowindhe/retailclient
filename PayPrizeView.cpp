#include "PayPrizeView.h"
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QScrollBar>
#include "Iconv.h"
#include "WaitAnsBox.h"

PayPrizeView::PayPrizeView()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    mytype = SLW_PayPrizeView;
    SetAmountTitle("派发奖金");
    _eventMask = 0;
    tsnlen = 0;
    tsnStr = "";
    tsnCode = "";
    passwordCode = "";
    showStr = "";
    jcPayCount = "";
    /*_value = 0;
    _max = 0;*/
}

PayPrizeView::~PayPrizeView()
{

}

//void PayPrizeView::Init()
//{
//    init();
//}

void PayPrizeView::init()
{
    //SlwView::init();
    _background = new QLabel(this);
    QPixmap pix;
    QString filename = getenv("SLWHOME");
    filename += "/res/prize.jpg";
    pix.load(filename);
    _background->setPixmap(pix);
    _background->move(0, 0);
    _background->lower();

    queryButton = new BaseButton(this);
    resetButton = new BaseButton(this);
    printButton = new BaseButton(this);
    returnButton = new BaseButton(this);
    payButton = new BaseButton(this);

    tsnClickButton = new BaseButton(this);
    passClickButton = new BaseButton(this);

    queryButton->set_ButtonLabel_size(114, 33);
    resetButton->set_ButtonLabel_size(114, 33);

    queryButton->upFont = "Microsoft YaHei";
    queryButton->upFontSize = 12;
    queryButton->dnFont = "Microsoft YaHei";
    queryButton->dnFontSize = 12;
    resetButton->upFont = "Microsoft YaHei";
    resetButton->upFontSize = 12;
    resetButton->dnFont = "Microsoft YaHei";
    resetButton->dnFontSize = 12;

    queryButton->InitFast(":/res/cb_prize_page_up.png",
                          ":/res/cb_prize_page_dn.png",
                          NULL, NULL, NULL);
    resetButton->InitFast(":/res/cb_prize_page_up.png",
                          ":/res/cb_prize_page_dn.png",
                          NULL, NULL, NULL);
    printButton->InitFast(":/res/cb_prize_print_up.png",
                          ":/res/cb_prize_print_dn.png",
                          ":/res/cb_prize_print_nouse.png",
                          NULL, NULL);
    returnButton->InitFast(":/res/cb_prize_return_up.png",
                           ":/res/cb_prize_return_dn.png",
                           NULL, NULL, NULL);
    payButton->InitFast(":/res/cb_prize_pay_up.png",
                        ":/res/cb_prize_pay_dn.png",
                        ":/res/cb_prize_pay_nouse.png", NULL, NULL);

    tsnClickButton->InitFast(":/res/cb_refund_click_up.png",
                             ":/res/cb_refund_click_dn.png",
                             NULL, NULL, NULL);
    passClickButton->InitFast(":/res/cb_refund_click_up.png",
                              ":/res/cb_refund_click_dn.png",
                              NULL, NULL, NULL);

    queryButton->set_ButtonFlag(Btn_Click);
    resetButton->set_ButtonFlag(Btn_Click);
    printButton->set_ButtonFlag(Btn_Click);
    returnButton->set_ButtonFlag(Btn_Click);
    payButton->set_ButtonFlag(Btn_Click);

    tsnClickButton->set_ButtonFlag(Btn_Normal);
    passClickButton->set_ButtonFlag(Btn_Normal);

    queryButton->set_ButtonLabel("查  询");
    resetButton->set_ButtonLabel("重新输入");

    queryButton->move(786, 172);
    resetButton->move(899, 172);
    printButton->move(643-130-130, 710);
    returnButton->move(772, 710);
    payButton->move(890, 710);
    tsnClickButton->move(441, 169);
    passClickButton->move(738, 169);

    queryButton->show();
    resetButton->show();
    printButton->show();
    returnButton->show();
    payButton->show();
    tsnClickButton->show();
    passClickButton->show();

    tsnFrame = new CommitNumFrame(this);
    passFrame = new CommitNumFrame(this);

    tsnFrame->init();
    passFrame->init();

    tsnFrame->move(312, 206);
    passFrame->move(609, 206);

    tsnFrame->Show();
    passFrame->Show();

    tsnLabel = new QLabel(this);
    passLabel = new QLabel(this);

    tsnLabel->setFont(FontFactory::MicrosoftYaHei(14));
    passLabel->setFont(FontFactory::MicrosoftYaHei(14));

    tsnLabel->resize(352, 39);
    passLabel->resize(160, 39);
    tsnLabel->move(90, 167);
    passLabel->move(570, 167);

    showLabel = new QLabel(this);
    showLabel->resize(987, 379);
    showLabel->setAlignment(Qt::AlignCenter);
    showLabel->setFont(FontFactory::MicrosoftYaHei(14));
    showLabel->setText(showStr);
    showLabel->move(14, 218);
    preButton = new BaseButton(this);
    preButton->InitFast(":/res/cb_jc_match_pre_up.png",
                        ":/res/cb_jc_match_pre_dn.png",
                        ":/res/cb_jc_match_pre_nouse.png",
                        NULL, NULL);
    preButton->set_ButtonFlag(Btn_Click);
    preButton->move(772-129-129-2,709);
    preButton->show();
    nextButton = new BaseButton(this);
    nextButton->InitFast(":/res/cb_jc_match_nex_up.png",
                         ":/res/cb_jc_match_next_dn.png",
                         ":/res/cb_jc_match_next_nouse.png",
                         NULL, NULL);
    nextButton->set_ButtonFlag(Btn_Click);
    nextButton->move(772-129, 709);
    nextButton->show();


    treeview = new QTreeWidget(this);
    treeview->setColumnCount(1);
    treeview->setColumnWidth(0,1000);
    treeview->setStyleSheet("QTreeView::branch {image:none;}");

    //treeview->setFont(FontFactory::MicrosoftYaHei(16));
    treeview->setFont(FontFactory::ARPLKaitiMGB(18));

    treeview->setHeaderHidden(true);
    treeview->setFrameShape(QFrame::NoFrame);
    treeview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    treeview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    treeview->setSelectionMode(QAbstractItemView::NoSelection);
    //    treeview->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeview->setFocusPolicy(Qt::NoFocus);
    treeview->resize(800,300);



    //    _table->setHorizontalHeaderLabels(header);
    disconnect(treeview, 0, 0, 0);
    //    _table->resize(995, 250);
    treeview->move(200, 250);
    treeview->show();

}

void PayPrizeView::Connect()
{
    //SlwView::Connect();
    TransactionView::Connect();
    connect(nextButton,SIGNAL(clicked(int)),this,SLOT(on_nextButton_clicked(int)));
    connect(preButton,SIGNAL(clicked(int)),this,SLOT(on_preButton_clicked(int)));
    connect(queryButton, SIGNAL(clicked(int)), this, SLOT(on_queryButton_clicked(int)));
    connect(resetButton, SIGNAL(clicked(int)), this, SLOT(on_resetButton_clicked(int)));
    connect(printButton, SIGNAL(clicked(int)), this, SLOT(on_printButton_clicked(int)));
    connect(returnButton, SIGNAL(clicked(int)), this, SLOT(on_returnButton_clicked(int)));
    connect(payButton, SIGNAL(clicked(int)), this, SLOT(on_payButton_clicked(int)));
    connect(tsnClickButton, SIGNAL(clicked(int)), this, SLOT(on_tsnButton_clicked(int)));
    connect(passClickButton, SIGNAL(clicked(int)), this, SLOT(on_passButton_clicked(int)));
    connect(tsnFrame->NumBox, SIGNAL(ButtonBoxClicked(int,int)), this, SLOT(on_tsn_num_changed(int,int)));
    connect(passFrame->NumBox, SIGNAL(ButtonBoxClicked(int,int)), this, SLOT(on_pass_num_changed(int,int)));
    //    connect(_table, SIGNAL(upClick()), this, SLOT(on_key_up_press()));
    //    connect(_table, SIGNAL(dnClick()), this, SLOT(on_key_dn_press()));
}

void PayPrizeView::Disconnect()
{
    //SlwView::Disconnect();
    disconnect(nextButton, 0, 0, 0);
    disconnect(preButton, 0, 0, 0);
    disconnect(queryButton, 0, 0, 0);
    disconnect(resetButton, 0, 0, 0);
    disconnect(printButton, 0, 0, 0);
    disconnect(returnButton, 0, 0, 0);
    disconnect(payButton, 0, 0, 0);
    disconnect(tsnClickButton, 0, 0, 0);
    disconnect(passClickButton, 0, 0, 0);
    disconnect(tsnFrame->NumBox, 0, 0, 0);
    disconnect(passFrame->NumBox, 0, 0, 0);
    TransactionView::Disconnect();
    //    disconnect(_table, 0, 0, 0);
}

void PayPrizeView::OnShow(bool keepState)
{
    Reset();
    bdShopId = "";
    bdPaidTime = "";
    SetAmount(0);
    Show();
    TransactionView::OnShow(true);
}

void PayPrizeView::Show()
{
    SlwView::Show();
    this->resize(1024, 768);
    this->show();
    treeview->clear();
    treeview->show();
    //_background->raise();
    _background->show();
    queryButton->raise();
    queryButton->show();
    resetButton->raise();
    resetButton->show();
    printButton->set_ButtonFlag(Btn_NoUse);
    printButton->update();
    printButton->raise();
    printButton->show();
    returnButton->raise();
    returnButton->show();
    payButton->set_ButtonFlag(Btn_NoUse);
    payButton->update();
    payButton->raise();
    payButton->show();
    tsnClickButton->raise();
    tsnClickButton->show();
    passClickButton->raise();
    passClickButton->show();
    tsnFrame->Hide();
    passFrame->Hide();
    tsnLabel->raise();
    tsnLabel->show();
    passLabel->raise();
    passLabel->show();
    showLabel->raise();
    showLabel->show();
    /*_table->raise();
    _table->show();*/
    if(RP->scanFlag == sc_pay)
    {
        tsnCode = QString::fromStdString(RP->_ticket._entry._ticketcode);
        passwordCode = QString::fromStdString(RP->_ticket._entry._ticketsafecode);
        tsnStr = tsnCode.mid(0, 6) + "-" + tsnCode.mid(6, 6) + "-" + tsnCode.mid(12, 6) + "-" + tsnCode.mid(18, -1);
        tsnlen = tsnCode.length();
        RP->scanFlag = sc_pay1;
        tsnLabel->setText(tsnStr);
        tsnLabel->show();
        passLabel->setText(passwordCode);
        passLabel->show();
        scan_thread();
    }
}

void PayPrizeView::scan_thread()
{
    cout << "-------scan start-------" << endl;
    if(RP->ThreadLock)
    {
        cout << "locked, wait for release" << endl;
        QTimer::singleShot(50, this, SLOT(scan_thread()));
    }
    else
    {
        RP->ThreadLock = true;
        on_queryButton_clicked(0);
        RP->ThreadLock = false;
        RP->GetFlag = true;
    }
}

void PayPrizeView::Reset()
{
    SetAmount(0);
    tsnlen = 0;
    tsnStr = "";
    tsnCode = "";
    passwordCode = "";
    showStr = "";
    tsnLabel->setText(tsnStr);
    passLabel->setText(passwordCode);
    showLabel->setText(showStr);
    tsnClickButton->set_ButtonFlag(Btn_Normal);
    tsnClickButton->update();
    passClickButton->set_ButtonFlag(Btn_Normal);
    passClickButton->update();

}

void PayPrizeView::on_queryButton_clicked(int Val)
{
    SetAmount(0);
    backflag = false;
    _support.supportList.clear();
    PayPrizeItem    pay_item;
    if(tsnClickButton->get_ButtonFlag() == Btn_Press)
    {
        on_tsnButton_clicked(0);
    }
    if(passClickButton->get_ButtonFlag() == Btn_Press)
    {
        on_passButton_clicked(0);
    }
    _amount = 0;
    showStr = "";
    //    145680649000000001 529398
    jcPayCount = "";

    QString msg="";
    payButton->set_ButtonFlag(Btn_NoUse);
    printButton->set_ButtonFlag(Btn_NoUse);
    //        tsnCode="000010147291649000000001";//11pick5
    //        passwordCode="033830";
    //    tsnCode="110117593400000501703406";
    //    passwordCode="610025";
    //         tsnCode="000077147697490871274640";    //胜负大奖票
    //        passwordCode="";
    if(tsnCode.length() == 20 && passwordCode.length() == 8)
    {
        if(!(RP->_global._systemState & SYS_HKJC_AVAIL))
        {
            MessageBox dialog(*this);
            dialog.SetText("竞彩系统不可用!");
            dialog.DoModal();
            payButton->set_ButtonFlag(Btn_NoUse);
            payButton->update();
            if(RP->scanFlag == sc_pay1)
            {
                RP->scanFlag = sc_endScan;
            }
            return;
        }
        if(!RP->AuthorizeGame(511))
        {
            MessageBox dialog(*this);
            dialog.SetText("没有竞彩玩法授权!");
            dialog.DoModal();
            payButton->set_ButtonFlag(Btn_NoUse);
            payButton->update();
            if(RP->scanFlag == sc_pay1)
            {
                RP->scanFlag = sc_endScan;
            }
            return;
        }
        FPPayEnquiryRequest_t _request;
        FPPayEnquiryReply_t _reply;
        _request.inputMethod = "0";
        _request.sameCustomer = "0";
        _request.tsn = tsnCode.toStdString();
        _request.ticketPassword = passwordCode.toStdString();
        CObject ret = RP->_service->_gameService->_hkjcServ->payEnquiry(_request, _reply);
        qDebug() << ret;
        if(ret["ErrorCode"].toInt() != Err::SUCCEEDED)
        {
            QString msg = ret["ErrDesc"].toString();
            MessageBox dialog(*this);
            dialog.SetText(msg);
            dialog.DoModal();
            payButton->set_ButtonFlag(Btn_NoUse);
            payButton->update();
            if(RP->scanFlag == sc_pay1)
            {
                RP->scanFlag = sc_endScan;
            }
            return;
        }
        else
        {
            if(_reply.tsn != tsnCode.toStdString())
            {
                MessageBox dialog(*this);
                dialog.SetText("票未找到");
                dialog.DoModal();
                payButton->set_ButtonFlag(Btn_NoUse);
                payButton->update();
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_endScan;
                }
            }
            else
            {
                showStr += "票号:" + QString::fromStdString(_reply.tsn) + "\n";
                showStr += "中奖注数:" + QString::number(QString::fromStdString(_reply.winCount).toInt()-QString::fromStdString(_reply.refundCount).toInt()) + "注*" + QString::fromStdString(_reply.multiplier) + "倍\n";
                showStr += "应退注数:" + QString::fromStdString(_reply.refundCount) + "注*" + QString::fromStdString(_reply.multiplier) + "倍\n";
                showStr += "中奖金额:" + QString::number(QString::fromStdString(_reply.payAmount).toDouble() - QString::fromStdString(_reply.refundAmount).toDouble(), 'f', 2) + "元\n";
                showStr += "应退金额:" + QString::fromStdString(_reply.refundAmount) + "元\n";
                showStr += "应付金额:" + QString::number(QString::fromStdString(_reply.payAmount).toDouble(), 'f', 2) + "元\n";
                showStr += "应付金额=中奖金额+应退金额\n";
                _amount = QString::fromStdString(_reply.payAmount).toDouble();
                if(_reply.payStatus == "0")
                {
                    _support.paidtime = _reply.payDateTime;
                    _support.paidTerminalId = _reply.payTerminalID;
                    showStr += "兑奖状态:已兑奖\n";
                    showStr += "兑付网点:" + QString::fromStdString(_reply.payTerminalID) + "\n";
                    showStr += "兑付时间:" + QString::fromStdString(_reply.payDateTime);
                    payButton->set_ButtonFlag(Btn_NoUse);
                    //                    printButton->set_ButtonFlag(Btn_Click);
                    if(RP->scanFlag == sc_pay1)
                    {
                        RP->scanFlag = sc_endScan;
                    }
                }
                else if(_reply.payStatus == "1")
                {
                    showStr += "兑奖状态:可兑奖";
                    payButton->set_ButtonFlag(Btn_Click);
                    jcPayCount = _reply.winCount;
                }
                char tmpbuf[2048];
                sprintf(tmpbuf,
                        "兑奖彩票：　　　　　%11s\n"
                        "中奖金额：　　　　　　　　　%12.2f\n"
                        "退票金额：　　　　　　　　　%12.2f\n"
                        "代扣税款：　　　　　　　　　%12.2f\n"
                        "合　　计：　　　　　　　　　%12.2f"
                        , tsnCode.toStdString().c_str()
                        , atof(_reply.payAmount.c_str())-atof(_reply.refundAmount.c_str())
                        , atof(_reply.refundAmount.c_str())
                        , atof(_reply.taxAmount.c_str())
                        , atof(_reply.payAmount.c_str())-atof(_reply.taxAmount.c_str())
                        );
                _support.showBuf = tmpbuf;
            }
        }
        payButton->update();
        printButton->update();
    }
    else if(tsnCode.length() == 24&&passwordCode.length() == 6){
        if(tsnCode.mid(0,6)=="000010")
        {
            backflag = true;
            CCPCITQueryPayReply_t reply;
            int ret=RP->_service->queryHFPay("10"+tsnCode.mid(6,tsnCode.size()-6),passwordCode,reply,msg);
            if(ret){
                MessageBox dialog(*this);
                dialog.SetText(msg);
                dialog.DoModal();
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_endScan;
                }
                payButton->set_ButtonFlag(Btn_NoUse);

            }
            else{
                Iconv iconv1("GBK", "UTF-8");
                string gbkStr = reply.queryPayInfo.acPayContent;
                string utfStr;
                iconv1.Convert(gbkStr, utfStr);
                QString replytmpstr = QString::fromStdString(utfStr);
                int index = replytmpstr.indexOf("兑付销售点");
                if(index != -1)
                {
                    showStr += replytmpstr.mid(0, index+6) + "(旧)" + replytmpstr.mid(index+6, -1);
                }
                else
                    showStr += replytmpstr;
                if(!reply.queryPayInfo.payTksStatus.isPaid&&(reply.queryPayInfo.usPrizeType==3||reply.queryPayInfo.usPrizeType==2)){
                    payButton->set_ButtonFlag(Btn_Click);
                }
            }

        }
        else {
            RP->_service->queryPay(tsnCode,passwordCode,payquery);
            if(payquery.errdesc_==""){
                showStr+=payquery.detail_;
                RP->payinfo.paidStatus_=payquery.status_;
                if(payquery.status_==1||payquery.status_==2||payquery.status_==3){
                    payButton->set_ButtonFlag(Btn_Click);
                }
            }
            else {
                RP->payinfo.paidStatus_=0;
                payButton->set_ButtonFlag(Btn_NoUse);
                MessageBox dialog(*this);
                dialog.SetText(payquery.errdesc_);
                dialog.DoModal();
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_endScan;
                }
            }

        }
    }
    else if(tsnCode.length() == 24&&passwordCode.length() == 0&&tsnCode.mid(0,1)=="0"&&tsnCode.mid(0,6)!="000010"){
        unsigned long code;
        string message;
        bool renewFlag;
        list<DrawWinVo> drawWinVoList;
        GameVo gameVo;
        TicketVo baseInfo;
        string numbers;
        string shopId;
        string paidTime;
        RP->_bdservice->GetWin(tsnCode,code,message,renewFlag,drawWinVoList,gameVo,baseInfo,numbers,bdShopId,bdPaidTime);

        if(code==0){
            if(renewFlag){
                MessageBox dialog(*this);
                dialog.SetText("存在未开奖奖期!");
                dialog.DoModal();
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_endScan;
                }
                return;
            }
            double tmpWinMount = 0;
            double tmpWithdrawedMount = 0;
            double tmpTaxMount = 0;
            bool paidFlag=false;
            _amount = 0;
            QString titelInfo="";
            bool isWon=false;
            list<DrawWinVo>::iterator winlsi;
            for(winlsi=drawWinVoList.begin();winlsi!=drawWinVoList.end();winlsi++){
                if(winlsi->winFlag==Paid){
                    titelInfo="兑奖状态:已兑奖\n";
                    isWon=true;
                    printButton->set_ButtonFlag(Btn_Click);
                }
                else if(winlsi->winFlag==Won){
                    titelInfo="恭喜中奖！\n";
                    isWon=true;
                    payButton->set_ButtonFlag(Btn_Click);
                }
                else if(winlsi->winFlag==PreDraw){
                    MessageBox dialog(*this);
                    dialog.SetText("不在兑奖期！");
                    dialog.DoModal();
                    if(RP->scanFlag == sc_pay1)
                    {
                        RP->scanFlag = sc_endScan;
                    }
                    return;
                }
                else if(winlsi->winFlag==Waiting){
                    MessageBox dialog(*this);
                    dialog.SetText("存在未开奖场次！");
                    dialog.DoModal();
                    if(RP->scanFlag == sc_pay1)
                    {
                        RP->scanFlag = sc_endScan;
                    }
                    return;
                }
                if(winlsi->winFlag==Paid||winlsi->winFlag==Won){
                    showStr += "-------------------------------------\n";
                    showStr+="奖期编号:"+QString::fromStdString(winlsi->drawNo)+"  奖期年:"+QString::fromStdString(winlsi->drawYear)+"\n";
                    pay_item.showBuf="奖期编号:"+winlsi->drawNo+"  奖期年:"+winlsi->drawYear;
                    _support.supportList.push_back(pay_item);
                    list<PrizeLevelVo>::iterator plvi;
                    for(plvi=winlsi->level.begin();plvi!=winlsi->level.end();plvi++){
                        string payBuf;
                        payBuf = "";
                        if(plvi->name=="赠送彩票"){
                            payBuf += plvi->name;
                            payBuf += "共"+plvi->amount + "元\n";
                            pay_item.showBuf = payBuf;
                            _support.supportList.push_back(pay_item);
                            showStr+=QString::fromStdString(plvi->name)+":"+QString::fromStdString(plvi->amount)+"元\n";
                            _amount += QString::fromStdString(plvi->amount).toDouble();
                            _amount += QString::fromStdString(plvi->withdrawed).toDouble();
                            _amount -= QString::fromStdString(plvi->tax).toDouble();
                            tmpWinMount += QString::fromStdString(plvi->amount).toDouble();
                            tmpWithdrawedMount += QString::fromStdString(plvi->withdrawed).toDouble();
                            tmpTaxMount += QString::fromStdString(plvi->tax).toDouble();

                        }
                        else{
                            payBuf += plvi->name;
                            payBuf += "共" + plvi->chances + "注  奖金:" + plvi->amount + "元\n税额:" + plvi->tax + "元 退票金额:" + plvi->withdrawed + "元\n";
                            pay_item.showBuf = payBuf;
                            _support.supportList.push_back(pay_item);
                            showStr+=QString::fromStdString(plvi->name)+":"
                                    +QString::fromStdString(plvi->chances)+"注"+
                                    "  中奖金额:"+QString::fromStdString(plvi->amount)+"元\n"+
                                    "退票金额:"+QString::fromStdString(plvi->withdrawed)+"元 "+
                                    "  代缴税款:"+QString::fromStdString(plvi->tax)+"元\n";
                            _amount += QString::fromStdString(plvi->amount).toDouble();
                            _amount += QString::fromStdString(plvi->withdrawed).toDouble();
                            _amount -= QString::fromStdString(plvi->tax).toDouble();
                            tmpWinMount += QString::fromStdString(plvi->amount).toDouble();
                            tmpWithdrawedMount += QString::fromStdString(plvi->withdrawed).toDouble();
                            tmpTaxMount += QString::fromStdString(plvi->tax).toDouble();
                        }
                    }
                }
                if(winlsi->winFlag==Paid)
                    paidFlag=true;
                else paidFlag=false;
            }
            showStr=titelInfo+showStr;
            if(!isWon){
                MessageBox dialog(*this);
                dialog.SetText("该张彩票未中奖！");
                dialog.DoModal();
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_endScan;
                }
                return;
            }
            char tmpbuf[2048];
            memset(tmpbuf,0,2048);
            sprintf(tmpbuf,
                    "兑奖彩票：　　　%11s\n"
                    "中奖金额：　　　　　　　　　%12.2f\n"
                    "退票金额：　　　　　　　　　%12.2f\n"
                    "代扣税款：　　　　　　　　　%12.2f\n"
                    "合　　计：　　　　　　　　　%12.2f"
                    , tsnCode.toStdString().c_str()
                    , tmpWinMount
                    , tmpWithdrawedMount
                    , tmpTaxMount
                    , tmpWinMount+tmpWithdrawedMount-tmpTaxMount
                    );
            bdPrintStr = tmpbuf;
            showStr += "-------------------------------------\n";
            showStr += QString::fromStdString(bdPrintStr) + "\n";
            cout<<drawWinVoList.end()->winFlag<<endl;
            if(paidFlag)
            {
                sprintf(tmpbuf,
                        "兑奖专卖店：　　　　　　　　　%s\n"
                        "兑奖时间：　　　　 　%s\n",
                        bdShopId.c_str(),
                        bdPaidTime.c_str());
                string ss;
                ss = tmpbuf;
                showStr += QString::fromStdString(ss);
            }
        }
        else{
            MessageBox dialog(*this);
            dialog.SetText(QString::fromStdString(message));
            dialog.DoModal();
            if(RP->scanFlag == sc_pay1)
            {
                RP->scanFlag = sc_endScan;
            }
            return;
            //            showStr+=QString::fromStdString(message);
        }
    }
    else
    {
        MessageBox dialog(*this);
        dialog.SetText("请输入正确的票号和票密码");
        dialog.DoModal();
        payButton->set_ButtonFlag(Btn_NoUse);
        if(RP->scanFlag == sc_pay1)
        {
            RP->scanFlag = sc_endScan;
        }
    }
    showStrls.clear();
    showStrls=showStr.split("\n");
    int i;

    QString firstshowstr="";
    treeview->clear();
    QTreeWidgetItem *item;
    for(i=0;i<showStrls.size();i++)
    {
        item = new QTreeWidgetItem(treeview);
        if(i==showStrls.size()-1)
            item->setText(0,showStrls[i]);
        else
        {
            if(backflag)
                item->setText(0,showStrls[i].left(showStrls[i].size() -1));
            else
                item->setText(0,showStrls[i].left(showStrls[i].size()));
        }
        treeview->insertTopLevelItem(i,item);

    }
    treeview->resize(800,310);
    treeview->show();
    treeview->scrollToTop();
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
    //    for(i=0;i<showStrls.size()&&i<pagesize;i++){
    //        qDebug()<<showStrls[i];
    //        firstshowstr+=showStrls[i]+"\n";
    //    }
    //    showLabel->setText(firstshowstr);
    //    showLabel->show();

    payButton->update();
    printButton->update();
    SetAmount(_amount);
}

void PayPrizeView::on_resetButton_clicked(int Val)
{
    if(tsnClickButton->get_ButtonFlag() == Btn_Press)
    {
        on_tsnButton_clicked(0);
    }
    if(passClickButton->get_ButtonFlag() == Btn_Press)
    {
        on_passButton_clicked(0);
    }
    tsnCode = "";
    tsnStr = "";
    passwordCode = "";
    tsnlen = 0;
    SetAmount(0);
    tsnLabel->setText("");
    passLabel->setText("");
    treeview->clear();
    treeview->show();
    tsnLabel->show();
    passLabel->show();
    payButton->set_ButtonFlag(Btn_NoUse);
    payButton->update();
    if(RP->scanFlag == sc_pay1)
    {
        RP->scanFlag = sc_endScan;
    }
}

void PayPrizeView::on_printButton_clicked(int Val)
{
    char *msg;

    if(tsnCode.length() == 24&&passwordCode.length() == 6){
        QString titelInfo;
        QString endInfo;
        titelInfo="兑奖彩票："+RP->InsertSpace((tsnCode.mid(0,6)+"-"+tsnCode.mid(6,6)+"-"+tsnCode.mid(12,6)+"-"+tsnCode.mid(18,6)),28)+"\n";
        titelInfo+="中奖金额："+RP->InsertSpace(QString::number(RP->payReply.Amount,'f',2),23) +"\n";
        endInfo="销售员号："+RP->_global._clerkNo+"\n";
        endInfo+="兑奖时间："+RP->payReply.payTime+"\n";
        endInfo+="兑奖地址："+RP->_global._shopAddress;

        if(tsnCode.mid(0,6)=="000010")
        {
            payprtSupport.systemType=HF;
        }
        else
        {
            payprtSupport.systemType=LOT;
            titelInfo+="退票金额："+RP->InsertSpace(QString::number(RP->payReply.RefundAmount,'f',2),23) +"\n";
        }
        titelInfo+="代扣税款："+RP->InsertSpace(QString::number(RP->payReply.TaxAmount,'f',2),23) +"\n";
        titelInfo+="合    计："+RP->InsertSpace(QString::number(RP->payReply.Amount+RP->payReply.RefundAmount-RP->payReply.TaxAmount,'f',2),23) +"\n";
        titelInfo+="终端编号："+RP->InsertSpace((RP->_global._terminalNo.mid(0,2)+"-"+
                                            RP->_global._terminalNo.mid(2,2)+"-"+
                                            RP->_global._terminalNo.mid(4,1)+"-"+
                                            RP->_global._terminalNo.mid(5,5)+"-"+
                                            RP->_global._terminalNo.mid(10,3)),28)+"\n";
        payprtSupport.supportList.clear();
        payprtSupport.showBuf=showStr.toStdString();
        payprtSupport.TitelInfo=titelInfo.toStdString();
        payprtSupport.EndInfo=endInfo.toStdString();
        int r;
        r=RP->printer_->Open("",msg);
        if(r)
        {
            MessageBox dialog(*this);
            dialog.SetText(Dictionary::BDError[r]);
            dialog.DoModal();
            return;
        }
        r = payprtSupport.Print(RP->printer_,msg);
        if(r)
        {
            MessageBox dialog(*this);
            dialog.SetText(Dictionary::BDError[r]);
            dialog.DoModal();
        }
    }
    else if(tsnCode.length() == 24&&passwordCode.length() == 0&&tsnCode.mid(0,1)=="0"&&tsnCode.mid(0,6)!="000010")
    {
        _support.systemType = BD;
        _support.showBuf = bdPrintStr;
        _support.shopId = RP->_global._shopId.toStdString();
        _support.shopName = RP->_global._shopAddress.toStdString();
        _support.paidtime = bdPaidTime;
        _support.paidshopid = bdShopId;
        //        RP->printer_->Open("",msg);
        int r;
        r=RP->printer_->Open("",msg);
        if(r)
        {
            MessageBox dialog(*this);
            dialog.SetText(Dictionary::BDError[r]);
            dialog.DoModal();
            return;
        }
        r = _support.Print(RP->printer_,msg);
        if(r)
        {
            MessageBox dialog(*this);
            dialog.SetText(Dictionary::BDError[r]);
            dialog.DoModal();
        }
    }
    else if(tsnCode.length() == 20 && passwordCode.length() == 8)
    {
        _support.systemType = JC;
        _support.shopName = RP->_global._shopAddress.toStdString();
        _support.paidTerminalId = (RP->_global._terminalNo.mid(0,2)+"-"+
                                   RP->_global._terminalNo.mid(2,2)+"-"+
                                   RP->_global._terminalNo.mid(4,1)+"-"+
                                   RP->_global._terminalNo.mid(5,5)+"-"+
                                   RP->_global._terminalNo.mid(10,3)).toStdString();
        //        RP->printer_->Open("",msg);
        int r;
        r=RP->printer_->Open("",msg);
        if(r)
        {
            MessageBox dialog(*this);
            dialog.SetText(Dictionary::BDError[r]);
            dialog.DoModal();
            return;
        }
        r = _support.Print(RP->printer_,msg);
        if(r)
        {
            MessageBox dialog(*this);
            dialog.SetText(Dictionary::BDError[r]);
            dialog.DoModal();
        }
    }
}

void PayPrizeView::on_returnButton_clicked(int Val)
{
    if(RP->scanFlag == sc_pay1)
    {
        RP->scanFlag = sc_endScan;
    }
    GetController().Navigate(SLW_GAMEVIEW, &GetController(), RP);
}

void PayPrizeView::on_payButton_clicked(int Val)
{
    QString msg;
    if(RP->GetTid(msg)){
        MessageBox dialog(*this);
        dialog.SetText(msg);
        dialog.DoModal();
        return;
    }
    QString tmptid=RP->_tid;
    if(tsnCode.length() == 20 && passwordCode.length() == 8)
    {
        if(RP->JCdelayRequest!=0&&RP->sellJcTimerCount<RP->JCdelayRequest)
        {
            MessageBox dialog(*this);
            dialog.SetText("竞彩售票、兑奖交易间隔时间过短，请稍侯重试！");
            dialog.DoModal();
            return;
        }
        Vlog("[Pay][Start][ticketCode:%s][password:%s][tid:%s]",tsnCode.toStdString().c_str(),passwordCode.toStdString().c_str(),tmptid.toStdString().c_str());
        if(jcPayCount == "")
            return;
        else
        {
            FPPayRequest_t _request;
            FPPayReply_t _reply;
            if(RP->_tid == "")
            {
                QString _msg;
                int r = RP->_service->GetTid(RP->_tid, _msg);
                if(r)
                {
                    MessageBox dialog(*this);
                    dialog.SetText(_msg);

                    if(RP->scanFlag == sc_pay1)
                    {
                        RP->scanFlag = sc_endScan;
                    }
                    payButton->set_ButtonFlag(Btn_NoUse);
                    payButton->update();

                    dialog.DoModal();
                    return;
                }
            }
            _request.tid = RP->_tid.toLongLong();
            _request.tsn = tsnCode.toStdString();
            _request.ticketPassword = passwordCode.toStdString();
            //            _request.termSerialNo_ = RP->_gameService->getTerminalTransNo();
            _request.inputMethod = "0";
            _request.sameCustomer = "0";
            _request.stakeCnt = atoi(jcPayCount.c_str());
            _request.ticketTypeCode = 3;
            CObject ret = RP->_service->_gameService->_hkjcServ->pay(_request, _reply);
            if(ret["ErrorCode"].toInt() != Err::SUCCEEDED)
            {
                RP->_tid = "";
                msg = ret["ErrDesc"].toString();
            }
            else
            {
                RP->_tid = ret["NextTid"].toString();
                RP->_service->commitTransaction(QString::number(_request.tid),msg,RP->_tid);
                RP->resetJcSellTimer=true;
                RP->DrawPayTagBMP(_amount);
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_startPayTag;
                }
                RP->_moneyCount -= _amount;
                RP->_payCount++;
                BalanceTicket curTic={"竟彩","兑奖",1,_amount};
                RP->_balanceTickets.append(curTic);
                Update();
                msg = "兑奖成功";
                //                printButton->set_ButtonFlag(Btn_Click);
                //                printButton->update();
                on_queryButton_clicked(0);
                Vlog("[Pay][Success][ticketCode:%s][password:%s][tid:%s][amount:%f][Paytime:%s]",tsnCode.toStdString().c_str(),passwordCode.toStdString().c_str(),tmptid.toStdString().c_str(),_amount,_support.paidtime.c_str());
                on_printButton_clicked(0);
            }
            qDebug() << "msg:" << msg;
            MessageBox dialog(*this);
            dialog.SetText(msg);

            if(RP->scanFlag == sc_pay1)
            {
                RP->scanFlag = sc_endScan;
            }
            payButton->set_ButtonFlag(Btn_NoUse);
            payButton->update();

            dialog.DoModal();
        }
    }
    else if(tsnCode.length() == 24&&passwordCode.length() == 6)
    {
        Vlog("[Pay][Start][ticketCode:%s][password:%s][tid:%s]",tsnCode.toStdString().c_str(),passwordCode.toStdString().c_str(),tmptid.toStdString().c_str());
        if(tsnCode.mid(0,6)=="000010")
        {
            if(RP->GetHFTid(msg))
            {
                MessageBox dialog(*this);
                dialog.SetText(msg);
                dialog.DoModal();
                return;
            }
            CCPCITPayV2Reply_t reply;
            int errCode;
            errCode=RP->_service->HFPay("10"+tsnCode.mid(6,tsnCode.size()-6),passwordCode,RP->_tid,QString::number(RP->_hftid),reply,msg);
            MessageBox dialog(*this);
            if(errCode==0)
            {
                dialog.SetText("兑奖成功！");
                RP->_service->commitHFTransaction(QString::number(RP->_hftid),msg);
                RP->payReply.Amount=reply.payInfo.ulPrizeAmount/100;
                _amount = RP->payReply.Amount;
                RP->payReply.TaxAmount=0;
                RP->payReply.RefundAmount=0;
                QDate tmpdate(reply.systemReply.systemTime.ucYear+1900,reply.systemReply.systemTime.ucMonth,reply.systemReply.systemTime.ucDay);
                QTime tmptime(reply.systemReply.systemTime.ucHour,reply.systemReply.systemTime.ucMinute,reply.systemReply.systemTime.ucSecond);
                QDateTime  tmpdt(tmpdate,tmptime);
                RP->payReply.payTime=tmpdt.toString("yyyy-MM-dd  hh:mm:ss");
                RP->_moneyCount -= _amount;
                BalanceTicket curTic={"11选5","兑奖",1,_amount};
                RP->_balanceTickets.append(curTic);
                Vlog("[Pay][Success][ticketCode:%s][password:%s][tid:%s][amount:%f][payTime:%s]",tsnCode.toStdString().c_str(),passwordCode.toStdString().c_str(),tmptid.toStdString().c_str(),_amount,RP->payReply.payTime.toStdString().c_str());
                RP->_payCount++;
                Update();
                RP->DrawPayTagBMP(_amount);
                SetAmount(_amount);
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_startPayTag;
                }
                printButton->set_ButtonFlag(Btn_Click);
                printButton->update();
                on_printButton_clicked(0);
            }
            else
            {
                dialog.SetText(msg);
            }

            if(RP->scanFlag == sc_pay1)
            {
                RP->scanFlag = sc_endScan;
            }
            payButton->set_ButtonFlag(Btn_NoUse);
            payButton->update();

            dialog.DoModal();

        }
        else
        {
            RP->payinfo.ticketNo_=tsnCode;
            RP->payinfo.ticketPwd_= passwordCode;
            RP->GetTid(msg);
            RP->payinfo.tid_ = RP->_tid.toLongLong();
            RP->payinfo.paymentTypeCode_=20;
            RP->payinfo.operationStyle_=20;
            //            RP->payinfo.termSerialNo_= RP->_gameService->getTerminalTransNo();
            RP->payReply.Amount=0;
            RP->payReply.RefundAmount=0;
            RP->payReply.TaxAmount=0;
            int r;
            RP->WriteTmpTicket(Lot_Pay_Start);
            WaitAnsBox ansBox(*this,RP);
            ansBox.boxFlag=Ans_Pay_Lot;
            r = ansBox.DoModal();
            RP->ClearTmpTicket();
            if(r==0)
            {
                RP->_service->commitTransaction(QString::number(RP->payinfo.tid_),msg,RP->_tid);
                _amount = RP->payReply.Amount-RP->payReply.TaxAmount;
                RP->_moneyCount -= _amount;
                BalanceTicket curTic={"乐透系统","兑奖",1,_amount};
                RP->_balanceTickets.append(curTic);
                if(RP->payReply.PrizeDetail != "")
                {
                    showStr=RP->payReply.PrizeDetail;
                }
                Vlog("[Pay][Success][ticketCode:%s][password:%s][tid:%s][amount:%f][payTime:%s]",tsnCode.toStdString().c_str(),passwordCode.toStdString().c_str(),tmptid.toStdString().c_str(),_amount,RP->payReply.payTime.toStdString().c_str());
                RP->_payCount++;
                Update();
                RP->DrawPayTagBMP(_amount);
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_startPayTag;
                }
                SetAmount(_amount);
                printButton->set_ButtonFlag(Btn_Click);
                printButton->update();
                on_printButton_clicked(0);

            }
            if(RP->scanFlag == sc_pay1)
            {
                RP->scanFlag = sc_endScan;
            }
            payButton->set_ButtonFlag(Btn_NoUse);
            payButton->update();


        }
    }
    else if(tsnCode.length() == 24&&passwordCode.length() == 0&&tsnCode.mid(0,1)=="0"&&tsnCode.mid(0,6)!="000010")
    {
        Vlog("[Pay][Start][ticketCode:%s][password:%s][tid:%s]",tsnCode.toStdString().c_str(),passwordCode.toStdString().c_str(),tmptid.toStdString().c_str());
        unsigned long code;
        string	msg;
        RP->payinfo.tid_=RP->_tid.toLongLong();
        RP->_bdservice->PayPrize(tsnCode,code,msg,RP->_tid);
        MessageBox dialog(*this);
        if(code)
        {
            if(code == 513609001)
            {
                dialog.SetText("兑奖或取消票交易成功！请回收彩票！\n提示：最新额度更新失败，请记录交易金额等信息，以便与中心核对账目。");
                RP->_moneyCount -= _amount;
                RP->_payCount++;
                BalanceTicket curTic={"北单系统","兑奖",1,_amount};
                RP->_balanceTickets.append(curTic);
                Update();
                RP->DrawPayTagBMP(_amount);
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_startPayTag;
                }
                printButton->set_ButtonFlag(Btn_Click);
                printButton->update();
                on_queryButton_clicked(0);
                on_printButton_clicked(0);
            }
            else
            {
                dialog.SetText(QString::fromStdString(msg));
                if(RP->scanFlag == sc_pay1)
                {
                    RP->scanFlag = sc_endScan;
                }
            }
        }
        else
        {
            QString bd_pay = "";
            QString errdesc;
            bd_pay += tsnCode + "\n";
            bd_pay += "兑奖成功\n请将已兑奖彩票回收";
            dialog.SetText(bd_pay);
            RP->_service->commitTransaction(QString::number(RP->payinfo.tid_),errdesc,RP->_tid);
            RP->_moneyCount -= _amount;
            BalanceTicket curTic={"北单系统","兑奖",1,_amount};
            RP->_balanceTickets.append(curTic);
            RP->_payCount++;
            Update();
            RP->DrawPayTagBMP(_amount);
            if(RP->scanFlag == sc_pay1)
            {
                RP->scanFlag = sc_startPayTag;
            }
            printButton->set_ButtonFlag(Btn_Click);
            printButton->update();
            on_queryButton_clicked(0);
            Vlog("[Pay][Success][ticketCode:%s][password:%s][tid:%s][amount:%f][Paytime:%s]",tsnCode.toStdString().c_str(),passwordCode.toStdString().c_str(),tmptid.toStdString().c_str(),_amount,bdPaidTime.c_str());
            on_printButton_clicked(0);
        }

        payButton->set_ButtonFlag(Btn_NoUse);
        payButton->update();

        dialog.DoModal();
    }
}
void PayPrizeView::on_tsnButton_clicked(int Val)
{
    if(tsnClickButton->get_ButtonFlag() == Btn_Normal)
    {
        if(passClickButton->get_ButtonFlag() == Btn_Press)
            on_passButton_clicked(Val);
        tsnFrame->raise();
        tsnFrame->Show();
    }
    else
    {
        tsnFrame->lower();
        tsnFrame->Hide();
    }
    tsnClickButton->SelectNumber(tsnClickButton->get_ButtonFlag());
    tsnClickButton->update();
}

void PayPrizeView::on_passButton_clicked(int Val)
{
    if(passClickButton->get_ButtonFlag() == Btn_Normal)
    {
        if(tsnClickButton->get_ButtonFlag() == Btn_Press)
            on_tsnButton_clicked(Val);
        passFrame->raise();
        passFrame->Show();
    }
    else
    {
        passFrame->lower();
        passFrame->Hide();
    }
    passClickButton->SelectNumber(passClickButton->get_ButtonFlag());
    passClickButton->update();
}

void PayPrizeView::on_tsn_num_changed(int Id, int Val)
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
        if(tsnlen > 0)
        {
            switch(tsnlen)
            {
            case 6:
            case 12:
            case 18:
                tsnStr = tsnStr.mid(0, tsnStr.length()-2);
                break;
            default:
                tsnStr = tsnStr.mid(0, tsnStr.length()-1);
                break;
            }
            tsnCode = tsnCode.mid(0, tsnCode.length()-1);
            tsnlen--;
            tsnLabel->setText(tsnStr);
            tsnLabel->show();
        }
    }
    else if((Val + 1) == -2)
    {
        on_tsnButton_clicked(0);
    }
    else
    {
        if(tsnlen < 24)
        {
            switch(tsnlen)
            {
            case 5:
            case 11:
            case 17:
                tsnStr += QString::number(Val+1) + "-";
                break;
            default:
                tsnStr += QString::number(Val+1);
            }
            tsnCode += QString::number(Val+1);
            tsnlen++;
            tsnLabel->setText(tsnStr);
            tsnLabel->show();
        }
    }
}

void PayPrizeView::on_pass_num_changed(int Id, int Val)
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
        if(passwordCode.length() > 0)
        {
            passwordCode = passwordCode.mid(0, passwordCode.length()-1);
            passLabel->setText(passwordCode);
            passLabel->show();
        }
    }
    else if((Val + 1) == -2)
    {
        on_passButton_clicked(0);
    }
    else
    {
        if(passwordCode.length() < 8)
        {
            passwordCode += QString::number(Val+1);
            passLabel->setText(passwordCode);
            passLabel->show();
        }
    }
}
void PayPrizeView::on_nextButton_clicked(int Val)
{
    nextButton->update();
    QScrollBar *vbar = treeview->verticalScrollBar();
    vbar->triggerAction(QAbstractSlider::SliderPageStepAdd);
}
void PayPrizeView::on_preButton_clicked(int Val)
{
    preButton->update();
    QScrollBar *vbar = treeview->verticalScrollBar();
    vbar->triggerAction(QAbstractSlider::SliderPageStepSub);
}

/*void PayPrizeView::on_key_up_press()
{
    if((_value - 20) < 0)
    {
        _value = 0;
    }
    else
    {
        _value -= 20;
    }
    _table->verticalScrollBar()->setValue(_value);
}

void PayPrizeView::on_key_dn_press()
{
    _value += 20;
    _table->verticalScrollBar()->setValue(_value);
}*/

/*void PayPrizeView::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key())
    {
    case Qt::Key_Up:
        if((_value - 1) < 0)
        {
            _value = 0;
        }
        else
        {
            _value -= 1;
        }
        _table->verticalScrollBar()->setValue(_value);
        break;
    case Qt::Key_Down:
        if((_value  + 2) < _max)
        {
            _value += 1;
        }
        _table->verticalScrollBar()->setValue(_value);
        break;
    }
}*/
