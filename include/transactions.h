#pragma once
#include<vector>
#include<memory>
#include "currency.h"
#include "account.h"

class TransactionsHistory;
class Account;

class Transaction{
    const long transactionDate;
    const double transactionValue;
    const bool deposit;
    const bool transfer;
    TransactionsHistory *historic;
    std::weak_ptr<Account> usedAccount;

public:
    long getTransactionDate() const {return transactionDate;};
    double getValue() const {return transactionValue;};
    bool isDeposit() const {return deposit;};
    bool isTransfer() const {return transfer;};
    std::weak_ptr<Account> getUsedAccount() {return usedAccount;}
    void setAccount(std::shared_ptr<Account>& account) { usedAccount=account;}
    TransactionsHistory* getHistoric(){return historic;}
    void setHistoric(TransactionsHistory* istoricT){ historic=istoricT;}

    Transaction(TransactionsHistory* historic1, long date, double value, bool deposit1, bool transfer1, const std::shared_ptr<Account>& account1=nullptr);
    Transaction(const Transaction& t);
    friend std::ostream& operator<<(std::ostream& out, const Transaction& t);
};

class TransactionsHistory{
    double depositedMoney=0;
    double withdrawnMoney=0;
    double transferedMoneyReceived=0;
    double transferedMoneySent=0;

    Account *account;
    std::vector<std::unique_ptr<Transaction>> transactions;

public:
    int getTransactionsCount() const {return transactions.size();}
    double getDepositedMoney() const {return depositedMoney;}
    double getWithdrawnMoney()const {return withdrawnMoney;}
    double getTransferedMoneyReceived() const {return transferedMoneyReceived;}
    double getTransferedMoneySent() const {return transferedMoneySent;}
    double getTotalTransactionedMoney() const {return withdrawnMoney + depositedMoney + transferedMoneyReceived + transferedMoneySent;}
    std::vector<std::unique_ptr<Transaction>>& getTransactions(){return transactions;};
    Account* getAccount(){return account;}

    void logTransaction(double value, bool deposit, const std::shared_ptr<Account>& usedAccount = nullptr);

    TransactionsHistory(Account& c);
    ~TransactionsHistory();
    TransactionsHistory(const TransactionsHistory &istoric);

    friend std::ostream& operator<<(std::ostream& out, const TransactionsHistory& istoric);
};

std::ostream& operator<<(std::ostream& out, const Transaction& t);

std::ostream& operator<<(std::ostream& out, const TransactionsHistory& istoric);