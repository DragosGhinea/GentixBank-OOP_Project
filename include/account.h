#pragma once
#include "transactions.h"
#include "card.h"
#include "user.h"

class User;
class TransactionsHistory;
class AccountBuilder;

class Account : public std::enable_shared_from_this<Account>{
    const int accountId;
    double balance=0;

    User *owner;

    Currency *usedCurrency;

    std::unique_ptr<TransactionsHistory> historic;

    std::vector<std::unique_ptr<Card>> cards;

    static int generateId(){
        static int id=0;
        id++;
        return id;
    }

    Account() : accountId(generateId()), owner(NULL), usedCurrency(NULL){};
public:
    friend class AccountBuilder;

    double getAccountId(){return accountId;};
    double getBalance(){return balance;};
    Currency* getCurrency(){return usedCurrency;}
    void changeCurrency(Currency* currency){ usedCurrency=currency;}
    std::vector<std::unique_ptr<Card>>& getCards(){return cards;}
    int getNumberOfCards() const {return cards.size();}
    User* getOwner(){return owner;}
    TransactionsHistory* getTransactionsHistory(){return &(*historic);}

    bool bankTransfer(double value, std::shared_ptr<Account> &account, Currency* currency = NULL);
    bool bankTransfer(double value, Card &card, Currency* currency = NULL);
    void setBalance(double value){ balance=value;}

    Account(User &u, Currency* currency = NULL);
    ~Account();
    Account(const Account &c);
    Account& operator=(const Account& c);

    Card* getCard(int cardId);
    bool addCard(std::string tip= "Virtual");
    bool addCard(Card* card);
    bool deleteCard(int cardNumber);
    void detachCard(Card* card);

    operator double() {return balance;}
    Account& operator+(const Account& c);
    Account& operator+(double value);
    Account& operator-(double value);
    Account& operator+=(double value);
    Account& operator-=(double value);

    virtual int getMaxCards() const {
        return 5;
    };

    virtual std::string getType() const {
        return "Personal";
    };

    friend std::ostream& operator<<(std::ostream& out, const Account& cont);

    virtual Account* Clone(){
        return new Account(*this);
    }
};

class AccountBusiness : public Account{

public:
    AccountBusiness(User &u, Currency* currency=NULL) : Account(u, currency){};

    int getMaxCards() const {
        return 15;
    };

    std::string getType() const {
        return "Business";
    };

    AccountBusiness* Clone(){
        return new AccountBusiness(*this);
    }
};

std::ostream& operator<<(std::ostream& out, const Account& account);

Account& operator+(double value, Account& c);

double operator-(double value, Account& c);


class AccountBuilder{
    Account account;

public:

    AccountBuilder& setBalance(double balance){
        account.balance=balance;
        return *this;
    }

    AccountBuilder& setOwner(User* u){
        account.owner=u;
        return *this;
    }

    AccountBuilder& setCurrency(Currency* v){
        account.usedCurrency=v;
        return *this;
    }

    AccountBuilder& addCard(Card* c){
        account.addCard(c);
        return *this;
    }

    Account build(){
        if(account.owner == NULL || account.usedCurrency == NULL)
            throw std::string("Important data not set.");
        return account;
    }

};
