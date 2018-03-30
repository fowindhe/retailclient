#include "BankReportView.h"

BankReportView::BankReportView()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    mytype = SLW_BankReportView;
    _eventMask = 0;
}
BankReportView::~BankReportView()
{
}
void BankReportView::init()
{
    SlwView::init();
    _background = new QLabel(this);
    QPixmap pix;
    QString filename = getenv("SLWHOME");
    filename += "/res/IL_ReportShowView.jpg";
    pix.load(filename);

    _textName = new QLabel(this);
    _textName->setFont(FontFactory::MicrosoftYaHei(20));
    _textName->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);"));
    //    _textName->resize(200,32);
    _textName->setText("缴款查询");
    _textName->adjustSize();

    _background->setPixmap(pix);
    _background->move(0, 0);
    _background->lower();

    _textbeginDesc = new QLabel(this);
    _textbeginDesc->setFont(FontFactory::MicrosoftYaHei(15));
    _textbeginDesc->setStyleSheet("color: rgb(255, 255, 255);\n"
                                  "padding: 0px 0px -5px 0px;");
    _textbeginDesc->resize(100,32);
    _textbeginDesc->setText("起始日期：");

    _textbendDesc = new QLabel(this);
    _textbendDesc->setFont(FontFactory::MicrosoftYaHei(15));
    _textbendDesc->setStyleSheet("color: rgb(255, 255, 255);\n"
                                 "padding: 0px 0px -5px 0px;");
    _textbendDesc->resize(100,32);
    _textbendDesc->setText("截止日期：");

    _textbegin = new QLabel(this);
    _textbegin->resize(240,40);
    _textbegin->setFont(FontFactory::MicrosoftYaHei(20));
    _textbegin->setText("");
    _textbegin->setStyleSheet("QLabel {\n"
                              "	border:2px solid rgb(42, 89, 126);\n"
                              "	border-radius:10px;\n"
                              "	padding:4px 4px;\n"
                              "	background-color: rgb(255, 255, 255);\n"
                              "}\n");

    _textend = new QLabel(this);
    _textend->resize(240,40);
    _textend->setFont(FontFactory::MicrosoftYaHei(20));
    _textend->setText("");
    _textend->setStyleSheet("QLabel {\n"
                            "	border:2px solid rgb(42, 89, 126);\n"
                            "	border-radius:10px;\n"
                            "	padding:4px 4px;\n"
                            "	background-color: rgb(255, 255, 255);\n"
                            "}\n");

    _pageLabel = new QLabel(this);
    _pageLabel->setFont(FontFactory::MicrosoftYaHei(11));
    _pageLabel->setText("");
    _pageLabel->setStyleSheet("color:white");

    startButton=new BaseButton(this);
    startButton->set_ButtonLabel_size(37,37);
    startButton->InitFast(":/res/down_up",":/res/down_dn",":/res/down_up",":/res/down_up",":/res/down_up");
    startButton->set_ButtonFlag(Btn_Normal);

    endButton=new BaseButton(this);
    endButton->set_ButtonLabel_size(37,37);
    endButton->InitFast(":/res/down_up",":/res/down_dn",":/res/down_up",":/res/down_up",":/res/down_up");
    endButton->set_ButtonFlag(Btn_Normal);

    okButton=new BaseButton(this);
    okButton->set_ButtonLabel_size(114,33);
    okButton->InitFast(":/res/serachup",":/res/serachdn",":/res/serachdn",":/res/serachdn",":/res/serachdn");
    okButton->set_ButtonFlag(Btn_Click);
    okButton->set_ButtonLabel("查   询");

    backButton=new BaseButton(this);
    backButton->set_ButtonLabel_size(116,52);
    backButton->InitFast(":/res/Report_back_up",":/res/Report_back_dn",NULL,NULL,NULL);
    backButton->set_ButtonFlag(Btn_Click);

    prtButton=new BaseButton(this);
    prtButton->set_ButtonLabel_size(118,52);
    prtButton->InitFast(":/res/Report_prt_up",":/res/Report_prt_dn",":/res/Report_prt_nouse",NULL,NULL);
    prtButton->set_ButtonFlag(Btn_Click);

    nextButton = new BaseButton(this);
    nextButton->set_ButtonLabel_size(114,34);
    nextButton->InitFast(":/res/IL_btn_up.png",":/res/IL_btn_dn.png",NULL,NULL,NULL);
    nextButton->set_ButtonLabel("下一页");
    nextButton->set_ButtonFlag(Btn_Click);

    previousButton = new BaseButton(this);
    previousButton->set_ButtonLabel_size(114,34);
    previousButton->InitFast(":/res/IL_btn_up.png",":/res/IL_btn_dn.png",NULL,NULL,NULL);
    previousButton->set_ButtonLabel("上一页");
    previousButton->set_ButtonFlag(Btn_Click);

    calendar1 = new QCalendarWidget(this);
    calendar1->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar1->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    calendar1->setFont(FontFactory::MicrosoftYaHei(15));
    calendar1->adjustSize();
    calendar1->raise();

    calendar2 = new QCalendarWidget(this);
    calendar2->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar2->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    calendar2->setFont(FontFactory::MicrosoftYaHei(15));
    calendar2->adjustSize();
    calendar2->raise();

    infoEdit = new QTextEdit(this);
    infoEdit->setGeometry(QRect(19, 275, 980, 368));
    infoEdit->setReadOnly(true);
    infoEdit->setFrameShape(QFrame::NoFrame);
    infoEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    infoEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    infoEdit->setFont(FontFactory::ARPLKaitiMGB(20));
    infoEdit->setFocusPolicy(Qt::NoFocus);
    infoEdit->setAttribute(Qt::WA_Disabled);
    infoEdit->setAlignment(Qt::AlignCenter);

    _textName->move(120-(_textName->width()/2),85);
    _textbeginDesc->move(75,178);
    _textbendDesc->move(506,178);
    _textbegin->move(178,177);
    _textend->move(604,177);
    _pageLabel->move(390,665);
    okButton->move(844,179);
    startButton->move(394,179);
    endButton->move(790,179);
    prtButton->move(755,709);
    backButton->move(877,709);
    nextButton->move(880,663);
    previousButton->move(759,663);
    calendar1->move(163,218);
    calendar2->move(560,216);

    _textName->show();
    _textbeginDesc->show();
    _textbendDesc->show();
    _textbegin->show();
    _textend->show();
    _pageLabel->show();
    okButton->show();
    startButton->show();
    endButton->show();
    backButton->show();
    prtButton->show();
    nextButton->show();
    previousButton->show();
    calendar1->show();
    calendar2->show();
}

