#include<iostream>
#include "../include/user.h"
#include "../include/controlPanel.h"
#include "../include/inputHandle.h"
#include "../include/menuUtils.h"
using namespace std;

void User::display(ostream& out, User* whoRequests) const {
    if(getType()=="Customer")
        color(Colors::GREEN);
    else if(getType()=="Moderator")
        color(Colors::YELLOW);
    else if(getType()=="Administrator")
        color(Colors::LIGHT_RED);
    out<<"────────────────┤ User Info ├────────────────\n";

    if(whoRequests == NULL)
        whoRequests=&ControlPanel::getControlPanel()->getRoot();
    
    out<<" • Category: "<<getType()<<endl;
    extraInformation(out, whoRequests);

    if(this->banned)
        out<<" • (*) Banned"<<endl;
    if(whoRequests->moderatorAcces())
        out << " • ID: " << this->userId << endl;
    out << " • Last Name: " << this->last_name << endl;
    out << " • First Name: " << this->first_name << endl;
    if(whoRequests->moderatorAcces())
        out<<" • Email: "<<this->email<<endl;
    color(Colors::WHITE);
}

//extraInformation

void Customer::extraInformation(ostream& out, User* whoRequests) const{
    if(whoRequests->moderatorAcces()){
        out << " • Accounts: " << getAccountsCount() << endl;
        out << " • Cards: " << getCardsCount() << endl;
    }
    else{
        out << whoRequests->getType();
    }

}

void Moderator::extraInformation(ostream& out, User* whoRequests) const{
    if(whoRequests->adminAcces()){
        out << " • Salary: " << getSalary() << " RON" << endl;
        out<<" • Bonus: "<<getBonus()<<" RON"<<endl;
        out<<" • Warnings: "<<getWarnings()<<endl;
    }
    Customer::extraInformation(out, whoRequests);
}

void Administrator::extraInformation(ostream& out, User* whoRequests) const{
    if(whoRequests->rootAcces()){
        out << " • Salary: " << getSalary() << " RON" << endl;
        out<<" • Bonus: "<<getBonus()<<" RON"<<endl;
        out<<" • Warnings: "<<getWarnings()<<endl;
    }
    Customer::extraInformation(out, whoRequests);
}

//end extraInformation

//edit()

void User::edit(istream& in){
    cout<<"────────────────┤ User Info Edit ├────────────────\n";
    cout<<"> Enter 0 anytime to cancel.\n";

    string lastName;
    cout<< "> Last Name (" + this->last_name + "): ";
    in >> lastName;
    if(lastName == "0"){
        cout<<"> User info edit cancelled.\n";
        return;
    }

    string firstName;
    cout<< "> First Name (" + this->first_name + "): ";
    in >> firstName;
    if(firstName == "0"){
        cout<<"> User info edit cancelled.\n";
        return;
    }

    string email;
    cout<<"> Email ("+this->email+"): ";
    in>>email;
    if(email=="0"){
        cout<<"> User info edit cancelled.\n";
        return;
    }

    this->last_name=lastName;
    this->first_name=firstName;
    this->email=email;
    cout<<"> Editing finished!\n";
}

void Employee::edit(istream& in){
    cout<<"────────────────┤ Date User ├────────────────\n";
    cout<<"> Enter -1 anytime to cancel.\n";

    string lastName;
    cout<< "> Last Name (" + this->last_name + "): ";
    in >> lastName;
    if(lastName == "-1"){
        cout<<"> User info edit cancelled.\n";
        return;
    }

    string firstName;
    cout<< "> First Name (" + this->first_name + "): ";
    in >> firstName;
    if(firstName == "-1"){
        cout<<"> User info edit cancelled.\n";
        return;
    }

    string email;
    cout<<"> Email ("+this->email+"): ";
    in>>email;
    if(email=="-1"){
        cout<<"> User info edit cancelled.\n";
        return;
    }

    double salary;
    cout << "> Salary (" << this->salary << " RON): ";
    getInput(salary, "Please insert a proper number.");
    if(salary == -1){
        cout<<"> User info edit cancelled.\n";
        return;
    }

    double salaryBonus;
    cout << "> Bonus (" << this->salaryBonus << " RON): ";
    getInput(salaryBonus, "Please insert a proper number.");
    if(salaryBonus == -1){
        cout<<"> User info edit cancelled.\n";
        return;
    }

    this->last_name=lastName;
    this->first_name=firstName;
    this->email=email;
    this->salary=salary;
    this->salaryBonus=salaryBonus;

    cout<<"> Editng finished!\n";
}

//end edit()

//user related

User& User::operator=(const User& u){
    if(this==&u)
        return *this;

    last_name=u.last_name;
    first_name=u.first_name;
    email=u.email;

    return *this;
}

ostream& operator<<(ostream& out, const User& u){
    u.display();

    return out;
}

istream& operator>>(istream& in, User& u){
    u.edit(in);
    return in;
}

User::User(const User& u) : userId(u.userId){
    banned=u.banned;
    last_name=u.last_name;
    first_name=u.first_name;
    email=u.email;
}

User::User(string last_name1, string first_name1, string email) : userId(generateId()){
    this->last_name=last_name1;
    this->first_name=first_name1;
    this->email=email;
}

bool User::setBanned(bool banned1){
    if(banned == banned1)
        return false;
    banned=banned1;
    return true;
}

//customer related

Customer::Customer(const Customer& customer) : User(customer){
    for(const shared_ptr<Account>& account : customer.accounts)
        //accounts.push_back(make_shared<Account>(*account));    //causes object slicing
        accounts.push_back(shared_ptr<Account>(account->Clone()));

}

Customer& Customer::operator=(const Customer& customer){
    if(this==&customer)
        return *this;
    User::operator=(customer);
    accounts.clear();
    for(const shared_ptr<Account>& account : customer.accounts)
        //accounts.push_back(make_shared<Account>(*account));    //causes object slicing
        accounts.push_back(shared_ptr<Account>(account->Clone()));
    
    return *this;
}

int Customer::getCardsCount() const{
    int toReturn=0;
    for(const shared_ptr<Account>& c : getAccounts())
        toReturn+= c->getNumberOfCards();
    
    return toReturn;
}

//optimistic approach
//we assume no account is deleted
//while searching for a card
Card* Customer::getCard(int cardId){
    Card* c=NULL;
    for(int i=0; i < accounts.size(); i++){
        c=accounts[i]->getCard(cardId);
        if(c!=NULL)
            break;
    }
    return c;
}

shared_ptr<Account> Customer::getAccount(int accountId){
    for(auto& cont : getAccounts())
        if(cont->getAccountId() == accountId)
            return cont;
    return nullptr;
}

bool Customer::createAccount(bool business, Currency* currency){
    if(currency == NULL)
        currency= ControlPanel::getControlPanel()->getCurrency();
    if(accounts.size() == 5)
        return false;
    if(business)
        accounts.push_back(make_shared<AccountBusiness>(*this, currency));
    else
        accounts.push_back(make_shared<Account>(*this, currency));
    return true;
}

bool Customer::deleteAccount(){
    if(accounts.size() == 0)
        return false;
    accounts.pop_back();
    return true;
}

bool Customer::deleteAccount(int accountId){
    for(auto account=accounts.begin(); account != accounts.end(); account++){
        if((*account)->getAccountId() == accountId){
            accounts.erase(account);
            return true;
        }
    }
    return false;
}