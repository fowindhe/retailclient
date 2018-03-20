#ifndef BALANCEVIEW_H
#define BALANCEVIEW_H
#include "TransactionView.h"
#include "CommitNumFrame.h"
class BalanceView: public TransactionView{
    Q_OBJECT
private:
    QLabel *_background;
    BaseButton *_returnButton;
    QLabel *_titleInfo;
    QLabel *_amountInfo;
    QLabel *_detailInfo;
    QLabel *_balanceInfo;
    QLabel *_MoneyLabel;


    BaseButton *_prePageButton;
    BaseButton *_nextPageButton;
    BaseButton *_clearCashButton;
    BaseButton *_clearBalanceButton;

    QLabel *_pageNoLabel;


    BaseButton  *_cashButton;
    QString             _cashStr;
    CommitNumFrame    *_cashNumFrame;


    virtual void Connect();
    virtual void Disconnect();
    virtual void OnShow(bool keepState);
public:
    BalanceView();
    ~BalanceView();
    void init();
    void Reset();
    virtual void Show();
private slots:
    void on_returnButton_clicked(int Val);
    void on_cashButton_clicked(int Val);
    void    on_Numbox_changed(int Id, int Val);

};

#endif // BALANCEVIEW_H
