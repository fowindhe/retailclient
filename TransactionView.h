#ifndef TRANSACTIONVIEW_H
#define TRANSACTIONVIEW_H

#include "BaseView.h"
#include "SlwView.h"

class TransactionView : public SlwView
{
    Q_OBJECT
    const char*     _amountTitle;
    double  _amount;
    BaseButton  *_balanceButton;
    BaseButton  *_clearingButton;

private slots:
    void    on_balanceButton_clicked(int val);
    void    on_clearingButton_clicked(int val);
protected:
    void    SetAmountTitle(const char* amountTitle)
    {
        _amountTitle = amountTitle;
    }

    virtual void Init();

    void SetAmount(double amount)
    {
        _amount = amount;
        Update();
    }

    virtual void Connect();
    virtual void Disconnect();
    virtual void OnShow(bool keepState);

    void Update();

public:
    TransactionView(void);
    ~TransactionView(void);
};

#endif // TRANSACTIONVIEW_H
