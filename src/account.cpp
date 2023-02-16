#include<iostream>
#include "../include/account.h"
#include "../include/controlPanel.h"
#include "../include/menuUtils.h"
using namespace std;

Account& Account::operator+=(double value){
    this->balance+=value;
    return *this;
}

Account& Account::operator-=(double value){
    this->balance-=value;
    return *this;
}

Account& Account::operator+(const Account& c){
    Account& newA(*this);
    newA.balance+=c.balance;
    return newA;
}

Account& Account::operator+(double value){
    Account& newA(*this);
    newA.balance+=value;
    return newA;
}

Account& Account::operator-(double value){
    Account& newA(*this);
    newA.balance-=value;
    return newA;
}

Account& operator+(double value, Account& c){
    return c + value;
}

double operator-(double value, Account& c){
    return value - c.getBalance();
}


ostream& operator<<(ostream& out, const Account& account){
    if(account.getType() == "Personal")
        color(Colors::GREEN);
    else if(account.getType() == "Business")
        color(Colors::LIGHT_BLUE);
    out<<"────────────────┤ Account Info ├────────────────\n";
    out << " • Owner: " << account.owner->getLastName() + " " + account.owner->getFirstName() << endl;
    out << " • Owner ID: " << account.owner->getUserId() << endl;
    out<<"\n\n";
    out << " • Account ID: " << account.accountId << endl;
    out << " • Account Type: " << account.getType() << endl;
    out << " • Cards: " << account.getNumberOfCards() << "/" << account.getMaxCards() << endl;
    out<<"\n\n";
    out << " • Currency: " << account.usedCurrency->getName() << endl;
    out << " • Balance: " << account.usedCurrency->fromUnit(account.balance) << endl;
    out<<"─────────────────────────────────────────────\n\n";
    color(Colors::WHITE);

    return out;
}

Account::Account(User &u, Currency* currency): accountId(generateId()), historic(make_unique<TransactionsHistory>(*this)){
    owner=&u;
    if(currency == NULL)
        currency= ControlPanel::getControlPanel()->getCurrency();

    usedCurrency=currency;
}

Account::Account(const Account &c) : accountId(c.accountId), balance(c.balance), owner(c.owner), usedCurrency(c.usedCurrency), historic(make_unique<TransactionsHistory>(*c.historic)){
    for(auto& card : c.cards){
        //cards.push_back(make_unique<Card>(*card));    //causes object slicing
        cards.push_back(unique_ptr<Card>(card->Clone()));
    }
}

Account& Account::operator=(const Account &c){
    if(this==&c)
        return *this;
    balance=c.balance;
    owner=c.owner;
    usedCurrency=c.usedCurrency;

    historic.reset(new TransactionsHistory(*c.historic));

    for(auto& card : c.cards){
        //cards.push_back(make_unique<Card>(*card));     //causes object slicing
        cards.push_back(unique_ptr<Card>(card->Clone()));
    }

    return *this;
}

Account::~Account(){
    //before the smart pointers implementation
    //this code was required to replace with null pointer all connections
    //of other accounts (through transactions) with the account that gets deleted
    //to avoid undefined behaviour

    //the weak pointer will do that automatically now
    /*
    for(int i=0; i < historic->getTransactionsCount(); i++){
        Account* toModify = historic->getTransactions()[i]->getUsedAccount();
        if(toModify == NULL)
            continue;
        for(int j=0; j < toModify->historic->getTransactionsCount(); j++)
            if(toModify->historic->getTransactions()[j]->getUsedAccount() == this)
                toModify->historic->getTransactions()[j]->setAccount(NULL);
    }
    */
}

Card* Account::getCard(int cardId){
    for(int i=0; i < cards.size(); i++)
        if(cards[i]->getCardNumber() == cardId)
            return &(*cards[i]);
    return NULL;
}

bool Account::addCard(string tip){
    if(cards.size() == getMaxCards())
        return false;

    if(tip=="Virtual")
        cards.push_back(make_unique<Card>(this->shared_from_this()));
    else if(tip=="Standard")
        cards.push_back(make_unique<CardStandard>(this->shared_from_this()));
    else if(tip=="Premium")
        cards.push_back(make_unique<CardPremium>(this->shared_from_this()));
    else
        throw std::string("Unknown Card Type");

    return true;
}

bool Account::addCard(Card* card){
    if(cards.size() == getMaxCards())
        return false;

    //cards.push_back(unique_ptr<Card>(card));    //causes object slicing
    cards.push_back(unique_ptr<Card>(card->Clone()));

    return true;
}

bool Account::deleteCard(int cardNumber){
    for(auto card=cards.begin(); card != cards.end(); card++)
        if((*card)->getCardNumber()==cardNumber){
            cards.erase(card);
            return true;
        }
    return false;
}

void Account::detachCard(Card* card){
    for(auto cardL=cards.begin(); cardL != cards.end(); cardL++)
        if((*cardL)->getCardNumber()==card->getCardNumber()){
            (*cardL).release();
            cards.erase(cardL);
            break;
        }
}

bool Account::bankTransfer(double value, shared_ptr<Account> &account, Currency* currency){
    if(owner->isBanned() || account->owner->isBanned())
        return false;
    if(currency == NULL)
        currency=this->usedCurrency;
    value= currency->toUnit(value);
    if(balance < value)
        return false;
    balance-=value;
    historic->logTransaction(value, false, account);

    account->balance+=value;
    account->historic->logTransaction(value, true, this->shared_from_this());

    return true;
}

bool Account::bankTransfer(double value, Card& card, Currency* currency){
    if(shared_ptr<Account> account = card.getAccount().lock()){
        return bankTransfer(value, account, currency);
    }
    else{
        cout<<"The account associated with the parameter card no longer exists!";
        return false;
    }
}
