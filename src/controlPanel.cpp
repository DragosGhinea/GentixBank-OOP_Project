#include<iostream>
#include "../include/controlPanel.h"
#include "../include/inputHandle.h"
using namespace std;

ControlPanel::~ControlPanel(){
    for(auto& i : currencies)
        delete i;

    for(auto& i : customers)
        delete i;
}

bool ControlPanel::loadRandomData(){
    static bool executed=false;
    if(executed)
        return false;
    executed=true;

    //Romanian names
    vector<string> names = {
        "Constantinescu",
        "Iliescu",
        "Marin",
        "Vladimirescu",
        "Dumitrescu",
        "Vaduva",
        "Negrescu",
        "Alexandrescu",
        "Petrescu",
        "Popa",
        "Petran",
        "Grigorescu",
        "Adam",
        "Ungureanu",
        "Lungu",
        "Solomon",
        "Lupei",
        "Anghelescu",
        "Badea",
        "Balan",
        "Nicolescu",
        "Rusu"
    };

    vector<string> firstNames = {
        "Claudia",
        "Daria",
        "Alina",
        "Teodor",
        "Stefan",
        "Mihai-Alexandru",
        "Cornel",
        "Albert",
        "Liliana",
        "Ioana",
        "Toma",
        "Marian",
        "Ana",
        "Daniel",
        "Izabela-Maria",
        "Rodica-Andreea",
        "Marinela",
        "Catalin-Radu",
        "Dragos",
        "Matei",
        "Florina",
        "Rares"
    };

    vector<string> emails = {
        "c.claudia@gmail.com",
        "daria.iliescu@gmail.com",
        "alina.marin@gmail.com",
        "v.teodor@yahoo.com",
        "stefan.dumi@gmail.com",
        "mihai.vaduva@yahoo.com",
        "negrescu.cornel@gmail.com",
        "albert.alex@yahoo.com",
        "lili.petrescu@gmail.com",
        "popa.ioana@yahoo.com",
        "toma.petran@yahoo.com",
        "m.grigorescu@yahoo.com",
        "adam.ana@gmail.com",
        "dani.ungureanu@yahoo.com",
        "iza.lungu@yahoo.com",
        "andreea.solo@gmail.com",
        "mari.lupei@yahoo.com",
        "radu.a22@gmail.com",
        "dragos.b@yahoo.com",
        "balan.matei@gmail.com",
        "florina.nicolescu@yahoo.com",
        "rusu.r@gmail.com"
    };

    for(int i=0;i<22;i++){
        //25% chance administrator
        //25% chance moderator
        //50% chance customer
        int chance = rand() % 100;
        if(chance < 50)
            customers.push_back(new Customer(names[i], firstNames[i], emails[i]));
        else if(chance < 75)
            customers.push_back(new Moderator("moderator2", names[i], firstNames[i], emails[i], 3000));
        else
            customers.push_back(new Administrator("amGoodAdmin1", names[i], firstNames[i], emails[i], 7000));
    }


    currencies.push_back(new Currency("RON", 1)); //we'll use RON as the unit reference for currencies
    currencies.push_back(new Currency("EUR", 4.95));
    currencies.push_back(new Currency("USD", 4.53));
    currencies.push_back(new Currency("TRY", 0.32));
    currencies.push_back(new Currency("GBP", 5.99));

    for(auto& customer : customers){
        int accounts = rand() % 5 + 1;

        for(int j=0; j < accounts; j++){
            Currency* currency=currencies[rand() % getCurrenciesSize()];

            customer->createAccount(rand() % 2, currency);
            Account* c=&(*customer->getAccounts()[j]);

            int cards = rand() % 5 + 1;
            for(int h=0; h < cards; h++){
                int cardType= rand() % 3;
                if(cardType == 0)
                    c->addCard();
                else if(cardType == 1)
                    c->addCard("Standard");
                else
                    c->addCard("Premium");
            }
        }
    }

    for(auto& customer : customers){
        for(int j=0; j < customer->getAccountsCount(); j++){
            if(customer->getAccounts()[j]->getNumberOfCards() != 0)
                customer->getAccounts()[j]->setBalance(rand());
        }
    }


    for(Customer*& customer : getCustomers()) {
        for(shared_ptr<Account>& c : customer->getAccounts()){
            int nrTransactions = rand() % 16;
            for (int i = 0; i < nrTransactions; i++) {
                int option = rand() % 3;
                //0 = transfer
                //1 = withdraw
                //2 = deposit
                if (option == 0) {
                    if (c->getBalance() == 0)
                        continue;
                    int randomCustomer2 = rand() % getCustomersSize();
                    double value = rand() % ((int) c->getBalance());
                    c->bankTransfer(value, customers[randomCustomer2]->getAccounts()[0]);
                } else if (option == 1) {
                    if (c->getBalance() == 0)
                        continue;
                    double value = rand() % ((int) c->getBalance());
                    c->getCards()[0]->withdraw(value);
                } else {
                    double value = rand();
                    c->getCards()[0]->deposit(value);
                }
            }
        }
    }

    return true;
}

Administrator* ControlPanel::getRandomAdministrator(){
    int v[getCustomersSize()];
    int adminCount=0;
    for(int i=0; i < getCustomersSize(); i++)
        if(customers[i]->getType() == "Administrator")
            v[adminCount++]=i;
    if(adminCount == 0)
        return NULL;
    int admin= rand() % adminCount;
    //we are assuming no other child classes will appear
    //that also have the "Administrator" getType()
    //so static_cast is safe
    return static_cast<Administrator*>(customers[v[admin]]);
}