void BankReportView::Connect()
{
    SlwView::Connect();
    connect(backButton, SIGNAL(clicked(int)), this, SLOT(on_backButton_clicked(int)));
    //    connect(calendar1,SIGNAL(clicked(QDate)),this,SLOT(on_calendar1_clicked(QDate)));
    //    connect(calendar2,SIGNAL(clicked(QDate)),this,SLOT(on_calendar2_clicked(QDate)));
    connect(nextButton,SIGNAL(clicked(int)),this,SLOT(on_nextPage_clicked(int)));
    connect(previousButton,SIGNAL(clicked(int)),this,SLOT(on_perviousPage_clicked(int)));
    //    connect(startButton,SIGNAL(clicked(int)),this,SLOT(on_startButton_clicked(int)));
    //  connect(endButton,SIGNAL(clicked(int)),this,SLOT(on_endButton_clicked(int)));
    connect(prtButton,SIGNAL(clicked(int)),this,SLOT(on_prtButton_clicked(int)));
}
void BankReportView::Disconnect()
{
    SlwView::Disconnect();
    disconnect(backButton,0,0,0);
    //    disconnect(calendar1,0,0,0);
    //    disconnect(calendar2,0,0,0);
    disconnect(nextButton,0,0,0);
    disconnect(previousButton,0,0,0);
    //  disconnect(startButton,0,0,0);
    //   disconnect(endButton,0,0,0);
    disconnect(prtButton,0,0,0);
}
void BankReportView::OnShow(bool keepState)
{
    Reset();
    Show();
}
void BankReportView::Hide()
{
    SlwView::Hide();
    this->hide();
}
void BankReportView::Show()
{
    SlwView::Show();
    calendar1->hide();
    calendar2->hide();
    startButton->hide();
    endButton->hide();
    okButton->hide();
    nextButton->show();
    previousButton->show();
    _pageLabel->show();
    prtButton->set_ButtonFlag(Btn_NoUse);
    prtButton->show();
    this->show();
    on_okButton_clicked(0);
}
void BankReportView::Reset()
{
    queryflag = true;
    infoEdit->clear();
    _pageLabel->setText("");
    _pageno = 1;
    QDateTime now = QDateTime::currentDateTime();
    QDateTime begin = now.addSecs(-2678400);
    QDate tmpdate;
    QString _tmpdate;

    tmpdate = now.date();
    _tmpdate = now.toString("yyyy年MM月dd日");
    _end = tmpdate.toString("yyyyMMdd");
    _textend->setText(_tmpdate);

    tmpdate = begin.date();
    _tmpdate = begin.toString("yyyy年MM月dd日");
    _begin = begin.toString("yyyyMMdd");
    qDebug()<<"_begin:"<<_begin<<endl;
    _textbegin->setText(_tmpdate);
}
void BankReportView::query()
{
    _pageno = 1;
    CObject ret;
    ret["ReportType"] = INFOQUERY_PAYMENT;
    ret["ResultType"] = 0;
    ret["RequestTime"] = _end;
    ret["QueryStartDate"] = _begin;
    CObject replay = RP->_gameService->_lottoServ->getTextReport(ret,1);
 //   qDebug()<<"getTextReport:"<<replay;
    if(replay["ErrorCode"].toInt() != 0)
    {
        this->dialog->done(0);
        MessageBox dialog(*this);
        dialog.SetText(replay["ErrDesc"].toString());
        dialog.DoModal();
        _content = "";
        infoEdit->setText(_content);
        _pageLabel->hide();
        queryflag = false;
    }
    else
    {
        _content = replay["ReportContext"].toString();
        infoEdit->setText(_content);
        int _enternum = _content.count('\n');
        Pageno = _enternum/13;
        if(_enternum%13)
        {
            Pageno++;
        }
        _pageLabel->setText(QString("第%1页/共%2页").arg(_pageno).arg(Pageno));
        _pageLabel->adjustSize();
        _pageLabel->show();
        prtButton->set_ButtonFlag(Btn_Click);
        prtButton->show();
        this->dialog->done(0);
    }
    return;
}
int BankReportView::on_okButton_clicked(int Val)
{
    QPixmap pix;
    QString picName = getenv("SLWHOME");
    picName += "/res/dialog.png";
    pix.load(picName);
    dialog = new QDialog(this);
    dialog->resize(pix.width(), pix.height());
    dialog->setWindowFlags(Qt::FramelessWindowHint);
    QLabel *background;
    background = new QLabel(dialog);
    background->setPixmap(pix);
    background->move(0,0);
    background->show();
    background->lower();
    QLabel *_labelText;
    _labelText = new QLabel(dialog);
    _labelText->resize(280, 104);
    _labelText->move(60, 25);
    _labelText->setAlignment(Qt::AlignCenter);
    _labelText->setWordWrap(true);
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::black);
    _labelText->setPalette(pe);
    _labelText->setFont(FontFactory::MicrosoftYaHei(12));
    _labelText->setText("正在查询，请稍候...");
    _labelText->raise();
    dialog->move(307,284);
    QTimer::singleShot(0,this, SLOT(query()));
    return dialog->exec();
}
void BankReportView::on_backButton_clicked(int Val)
{
    GetController().Navigate( SLW_MainView, &GetController(), RP);
}
void BankReportView::on_startButton_clicked(int Val)
{
    //    ButtonType i = startButton->get_ButtonFlag();

    //    if(i == Btn_Normal)
    //    {
    //        startButton->set_ButtonFlag(Btn_Press);
    //        startButton->update();
    //        calendar1->show();
    //        calendar1->raise();
    //        ButtonType endButtonFlag = endButton->get_ButtonFlag();
    //        if (endButtonFlag == Btn_Press)
    //        {
    //            on_endButton_clicked(0);
    //        }
    //    }
    //    else if(i == Btn_Press)
    //    {
    //        startButton->set_ButtonFlag(Btn_Normal);
    //        startButton->update();
    //        calendar1->hide();
    //    }
    return;
}
void BankReportView::on_endButton_clicked(int Val)
{
    //    ButtonType i = endButton->get_ButtonFlag();

    //    if(i == Btn_Normal)
    //    {
    //        endButton->set_ButtonFlag(Btn_Press);
    //        endButton->update();
    //        calendar2->show();
    //        calendar2->raise();
    //        ButtonType startButtonFlag = startButton->get_ButtonFlag();
    //        if (startButtonFlag == Btn_Press)
    //        {
    //            on_startButton_clicked(0);
    //        }
    //    }
    //    else if(i == Btn_Press)
    //    {
    //        endButton->set_ButtonFlag(Btn_Normal);
    //        endButton->update();
    //        calendar2->hide();

    //    }
    return;
}
//void BankReportView::on_calendar1_clicked(QDate Val)
//{
//    QDate date = calendar1->selectedDate();
//    QString str_total;
//    str_total = date.toString("yyyy年MM月dd日");
//    _begin = date.toString("yyyy-MM-dd");
//    _textbegin->setText(str_total);
//    _textbegin->show();
//    _textbegin->adjustSize();
//}
//void BankReportView::on_calendar2_clicked(QDate Val)
//{
//    QDate date = calendar2->selectedDate();
//    QString str_total;
//    str_total = date.toString("yyyy年MM月dd日");
//    _end = date.toString("yyyy-MM-dd");
//    _textend->setText(str_total);
//    _textend->show();
//    _textend->adjustSize();
//}
void BankReportView::on_nextPage_clicked(int Val)
{
    if(!queryflag)
        return;
    QScrollBar *vbar = infoEdit->verticalScrollBar();
    vbar->triggerAction(QAbstractSlider::SliderPageStepAdd);
    _pageno++;
    if(_pageno > Pageno)
        _pageno = Pageno;
    _pageLabel->setText(QString("第%1页/共%2页").arg(_pageno).arg(Pageno));
    _pageLabel->adjustSize();
}
void BankReportView::on_perviousPage_clicked(int Val)
{
    if(!queryflag)
        return;
    QScrollBar *vbar = infoEdit->verticalScrollBar();
    vbar->triggerAction(QAbstractSlider::SliderPageStepSub);
    _pageno--;
    if(_pageno < 1)
        _pageno = 1;
    _pageLabel->setText(QString("第%1页/共%2页").arg(_pageno).arg(Pageno));
    _pageLabel->adjustSize();
}
void BankReportView::keyPressEvent(QKeyEvent *ev)
{
    SlwView::keyPressEvent(ev);
    switch(ev->key())
    {
    case Qt::Key_Up:
        on_perviousPage_clicked(0);
        break;
    case Qt::Key_Down:
        on_nextPage_clicked(0);
        break;
    default:
        break;
    }
}
void BankReportView::on_prtButton_clicked(int  Val)
{
    if(_content == "")
        return;
    int r;
    char * msg;
    r = RP->printer_->Open("",msg);
    if(r)
    {
        MessageBox dialog(*this);
        dialog.SetText(Dictionary::BDError[r]);
        dialog.DoModal();
        return;
    }
    ReportPrintSupport _printsupport;
    _printsupport._content = _content.toStdString();
    r = _printsupport.Print(RP->printer_,msg);
    if(r)
    {
        MessageBox dialog(*this);
        dialog.SetText(Dictionary::BDError[r]);
        dialog.DoModal();
        return;
    }
    prtButton->set_ButtonFlag(Btn_NoUse);
    return;
}
