#include<iostream>
#include<chrono>
#include<ctime>
#include "../include/card.h"
#include "../include/menuUtils.h"
using namespace std;

//https://stackoverflow.com/questions/1692184/converting-epoch-time-to-real-date-time
void display_card_date(long temp) {
  const time_t old = (time_t)temp;
  struct tm *oldt = gmtime(&old);
  if(oldt->tm_mon<10)
    cout<<0;
  cout<<(1+oldt->tm_mon)<<"/"<<(1900+oldt->tm_year)%100;
}

ostream& operator<<(ostream& out, const Card& card){
    if(card.getType()=="Virtual")
        color(Colors::LIGHT_BLUE);
    else if(card.getType()=="Standard")
        color(Colors::BLUE);
    else if(card.getType()=="Premium")
        color(Colors::YELLOW);
    out<<"────────────────┤ Card Info ├────────────────\n";
    out<<" • Card Type: "<<card.getType()<<endl;
    out<<endl;
    out<<" • Card Number: "<<card.cardNumber<<endl;
    out<<" • CVV: "<<card.cvv<<endl;
    out<<" • Expire Date: ";
    display_card_date(card.expireDate);
    out<<endl;
    if(shared_ptr<Account> cardAccount = card.account.lock()) {
        out << " • Owner ID: " << cardAccount->getOwner()->getUserId() << endl;
        out << " • Owner Last Name: " << cardAccount->getOwner()->getLastName() << endl;
        out << " • Owner First Name: " << cardAccount->getOwner()->getFirstName() << endl;
    }
    else{
        out << " • Owner Deleted";
    }
    out<<"─────────────────────────────────────────────\n\n";
    color(Colors::WHITE);
    return out;
}

bool CardStandard::withdraw(double value, Currency* currency){
    if(shared_ptr<Account> account = this->account.lock()) {
        if (account->getOwner()->isBanned())
            return false;
        if (currency == NULL)
            currency = account->getCurrency();
        value = currency->toUnit(value);

        if (value > getMaximumTotal())
            return false;

        if (account->getBalance() < value)
            return false;
        account->setBalance(account->getBalance() - value);
        account->getTransactionsHistory()->logTransaction(value, false);

        return true;
    }
    else{
        cout<<"Oops, the account associated to this card no longer exists.";
        return false;
    }
}

bool CardStandard::deposit(double value, Currency* currency){
    if(shared_ptr<Account> account = this->account.lock()) {
        if (account->getOwner()->isBanned())
            return false;
        if (currency == NULL)
            currency = account->getCurrency();
        value = currency->toUnit(value);

        if (value > getMaximumTotal())
            return false;

        account->setBalance(account->getBalance() + value);
        account->getTransactionsHistory()->logTransaction(value, true);

        return true;
    }
    else{
        cout<<"Oops, the account associated to this card no longer exists.";
        return false;
    }
}

bool Card::transfer(double value, shared_ptr<Account>& involvedAccount, Currency* currency){
    if(shared_ptr<Account> account = this->account.lock()) {
        if (account->getOwner()->isBanned() || involvedAccount->getOwner()->isBanned())
            return false;
        if (currency == NULL)
            currency = account->getCurrency();
        value = currency->toUnit(value);
        if (account->getBalance() < value)
            return false;

        if (value > getMaximumTotal())
            return false;

        account->setBalance(account->getBalance() - value);
        account->getTransactionsHistory()->logTransaction(value, false, involvedAccount);

        involvedAccount->setBalance(involvedAccount->getBalance() + value);
        involvedAccount->getTransactionsHistory()->logTransaction(value, true, account);

        return true;
    }
    else{
        cout<<"Oops, the account associated to this card no longer exists.";
        return false;
    }
}

bool Card::transfer(double value, Card& card, Currency* currency){
    if(shared_ptr<Account> cardAcc = card.account.lock()){
        return transfer(value, cardAcc, currency);
    }
    else{
        cout<<"Oops, the parameter card's account no longer exists.";
        return false;
    }
}

bool Card::changeAccount(Account &acc){
    if(acc.addCard(this)){
        acc.detachCard(this);
        return true;
    }
    return false;
}

Card& Card::operator=(Card& card){
    expireDate=card.expireDate;
    return *this;
}

Card::Card(const std::shared_ptr<Account> &c) : cardNumber(generateCardNumber()), cvv(generateCVV()), expireDate(time(NULL) + 157784630){
    account=c;
}; // 5 years = 157784630 seconds

string CardDecorator::toCSV(){
    if(shared_ptr<Account> account = card->account.lock()) {
        return to_string(card->cardNumber) + "," + to_string(card->cvv) + "," + to_string(card->expireDate) + "," +
               to_string(
                       account->getAccountId());
    }
    else{
        return to_string(card->cardNumber) + "," + to_string(card->cvv) + "," + to_string(card->expireDate) + ",Deleted";
    }
}

string CardDecorator::toJSON(){
    if(shared_ptr<Account> account = card->account.lock()){
        return "{\"cardNumber\": "+to_string(card->cardNumber)+","
                +"\"cvv\": "+to_string(card->cvv)+","
                +"\"expireDate\": "+to_string(card->expireDate)+","
                +"\"account\": "+to_string(account->getAccountId())
                +"}";
    }
    else{
        return "{\"cardNumber\": "+to_string(card->cardNumber)+","
               +"\"cvv\": "+to_string(card->cvv)+","
               +"\"expireDate\": "+to_string(card->expireDate)+","
               +"\"account\": "+"null"
               +"}";
    }
}