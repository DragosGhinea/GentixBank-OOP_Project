#pragma once
#include "account.h"
#include "currency.h"

class Account;
class CardDecorator;

class Card{

    static int generateCardNumber(){
        static int id=0;
        id++;
        return id;
    }

    static int generateCVV(){
        int cvv = rand()+1;
        cvv%=1000;
        return cvv;
    }

    protected:
    const int cardNumber;
    const int cvv;
    long expireDate;

    std::weak_ptr<Account> account;

public:
    friend class CardDecorator;

    virtual double getMaximumTotal() const {
        return 50000;
    }

    virtual std::string getType() const{
        return "Virtual";
    }

    int getCardNumber() const {return cardNumber;};
    int getCVV() const {return cvv;};
    long getExpireDate() const {return expireDate;};
    std::weak_ptr<Account>& getAccount(){return account;}

    bool transfer(double value, std::shared_ptr<Account> &involvedAccount, Currency* currency=NULL);
    bool transfer(double value, Card &card, Currency* currency=NULL);
    bool changeAccount(Account &acc);

    virtual bool withdraw(double value, Currency* currency = NULL){return false;};
    virtual bool deposit(double value, Currency* currency = NULL){return false;};

    Card(const std::shared_ptr<Account>& c);
    friend std::ostream& operator<<(std::ostream& out, const Card& card);

    Card& operator=(Card& c);

    virtual Card* Clone(){
        return new Card(*this);
    }

    virtual ~Card()=default;
};

class CardStandard : public Card{

public:
    CardStandard(const std::shared_ptr<Account>& c) : Card(c){};

    std::string getType() const{
        return "Standard";
    }

    bool withdraw(double value, Currency* currency = NULL);
    bool deposit(double value, Currency* currency = NULL);

    virtual CardStandard* Clone(){
        return new CardStandard(*this);
    }

    virtual ~CardStandard()=default;
};

class CardPremium : public CardStandard{

public:
    CardPremium(const std::shared_ptr<Account>& c) : CardStandard(c){};

    double getMaximumTotal() const {
        return 100000;
    }

    std::string getType() const{
        return "Premium";
    }

    virtual CardPremium* Clone(){
        return new CardPremium(*this);
    }

    virtual ~CardPremium()=default;
};

std::ostream& operator<<(std::ostream& out, const Card& card);

class CardDecorator{
    Card* card;

public:
    CardDecorator(Card* c){
        card=c;
    }

    std::string toCSV();

    std::string toJSON();

};