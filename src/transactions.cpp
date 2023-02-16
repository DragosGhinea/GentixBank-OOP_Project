#include<iostream>
#include<ctime>
#include<chrono>
#include "../include/transactions.h"
using namespace std;

//https://stackoverflow.com/questions/1692184/converting-epoch-time-to-real-date-time
void display_transaction_date(long temp) {
  const time_t old = (time_t)temp;
  struct tm *oldt = gmtime(&old);
  cout<<asctime(oldt);
}

Transaction::Transaction(const Transaction& t) : transactionDate(t.transactionDate), transactionValue(t.transactionValue), deposit(t.deposit), transfer(t.transfer), historic(t.historic){
    usedAccount=t.usedAccount;
}

Transaction::Transaction(TransactionsHistory* historic1, long date, double value, bool deposit1, bool transfer1, const shared_ptr<Account>& account1) : transactionDate(date), transactionValue(value), deposit(deposit1), transfer(transfer1), historic(historic1){
    usedAccount=account1;
}

ostream& operator<<(ostream& out, const Transaction& t){
    out<<"────────────────┤ Transaction Info ├────────────────\n";
    out<<" • Date: ";
    display_transaction_date(t.transactionDate);

    out << " • " << t.historic->getAccount()->getCurrency()->fromUnit(t.transactionValue) << " " << t.historic->getAccount()->getCurrency()->getName();
    if(t.usedAccount.expired()){
        if(t.transfer){
            if (t.deposit)
                out << " received from [Deleted]." << endl;
            else
                out << " sent to [Deleted]." << endl;
        }
        else {
            if (t.deposit)
                out << " deposited." << endl;
            else
                out << " withdrawn." << endl;
        }
    }
    else{
        shared_ptr<Account> usedAccount = t.usedAccount.lock();
        if(t.deposit)
            out << " received from " << usedAccount->getOwner()->getLastName() << " " << usedAccount->getOwner()->getFirstName() << endl;
        else
            out << " sent to " << usedAccount->getOwner()->getLastName() << " " << usedAccount->getOwner()->getFirstName() << endl;
    }
    out<<"─────────────────────────────────────────────────\n\n";
    return out;
}

///Initialization TransactionsHistory

void TransactionsHistory::logTransaction(double value, bool deposit, const shared_ptr<Account>& usedAccount){
    if(deposit){
        if(usedAccount == nullptr)
            depositedMoney+=value;
        else
            transferedMoneyReceived+=value;
    }
    else{
        if(usedAccount == nullptr)
            withdrawnMoney+=value;
        else
            transferedMoneySent+=value;
    }
    if(transactions.size() == 100){
        transactions.erase(transactions.begin());
        transactions.push_back(make_unique<Transaction>(this, time(NULL), value, deposit, usedAccount!=nullptr, usedAccount));
    }
    else{
        transactions.push_back(make_unique<Transaction>(this, time(NULL), value, deposit, usedAccount!=nullptr, usedAccount));
    }
}

ostream& operator<<(ostream& out, const TransactionsHistory& istoric){
    out<<"────────────────┤ Transactions History ├────────────────\n";
    out << " • Total Deposited Money: " << istoric.account->getCurrency()->fromUnit(istoric.depositedMoney) << " " << istoric.account->getCurrency()->getName() << endl;
    out << " • Total Withdrawn Money: " << istoric.account->getCurrency()->fromUnit(istoric.withdrawnMoney) << " " << istoric.account->getCurrency()->getName() << endl;
    out << " • Total Received Money: " << istoric.account->getCurrency()->fromUnit(istoric.transferedMoneyReceived) << " " << istoric.account->getCurrency()->getName() << endl;
    out << " • Total Sent Money: " << istoric.account->getCurrency()->fromUnit(istoric.transferedMoneySent) << " " << istoric.account->getCurrency()->getName() << endl;
    out<<endl;
    out << " • Transactions Count: " << istoric.transactions.size() << endl;

    char afisare[20];
    out<<" • Display all transactions? (Yes/no): ";
    try{
        cin.getline(afisare, 20, '\n');
        if(afisare[0]!='n'){
            cout<<"\n\n";
            for(int i=0;i<istoric.transactions.size(); i++)
                out << *istoric.transactions[i] << endl;
        }
    }catch(...){

    }
    return out;
}

TransactionsHistory::TransactionsHistory(const TransactionsHistory &istoric){
    depositedMoney=istoric.depositedMoney;
    withdrawnMoney=istoric.withdrawnMoney;
    transferedMoneyReceived=istoric.transferedMoneyReceived;
    transferedMoneySent=istoric.transferedMoneySent;
    account=istoric.account;

    /*transactions.size()=historic.transactions.size();

    transactions=new Transaction*[100];
    for(int i=0;i<transactions.size();i++){
        transactions[i]=new Transaction(*historic.transactions[i]);
        transactions[i]->setHistoric(this);
    }
    for(int i=transactions.size();i<100;i++)
        transactions[i]=NULL;*/
    for(auto& transaction : transactions)
        transactions.push_back(make_unique<Transaction>(*transaction));
}

TransactionsHistory::TransactionsHistory(Account& c) : account(&c){}

TransactionsHistory::~TransactionsHistory(){
    /*for(int i=0;i<100;i++){
        if(transactions[i]==NULL)
            break;
        delete transactions[i];
    }
    delete[] transactions;*/
}