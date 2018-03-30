#ifndef BANKREPORTVIEW_H
#define BANKREPORTVIEW_H
#include <QtGui>
#include "stdafx.h"
#include "BaseButton.h"
#include "SlwView.h"
#include "MessageBox.h"
#include "ReportPrintSupport.h"

class BankReportView : public SlwView
{
    Q_OBJECT
private:
    QLabel *_background ;
    QLabel * _textend;
    QLabel * _textbegin;
    QLabel * _pageLabel;
    QLabel * _textName;
    QLabel * _textbeginDesc;
    QLabel * _textbendDesc;
    BaseButton *okButton;
    BaseButton *prtButton;
    BaseButton *backButton;
    BaseButton *startButton;
    BaseButton *endButton;
    BaseButton * nextButton;
    BaseButton * previousButton;
    QCalendarWidget * calendar1;
    QCalendarWidget * calendar2;
    QString _begin;
    QString _end;
    QString _content;
    QTextEdit *infoEdit;
    QDialog *dialog;
    int _pageno;
    int Pageno;
    bool queryflag;
    QTimer  *_timer;

    virtual void  Reset();
    virtual void Connect();
    virtual void Disconnect();
    virtual void OnShow(bool keepState);
    virtual void init();
public:
    BankReportView();
    ~BankReportView();
    virtual void Hide();
    virtual void Show();
    void keyPressEvent(QKeyEvent *ev);
private slots:
    int on_okButton_clicked(int Val);
    void on_backButton_clicked(int Val);
    void on_startButton_clicked(int Val);
    void on_endButton_clicked(int Val);
    void on_nextPage_clicked(int Val);
    void on_perviousPage_clicked(int  Val);
    void on_prtButton_clicked(int  Val);
    void query();
};

#endif // BANKREPORTVIEW_H