Moderator* ControlPanel::getRandomModerator(){
    int v[getCustomersSize()];
    int moderatorCount=0;
    for(int i=0; i < getCustomersSize(); i++)
        if(customers[i]->getType() == "Moderator")
            v[moderatorCount++]=i;
    if(moderatorCount == 0)
        return NULL;
    int admin= rand() % moderatorCount;
    //we are assuming no other child classes will appear
    //that also have the "Moderator" getType()
    //so static_cast is safe
    return static_cast<Moderator*>(customers[v[admin]]);
}

Customer* ControlPanel::getRandomCustomer(){
    int v[getCustomersSize()];
    int customerCount=0;
    for(int i=0; i < getCustomersSize(); i++)
        if(customers[i]->getType() == "Customer")
            v[customerCount++]=i;
    if(customerCount == 0)
        return NULL;
    int client= rand() % customerCount;
    return customers[v[client]];
}

Customer* ControlPanel::getCustomer(int id){
    for(auto& customer : customers)
        if(customer->getUserId() == id)
            return customer;
    return NULL;
}

bool ControlPanel::logout(){
    if(logged == NULL)
        return false;
    logged=NULL;
    return true;
}


Currency* ControlPanel::getCurrency(string currencyName){
    if(currencyName == "")
        return currencies[0];
    for(auto& currency : currencies)
        if(currency->getName() == currencyName)
            return currency;
    return NULL;
}

Currency* ControlPanel::getCurrency(int id){
    for(auto& currency : currencies)
        if(currency->getCurrencyId() == id)
            return currency;
    return NULL;
}

void ControlPanel::createCurrency(){
    cout<<"=========== [ New Currency ] ===========\n";
    cout<<"> Enter 0 anytime to cancel.\n";

    cout<<"> Name: ";
    string currencyName;
    getInput(currencyName, "Something went wrong! Make sure you insert a valid string.");
    if(currencyName == "0"){
        cout<<">>> New currency creation cancelled!";
        return;
    }

    cout<<"> (How many default units (RON) is this currency worth? (Ex: 4.95 | 1 EUR = 4.95 RON)\n";
    cout<<"> Exchange Rate: ";
    double exchangeRate;
    getInput(exchangeRate, "Something went wrong! Make sure you insert a valid number.");
    if(exchangeRate==0){
        cout<<">>> New currency creation cancelled!";
        return;
    }
    currencies.push_back(new Currency(currencyName, exchangeRate));
    cout<<">>> New currency created!\n";
}

void ControlPanel::createCustomer(){
    cout<<"=========== [ New Customer ] ===========\n";
    cout<<"> Enter 0 anytime to cancel.\n";

    cout<<"> What type of user do you want to create?\n";
    string category;
    do{
        cout<<"Available options:\n";
        cout<<"- Administrator\n";
        cout<<"- Moderator\n";
        cout<<"- Customer\n";
        cout<<"> Type: ";
        getInput(category, "Something went wrong! Make sure you insert a valid string.");
        if(category == "0"){
            cout<<"> User creation cancelled.\n";
            return;
        }
    }while(category != "Administrator" && category != "Moderator" && category != "Customer");

    string lastName;
    cout<<"> Last Name: ";
    getInput(lastName, "Something went wrong! Make sure you insert a valid string.");
    if(lastName == "0"){
        cout<<"> User creation cancelled.\n";
        return;
    }

    string firstName;
    cout<<"> First Name: ";
    getInput(firstName, "Something went wrong! Make sure you insert a valid string.");
    if(firstName == "0"){
        cout<<"> User creation cancelled.\n";
        return;
    }

    string email;
    cout<<"> Email: ";
    getInput(email, "Something went wrong! Make sure you insert a valid string.");
    if(email=="0"){
        cout<<"> User creation cancelled.\n";
        return;
    }

    if(category == "Customer")
        customers.push_back(new Customer(lastName, firstName, email));
    else{
        string password;
        cout<<"> Password Employee: ";
        getInput(password, "Something went wrong! Make sure you insert a valid string.");
        if(password == "0"){
            cout<<"> User creation cancelled.\n";
            return;
        }

        double salary;
        cout<<"> Salary in default units (RON): ";
        getInput(salary, "Insert a valid decimal number!");
        if(salary == 0){
            cout<<"> User creation cancelled.\n";
            return;
        }

        if(category == "Administrator"){
            customers.push_back(new Administrator(password, lastName, firstName, email, salary));
        }
        else{
            customers.push_back(new Moderator(password, lastName, firstName, email, salary));
        }
    }
}

bool ControlPanel::deleteCurrency(string currencyName){
    for(auto& customer : customers){
        for(int j=0; j < customer->getAccountsCount(); j++)
            if(customer->getAccounts()[j]->getCurrency()->getName() == currencyName){
                cout << "> Currency " << currencyName << " is used by at least one account.\n";
                cout<<"> Make sure no account has this currency before removal.\n";
                return false;
            }
    }

    for(auto currency=currencies.begin(); currency != currencies.end(); currency++)
        if((*currency)->getName() == currencyName){
            delete *currency;
            currencies.erase(currency);
            return true;
        }

    return false;
}

bool ControlPanel::deleteCustomer(int id){
    for(auto customer=customers.begin(); customer != customers.end(); customer++)
        if((*customer)->getUserId() == id){
            delete (*customer);
            customers.erase(customer);
            return true;
        }
    return false;
}

bool ControlPanel::deleteCustomer(string last_name, string first_name){
    for(auto customer=customers.begin(); customer != customers.end(); customer++)
        if((*customer)->getLastName() == last_name && (*customer)->getFirstName() == first_name){
            delete *customer;
            customers.erase(customer);
            return true;
        }
    return false;
}

ControlPanel* ControlPanel::instance = 0;