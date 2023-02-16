#pragma once
#include<vector>
#include "user.h"

class ControlPanel{
    Root root;
    User* logged=NULL;

    std::vector<Currency*> currencies;
    std::vector<Customer*> customers;

    //void resizeUtilizatori();
    //void resizeValuta();

    static ControlPanel* instance;
    ControlPanel() : root("unBreakable1"){};
    ~ControlPanel();
    ControlPanel(ControlPanel& p);
    ControlPanel& operator=(ControlPanel& p);

public:

    static ControlPanel* getControlPanel(){
        if(!instance)
            instance=new ControlPanel();
        return instance;
    }

    User* getLogged(){return logged;}

    void login(User* u);
    bool logout();

    Root& getRoot(){return root;};

    Customer* getRandomCustomer();
    Moderator* getRandomModerator();
    Administrator* getRandomAdministrator();

    void menuUser();

    Currency* getCurrency(std::string currencyName= "");
    Currency* getCurrency(int id);
    std::vector<Currency*>& getCurrencies(){return currencies;}
    std::vector<Customer*>& getCustomers(){return customers;}

    int getCurrenciesSize(){return currencies.size();}
    int getCustomersSize(){return customers.size();}

    Customer* getCustomer(int id);
    void createCustomer();
    bool deleteCustomer(int id);
    bool deleteCustomer(std::string last_name, std::string first_name);

    void createCurrency();
    bool deleteCurrency(std::string currencyName);

    bool loadRandomData();

    void menu();
};
