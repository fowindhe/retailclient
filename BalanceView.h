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
    QTreeWidget *_treeView;
    QList<QTreeWidgetItem*> _ticketitems;
    QLabel *_ticketTitle;
    QStringList tmpTickets;
    int _pageNo;

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
    void on_Numbox_changed(int Id, int Val);
    void on_prePageButton_clicked(int Val);
    void on_nextPageButton_clicked(int Val);
    void on_clearCashButton_clicked(int Val);
    void on_clearBalanceButton_clicked(int Val);


};

#endif // BALANCEVIEW_H
