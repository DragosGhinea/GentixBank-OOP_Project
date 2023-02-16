#include "../include/controlPanel.h"
#include "../include/menuUtils.h"
#include "../include/inputHandle.h"
#include<conio.h>
using namespace std;

enum UserOptions{
    EditPersonalData=0,
    ShowAccounts=1,
    ShowCards=2,
    ManageMode=3
};

enum ModeratorOptions{
    ShowUsers=6,
    AddAccount=7,
    DeleteAccount=8,
    AddCard=9,
    DeleteCard=10,
    SetBalance=11
};

enum AdministratorOptions{
    BanUser=12,
    AddUser=13,
    DeleteUser=14,
    TransferCard=15
};

enum ROOTOptions{
    ShowCurrencies=16,
    AddCurrencies=17,
    DeleteCurrencies=18
};

void userExecute(int option);
void manageExecute(int option);

UserOptions getNextOption(UserOptions& option, bool manage){
    switch(option){
        case EditPersonalData:
            return ShowAccounts;
        case ShowAccounts:
            return ShowCards;
        case ShowCards:
            if(manage)
                return ManageMode;
            else
                return EditPersonalData;
        default:
            return EditPersonalData;
    }
}

UserOptions getPreviousOption(UserOptions& option, bool manage){
    switch(option){
        case EditPersonalData:
            if(manage)
                return ManageMode;
            else
                return ShowCards;
        case ManageMode:
            return ShowCards;
        case ShowCards:
            return ShowAccounts;
        default:
            return EditPersonalData;
    }
}

void ControlPanel::login(User* u){
    cout<<"\n";
    clearConsole();
    if(u->getType()=="Root"){
        Root* root=dynamic_cast<Root*>(u);
        cout<<"» To cancel enter 0\n";
        cout<<"» Insert password: ";
        string password;
        getInput(password, "Insert a valid string!\n");
        if(password == "0"){
            return;
        }
        while(root->getPassword() != password){
            cout<<"Incorrect password!\nTry again = ";
            getInput(password, "Insert a valid string!\n");
            if(password == "0"){
                return;
            }
        };
    }

    logged=u;
    menuUser();
}

void loadUserInterface(UserOptions option, bool manage){
    cout<<"\n";
    clearConsole();

    cout<<"┌───────────────────────────────────────────────────────────────────────────────────────┐\n";
    cout<<"│                                                                                       │\n";
    cout<<"│                                   User Interface                                      │\n";
    cout<<"│                                                                                       │\n";
    cout<<"├──────────────────────────────────────────────────────┬────────────────────────────────┤\n";
    cout<<"│                     Informations                     │            Options             │\n";
    cout<<"├──────────────────────────────────────────────────────┼────────────────────────────────┤\n";
    cout<<"│                                                      │       Edit personal data       │\n";
    cout<<"│                                                      ├────────────────────────────────┤\n";
    cout<<"│                                                      │         Show accounts          │\n";
    cout<<"│                                                      ├────────────────────────────────┤\n";
    cout<<"│                                                      │           Show cards           │\n";
    cout<<"│                                                      ├────────────────────────────────┤\n";
    cout<<"│                                                      │                                │\n";
    cout<<"│                                                      │                                │\n";
    cout<<"│                                                      │                                │\n";
    cout<<"└──────────────────────────────────────────────────────┴────────────────────────────────┘\n";

    gotoxy(63, 7);
    if(option == EditPersonalData){
        coutColoredText("Edit personal data", Colors::RED);
    }
    else
        cout<<"Edit personal data";

    gotoxy(65, 9);
    if(option == ShowAccounts){
        coutColoredText("Show accounts", Colors::RED);
    }
    else
        cout<<"Show accounts";

    gotoxy(67, 11);
    if(option == ShowCards){
        coutColoredText("Show cards", Colors::RED);
    }
    else
        cout<<"Show cards";

    gotoxy(64, 14);
    if(manage){
        if(option == ManageMode){
            coutColoredText("Management Panel", Colors::RED);
        }
        else
            coutColoredText("Management Panel", Colors::LIGHT_CYAN);
    }

    gotoxy(3, 7);
    User& u=*ControlPanel::getControlPanel()->getLogged();
    gotoxy(3, 8);
    cout<<"• Last Name: "<< u.getLastName();
    gotoxy(3, 9);
    cout<<"• First Name: "<< u.getFirstName();
    gotoxy(3, 10);
    cout<<"• Email: "<<u.getEmail();
    try{
        Customer& c=dynamic_cast<Customer&>(u);
        gotoxy(3, 11);
        cout<<"• Accounts: "<< c.getAccountsCount();
        gotoxy(3, 12);
        cout<<"• Cards: "<< c.getCardsCount();
    }catch(...){

    }
    gotoxy(3, 14);
    cout<<"• User Category: "<<u.getType();
}

int getManageNextOption(int option){
    if(option == SetBalance){
        if(ControlPanel::getControlPanel()->getLogged()->adminAcces())
            return option + 1;
        else
            return ShowUsers;
    }
    else if(option == TransferCard){
        if(ControlPanel::getControlPanel()->getLogged()->rootAcces())
            return option + 1;
        else return ShowUsers;
    }

    if(option == DeleteCurrencies){
        return ShowUsers;
    }
    return option + 1;
}

int getManagePreviousOption(int option){
    if(option == ShowUsers){
        if(ControlPanel::getControlPanel()->getLogged()->rootAcces())
            return DeleteCurrencies;
        else if(ControlPanel::getControlPanel()->getLogged()->adminAcces())
            return TransferCard;
        else
            return SetBalance;
    }

    return option - 1;
}

void loadAdminInterface(int option){
    User* u= ControlPanel::getControlPanel()->getLogged();
    gotoxy(0, 17);
    cout<<"┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n";
    cout<<"│                                                                                                                 │\n";
    cout<<"│                                                 Management Interface                                            │\n";
    cout<<"│                                                                                                                 │\n";
    cout<<"├───────────────────┬───────────────────┬─────────────┬────────────────┬──────────┬─────────────┬─────────────────┤\n";
    cout<<"│ Moderator Options │    Show Users     │ Add Account │ Delete Account │ Add Card │ Delete Card │   Set Balance   │\n";

    if(u->adminAcces()){
        cout<<"├───────────────────┴───┬───────────────┴────┬────────┴───────────┬────┴──────────┴─────────┬───┴─────────────────┤\n";
        cout<<"│ Administrator Options │      Ban User      │      Add User      │       Delete User       │    Transfer Card    │\n";
    }

    if(u->rootAcces()){
        cout<<"├───────────────┬───────┴────────────────────┴────┬───────────────┴───────────────┬─────────┴─────────────────────┤\n";
        cout<<"│  ROOT Options │         List Currencies         │         Add Currency          │        Delete Currency        │\n";
        cout<<"└───────────────┴─────────────────────────────────┴───────────────────────────────┴───────────────────────────────┘\n";
    }
    else if(u->adminAcces())
        cout<<"└───────────────────────┴────────────────────┴────────────────────┴─────────────────────────┴─────────────────────┘\n"; //admin
    else
        cout<<"└───────────────────┴───────────────────┴─────────────┴────────────────┴──────────┴─────────────┴─────────────────┘\n"; //mod
}

void adminInterface(){
    int option=ShowUsers;

    loadAdminInterface(option);

    gotoxy(25, 22);
    if(option == ShowUsers){
        coutColoredText("Show Users", Colors::RED);
    }
    else
        cout<<"Show Users";

    gotoxy(42, 22);
    if(option == AddAccount){
        coutColoredText("Add Account", Colors::RED);
    }
    else
        cout<<"Add Account";

    gotoxy(56, 22);
    if(option == DeleteAccount){
        coutColoredText("Delete Account", Colors::RED);
    }
    else
        cout<<"Delete Account";

    gotoxy(73, 22);
    if(option == AddCard){
        coutColoredText("Add Card", Colors::RED);
    }
    else
        cout<<"Add Card";

    gotoxy(84, 22);
    if(option == DeleteCard){
        coutColoredText("Delete Card", Colors::RED);
    }
    else
        cout<<"Delete Card";

    gotoxy(100, 22);
    if(option == SetBalance){
        coutColoredText("Set Balance", Colors::RED);
    }
    else
        cout<<"Set Balance";

    if(ControlPanel::getControlPanel()->getLogged()->adminAcces()){
        gotoxy(31, 24);
        if(option == BanUser){
            coutColoredText("Ban User", Colors::RED);
        }
        else
            cout<<"Ban User";

        gotoxy(52, 24);
        if(option == AddUser){
            coutColoredText("Add User", Colors::RED);
        }
        else
            cout<<"Add User";

        gotoxy(74, 24);
        if(option == DeleteUser){
            coutColoredText("Delete User", Colors::RED);
        }
        else
            cout<<"Delete User";

        gotoxy(97, 24);
        if(option == TransferCard){
            coutColoredText("Transfer Card", Colors::RED);
        }
        else
            cout<<"Transfer Card";
    }

    if(ControlPanel::getControlPanel()->getLogged()->rootAcces()){
        gotoxy(26, 26);
        if(option == ShowCurrencies){
            coutColoredText("Show Currencies", Colors::RED);
        }
        else
            cout<<"Show Currencies";

        gotoxy(60, 26);
        if(option == AddCurrencies){
            coutColoredText("Add Currency", Colors::RED);
        }
        else
            cout<<"Add Currency";

        gotoxy(91, 26);
        if(option == DeleteCurrencies){
            coutColoredText("Delete Currency", Colors::RED);
        }
        else
            cout<<"Delete Currency";
    }


    char key;
    while(1){
        key=_getch();

        if(key==27){
            loadUserInterface(ManageMode, true);
            break;
        }
        else if(key==13){
            manageExecute(option);
            loadUserInterface(ManageMode, true);
            loadAdminInterface(option);
        }
        else if(key==87 || key==119 || key==72 || key==75){
            option= getManagePreviousOption(option);
        }
        else if(key==83 || key==115 || key==80 || key==77)
            option= getManageNextOption(option);
        else
            continue;

        gotoxy(25, 22);
        if(option == ShowUsers){
            coutColoredText("Show Users", Colors::RED);
        }
        else
            cout<<"Show Users";

        gotoxy(42, 22);
        if(option == AddAccount){
            coutColoredText("Add Account", Colors::RED);
        }
        else
            cout<<"Add Account";

        gotoxy(56, 22);
        if(option == DeleteAccount){
            coutColoredText("Delete Account", Colors::RED);
        }
        else
            cout<<"Delete Account";

        gotoxy(73, 22);
        if(option == AddCard){
            coutColoredText("Add Card", Colors::RED);
        }
        else
            cout<<"Add Card";

        gotoxy(84, 22);
        if(option == DeleteCard){
            coutColoredText("Delete Card", Colors::RED);
        }
        else
            cout<<"Delete Card";

        gotoxy(100, 22);
        if(option == SetBalance){
            coutColoredText("Set Balance", Colors::RED);
        }
        else
            cout<<"Set Balance";

        if(ControlPanel::getControlPanel()->getLogged()->adminAcces()){
            gotoxy(31, 24);
            if(option == BanUser){
                coutColoredText("Ban User", Colors::RED);
            }
            else
                cout<<"Ban User";

            gotoxy(52, 24);
            if(option == AddUser){
                coutColoredText("Add User", Colors::RED);
            }
            else
                cout<<"Add User";

            gotoxy(74, 24);
            if(option == DeleteUser){
                coutColoredText("Delete User", Colors::RED);
            }
            else
                cout<<"Delete User";

            gotoxy(97, 24);
            if(option == TransferCard){
                coutColoredText("Transfer Card", Colors::RED);
            }
            else
                cout<<"Transfer Card";
        }

        if(ControlPanel::getControlPanel()->getLogged()->rootAcces()){
            gotoxy(26, 26);
            if(option == ShowCurrencies){
                coutColoredText("Show Currencies", Colors::RED);
            }
            else
                cout<<"Show Currencies";

            gotoxy(60, 26);
            if(option == AddCurrencies){
                coutColoredText("Add Currency", Colors::RED);
            }
            else
                cout<<"Add Currency";

            gotoxy(91, 26);
            if(option == DeleteCurrencies){
                coutColoredText("Delete Currency", Colors::RED);
            }
            else
                cout<<"Delete Currency";
        }

    }
}

void twoFactorPassword(){
    cout<<"\n";
    clearConsole();
    Employee* angajat=dynamic_cast<Employee*>(ControlPanel::getControlPanel()->getLogged());
    cout<<"» To cancel insert 0\n";
    cout<<"» Insert password: ";
    string password;
    getInput(password, "Insert a valid string!\n");
    if(password == "0"){
        loadUserInterface(ManageMode, true);
        return;
    }
    while(angajat->getPassword2Factor() != password){
        cout<<"Incorrect password!\nTry again = ";
        getInput(password, "Insert a valid string!\n");
        if(password == "0"){
            loadUserInterface(ManageMode, true);
            return;
        }
    }

    loadUserInterface(ManageMode, true);
    adminInterface();
}

void ControlPanel::menuUser(){
    //cout<<"─ │ ┐ ┌ └ ┘ ├ ┤ ┬ ┴ ┼\n";
    UserOptions option=EditPersonalData;
    bool manage= logged->getType() != "Customer";

    loadUserInterface(option, manage);

    char key;
    while(1){
        gotoxy(0, 20);
        key=_getch();

        if(key==27){
            ControlPanel::logout();
            break;
        }
        else if(key==13){
            userExecute(option);
            loadUserInterface(option, manage);
        }
        else if(key==87 || key==119 || key==72 || key==75){
            option= getPreviousOption(option, manage);
        }
        else if(key==83 || key==115 || key==80 || key==77)
            option= getNextOption(option, manage);
        else
            continue;

        gotoxy(63, 7);
        if(option == EditPersonalData){
            coutColoredText("Edit personal data", Colors::RED);
        }
        else
            cout<<"Edit personal data";

        gotoxy(65, 9);
        if(option == ShowAccounts){
            coutColoredText("Show accounts", Colors::RED);
        }
        else
            cout<<"Show accounts";

        gotoxy(67, 11);
        if(option == ShowCards){
            coutColoredText("Show cards", Colors::RED);
        }
        else
            cout<<"Show cards";

        gotoxy(64, 14);
        if(manage){
            if(option == ManageMode){
                coutColoredText("Management Panel", Colors::RED);
            }
            else
                coutColoredText("Management Panel", Colors::LIGHT_CYAN);
        }
    }
}

void userExecute(int option){
    User* u= ControlPanel::getControlPanel()->getLogged();
    Customer* customer;
    shared_ptr<Account> account;
    int accountId;
    switch(option){
        case EditPersonalData:
            if(u->getType()=="Root"){
                coutColoredText("Error: ROOT doesn't have these settings.", Colors::LIGHT_RED);
                _getch();
                break;
            }
            cout<<"\n";
            clearConsole();
            u->User::edit(cin);
            break;
        case ShowAccounts:
            if(u->getType()=="Root"){
                coutColoredText("Error: ROOT doesn't have these settings.", Colors::LIGHT_RED);
                _getch();
                break;
            }
            cout<<"\n";
            clearConsole();
            customer=dynamic_cast<Customer*>(u);
            for(shared_ptr<Account>& c : customer->getAccounts())
                cout<<*c;

            cout<<"\nDo you want more details about an account?\n";
            cout<<"Enter -1 to go back to the main menu.\n";
            cout<<"Enter an account id to see the transaction history.\n";
            cout<<"Account ID=";
            getInput(accountId, "Please enter a valid id.");
            if(accountId!=-1) {
                account = customer->getAccount(accountId);
                while (account == nullptr) {
                    cout << "No account found with the provided id, try again=";
                    getInput(accountId, "Please enter a valid id.");
                    if(accountId==-1)
                        break;
                    account = customer->getAccount(accountId);
                }
                if(accountId!=-1) {
                    cout<<"\n\n";
                    color(Colors::GREEN);
                    cout<<*account->getTransactionsHistory();
                    color(Colors::WHITE);
                    cout << "Press anything to go back to the menu.\n";
                    _getch();
                }
            }
            break;
        case ShowCards:
            if(u->getType()=="Root"){
                coutColoredText("Error: ROOT doesn't have these settings.", Colors::LIGHT_RED);
                _getch();
                break;
            }
            cout<<"\n";
            clearConsole();

            customer=dynamic_cast<Customer*>(u);

            if(customer->getAccountsCount() == 0){
                cout<<"There are no accounts yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << customer->getAccounts()[0]->getAccountId();
                for(int i=1; i < customer->getAccountsCount(); i++)
                    cout << ", " << customer->getAccounts()[i]->getAccountId();
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Account ID: ";
            getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
            account= customer->getAccount(accountId);
            while(account == NULL){
                cout<<"Account not found!\n";
                cout<<"Account ID: ";
                getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
                account= customer->getAccount(accountId);
            }

            for(int i=0; i < account->getNumberOfCards(); i++)
                cout<<*account->getCards()[i];

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case ManageMode:
            if(u->getType()=="Root")
                adminInterface();
            else
                twoFactorPassword();
    }
}

void manageExecute(int option){
    User* u = ControlPanel::getControlPanel()->getLogged();
    int customerId;
    int accountId;
    Customer *customer;
    Currency *currency;
    Card* card;
    shared_ptr<Account> account;
    string name;
    int cardId;
    string s;
    double sum;
    bool validCard;
    switch(option){
        case ShowUsers:
            cout<<"\n";
            clearConsole();
            for(Customer* c : ControlPanel::getControlPanel()->getCustomers()){
                c->display(cout, u);
            }

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case AddAccount:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCurrenciesSize() == 0){
                cout<<"There needs to exist at least one currency to create an account!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            if(ControlPanel::getControlPanel()->getCustomersSize() == 0){
                cout<<"No customers registered yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCustomers()[0]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[0] << ")";
                for(int i=1; i < ControlPanel::getControlPanel()->getCustomersSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCustomers()[i]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[i] << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Customer ID: ";
            getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
            customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            while(customer == NULL){
                cout<<"Customer not found!\n";
                cout<<"Customer ID: ";
                getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
                customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
            cout << ControlPanel::getControlPanel()->getCurrencies()[0]->getName();
            for(int i=1; i < ControlPanel::getControlPanel()->getCurrenciesSize(); i++)
                cout << ", " << ControlPanel::getControlPanel()->getCurrencies()[i]->getName();
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Currency Name (Ex: RON, EUR) = ";

            getInput(name, "Something went wrong! Make sure you insert a valid string.");
            currency= ControlPanel::getControlPanel()->getCurrency(name);

            while(currency == NULL){
                cout<<"Currency not found!\n";
                cout<<"Currency Name (Ex: RON, EUR) = ";
                getInput(name, "Something went wrong! Make sure you insert a valid string.");
                currency= ControlPanel::getControlPanel()->getCurrency(name);
            }

            cout<<"Should the account be Business? (yes/no): ";
            getInput(s, "Something went wrong! Make sure you insert a valid string.");
            while(s!="no" && s!="yes"){
                cout<<"Unknown option, yes or no.\n";
                cout<<"Should the account be Business? (yes/no): ";
                getInput(s, "Something went wrong! Make sure you insert a valid string.");
            }

            if(customer->createAccount(s == "yes", currency))
                cout<<"Account added!\n";
            else
                cout<<"Account could not be added!\n";

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case DeleteAccount:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCustomersSize() == 0){
                cout<<"No customers registered yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCustomers()[0]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[0] << ")";
                for(int i=1; i < ControlPanel::getControlPanel()->getCustomersSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCustomers()[i]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[i] << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Customer ID: ";
            getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
            customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            while(customer == NULL){
                cout<<"Customer not found!\n";
                cout<<"Customer ID: ";
                getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
                customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            }

            if(customer->getAccountsCount() == 0){
                cout<<"No accounts yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << customer->getAccounts()[0]->getAccountId() << " (Balance: " << customer->getAccounts()[0]->getCurrency()->fromUnit(
                        (double) *customer->getAccounts()[0]) << " " << customer->getAccounts()[0]->getCurrency()->getName() << ")";
                for(int i=1; i < customer->getAccountsCount(); i++)
                    cout << ", " << customer->getAccounts()[i]->getAccountId() << " (Balance: " << customer->getAccounts()[i]->getCurrency()->fromUnit(
                            (double) *customer->getAccounts()[i]) << " " << customer->getAccounts()[i]->getCurrency()->getName() << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Account ID: ";
            getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
            account= customer->getAccount(accountId);
            while(account == NULL){
                cout<<"Account not found!\n";
                cout<<"Account ID: ";
                getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
                account= customer->getAccount(accountId);
            }

            if(customer->deleteAccount(accountId))
                cout<<"Account deleted successfully!\n";
            else
                cout<<"Account could not be deleted!\n";

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case AddCard:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCustomersSize() == 0){
                cout<<"No customers registered yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCustomers()[0]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[0] << ")";
                for(int i=1; i < ControlPanel::getControlPanel()->getCustomersSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCustomers()[i]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[i] << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Customer ID: ";
            getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
            customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            while(customer == NULL){
                cout<<"Customer not found!\n";
                cout<<"Customer ID: ";
                getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
                customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            }

            if(customer->getAccountsCount() == 0){
                cout<<"No accounts yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << customer->getAccounts()[0]->getAccountId() << " (Balance: " << customer->getAccounts()[0]->getCurrency()->fromUnit(
                        (double) *customer->getAccounts()[0]) << " " << customer->getAccounts()[0]->getCurrency()->getName() << ")";
                for(int i=1; i < customer->getAccountsCount(); i++)
                    cout << ", " << customer->getAccounts()[i]->getAccountId() << " (Balance: " << customer->getAccounts()[i]->getCurrency()->fromUnit(
                            (double) *customer->getAccounts()[i]) << " " << customer->getAccounts()[i]->getCurrency()->getName() << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Account ID: ";
            getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
            account= customer->getAccount(accountId);
            while(account == NULL){
                cout<<"Account not found!\n";
                cout<<"Account ID: ";
                getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
                account= customer->getAccount(accountId);
            }

            cout<<"What should the card type be? (Virtual/Standard/Premium): ";
            getInput(s, "Something went wrong! Make sure you insert a valid string.");
            //without try catch option
            /*while(s!="Virtual" && s!="Standard" && s!="Premium"){
                cout<<"Unknown option, Virtual, Standard or Premium.\n";
                cout<<"What should the card type be? (Virtual/Standard/Premium): ";
                getInput(s, "Something went wrong! Make sure you insert a valid string.");
            }*/

            do {
                try {
                    if (account->addCard(s))
                        cout << "The card was added successfully!\n";
                    else
                        cout << "The card could not be created!\n";
                    validCard=true;
                } catch (string &) {
                    validCard=false;
                    cout<<"Unknown option, Virtual, Standard or Premium.\n";
                    cout<<"What should the card type be? (Virtual/Standard/Premium): ";
                    getInput(s, "Something went wrong! Make sure you insert a valid string.");
                }
            }
            while(!validCard);

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case DeleteCard:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCustomersSize() == 0){
                cout<<"No customers registered yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCustomers()[0]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[0] << ")";
                for(int i=1; i < ControlPanel::getControlPanel()->getCustomersSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCustomers()[i]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[i] << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Customer ID: ";
            getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
            customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            while(customer == NULL){
                cout<<"Customer not found!\n";
                cout<<"Customer ID: ";
                getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
                customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            }

            if(customer->getAccountsCount() == 0){
                cout<<"No accounts yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << customer->getAccounts()[0]->getAccountId() << " (Balance: " << customer->getAccounts()[0]->getCurrency()->fromUnit(
                        (double) *customer->getAccounts()[0]) << " " << customer->getAccounts()[0]->getCurrency()->getName() << ")";
                for(int i=1; i < customer->getAccountsCount(); i++)
                    cout << ", " << customer->getAccounts()[i]->getAccountId() << " (Balance: " << customer->getAccounts()[i]->getCurrency()->fromUnit(
                            (double) *customer->getAccounts()[i]) << " " << customer->getAccounts()[i]->getCurrency()->getName() << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Account ID: ";
            getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
            account= customer->getAccount(accountId);
            while(account == NULL){
                cout<<"Account not found!\n";
                cout<<"Account ID: ";
                getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
                account= customer->getAccount(accountId);
            }

            if(account->getNumberOfCards() == 0){
                cout<<"No cards yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << account->getCards()[0]->getCardNumber();
                for(int i=1; i < account->getNumberOfCards(); i++)
                    cout << ", " << account->getCards()[i]->getCardNumber();
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Card ID: ";
            getInput(cardId, "Something went wrong, make sure you are inserting an integer!");

            if(account->deleteCard(cardId))
                cout<<"Card was deleted successfully!\n";
            else
                cout<<"Couldn't delete card!\n";

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case SetBalance:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCustomersSize() == 0){
                cout<<"No customers registered yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCustomers()[0]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[0] << ")";
                for(int i=1; i < ControlPanel::getControlPanel()->getCustomersSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCustomers()[i]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[i] << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Customer ID: ";
            getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
            customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            while(customer == NULL){
                cout<<"Customer not found!\n";
                cout<<"Customer ID: ";
                getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
                customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            }

            if(customer->getAccountsCount() == 0){
                cout<<"No accounts yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << customer->getAccounts()[0]->getAccountId() << " (Balance: " << customer->getAccounts()[0]->getCurrency()->fromUnit(
                        (double) *customer->getAccounts()[0]) << " " << customer->getAccounts()[0]->getCurrency()->getName() << ")";
                for(int i=1; i < customer->getAccountsCount(); i++)
                    cout << ", " << customer->getAccounts()[i]->getAccountId() << " (Balance: " << customer->getAccounts()[i]->getCurrency()->fromUnit(
                            (double) *customer->getAccounts()[i]) << " " << customer->getAccounts()[i]->getCurrency()->getName() << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Account ID: ";
            getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
            account= customer->getAccount(accountId);
            while(account == NULL){
                cout<<"Account not found!\n";
                cout<<"Account ID: ";
                getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
                account= customer->getAccount(accountId);
            }

            cout << "Balance Curenta: " << account->getCurrency()->fromUnit(account->getBalance()) << " " << account->getCurrency()->getName() << endl;
            cout << "The amount " << account->getCurrency()->getName() << " you wish to set: ";

            getInput(sum, "Something went wrong! Make sure you insert a valid number.");

            account->setBalance(account->getCurrency()->toUnit(sum));
            cout<<"Balance was modified!\n";

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case BanUser:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCustomersSize() == 0){
                cout<<"No customers registered yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCustomers()[0]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[0] << " | " << (ControlPanel::getControlPanel()->getCustomers()[0]->isBanned() ? "Interzis" : "Neinterzis") << ")";
                for(int i=1; i < ControlPanel::getControlPanel()->getCustomersSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCustomers()[i]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[i] << " | " << (ControlPanel::getControlPanel()->getCustomers()[i]->isBanned() ? "Interzis" : "Neinterzis") << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            int toBan;
            cout<<"Customer ID to ban/unban: ";
            getInput(toBan, "Something went wrong, make sure you are inserting an integer!");
            customer= ControlPanel::getControlPanel()->getCustomer(toBan);
            while(customer == NULL){
                cout<<"Customer not found!\n";
                cout<<"Customer ID to ban/unban: ";
                getInput(toBan, "Something went wrong, make sure you are inserting an integer!");
                customer= ControlPanel::getControlPanel()->getCustomer(toBan);
            }

            if(customer->isBanned()){
                customer->setBanned(false);
                cout << "The customer " << customer->getLastName() << " " << customer->getFirstName() << " was unbanned!\n";
            }
            else{
                customer->setBanned(true);
                cout << "The customer " << customer->getLastName() << " " << customer->getFirstName() << " was banned!\n";
            }

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case AddUser:
            cout<<"\n";
            clearConsole();

            ControlPanel::getControlPanel()->createCustomer();

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case DeleteUser:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCustomersSize() == 0){
                cout<<"No customers registered yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCustomers()[0]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[0] << ")";
                for(int i=1; i < ControlPanel::getControlPanel()->getCustomersSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCustomers()[i]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[i] << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            int toRemove;
            cout<<"Customer ID to remove: ";
            getInput(toRemove, "Something went wrong, make sure you are inserting an integer!");

            if(toRemove == ControlPanel::getControlPanel()->getLogged()->getUserId()){
                cout<<"You can't remove yourself!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            if(ControlPanel::getControlPanel()->deleteCustomer(toRemove))
                cout<<"Customer removed successfully!\n";
            else
                cout<<"Couldn't remove customer!\n";

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case TransferCard:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCustomersSize() == 0){
                cout<<"No customers registered yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCustomers()[0]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[0] << ")";
                for(int i=1; i < ControlPanel::getControlPanel()->getCustomersSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCustomers()[i]->getUserId() << " (" << (string)*ControlPanel::getControlPanel()->getCustomers()[i] << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Customer ID: ";
            getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
            customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            while(customer == NULL){
                cout<<"Customer not found!\n";
                cout<<"Customer ID: ";
                getInput(customerId, "Something went wrong, make sure you are inserting an integer!");
                customer= ControlPanel::getControlPanel()->getCustomer(customerId);
            }

            if(customer->getAccountsCount() < 2){
                cout<<"Not enough accounts!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << customer->getAccounts()[0]->getAccountId() << " (Balance: " << customer->getAccounts()[0]->getCurrency()->fromUnit(
                        (double) *customer->getAccounts()[0]) << " " << customer->getAccounts()[0]->getCurrency()->getName() << ")";
                for(int i=1; i < customer->getAccountsCount(); i++)
                    cout << ", " << customer->getAccounts()[i]->getAccountId() << " (Balance: " << customer->getAccounts()[i]->getCurrency()->fromUnit(
                            (double) *customer->getAccounts()[i]) << " " << customer->getAccounts()[i]->getCurrency()->getName() << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Account ID to take card from: ";
            getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
            account= customer->getAccount(accountId);
            while(account == NULL){
                cout<<"Account not found!\n";
                cout<<"Account ID to take card from: ";
                getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
                account= customer->getAccount(accountId);
            }

            if(account->getNumberOfCards() == 0){
                cout<<"No cards yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << account->getCards()[0]->getCardNumber();
                for(int i=1; i < account->getNumberOfCards(); i++)
                    cout << ", " << account->getCards()[i]->getCardNumber();
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Card ID which should be transferred: ";
            getInput(cardId, "Something went wrong, make sure you are inserting an integer!");
            card=account->getCard(cardId);
            while(card==NULL){
                cout<<"Card not found!\n";
                cout<<"Card ID which should be transferred: ";
                getInput(cardId, "Something went wrong, make sure you are inserting an integer!");
                card=account->getCard(cardId);
            }

            cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << customer->getAccounts()[0]->getAccountId() << " (Balance: " << customer->getAccounts()[0]->getCurrency()->fromUnit(
                        (double) *customer->getAccounts()[0]) << " " << customer->getAccounts()[0]->getCurrency()->getName() << ")";
                for(int i=1; i < customer->getAccountsCount(); i++)
                    cout << ", " << customer->getAccounts()[i]->getAccountId() << " (Balance: " << customer->getAccounts()[i]->getCurrency()->fromUnit(
                            (double) *customer->getAccounts()[i]) << " " << customer->getAccounts()[i]->getCurrency()->getName() << ")";
            cout<<"\n──────────────────────────────────────────────\n";

            cout<<"Account ID to whom the card will be moved: ";
            getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
            account= customer->getAccount(accountId);
            while(account == NULL){
                cout<<"Account not found!\n";
                cout<<"Account ID to whom the card will be moved: ";
                getInput(accountId, "Something went wrong, make sure you are inserting an integer!");
                account= customer->getAccount(accountId);
            }

            if(shared_ptr<Account> cardAccount = card->getAccount().lock()) {
                if (account == cardAccount) {
                    cout << "The card can not be moved in the same account!\n";
                    cout << "\nPress anything to go back to the menu.\n";
                    _getch();
                    break;
                }
            }

            if(card->changeAccount(*account)){
                cout<<"The card was transferred successfully!\n";
            }
            else{
                cout<<"Couldn't transfer the card!\n";
            }

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case ShowCurrencies:
            cout<<"\n";
            clearConsole();

            if(ControlPanel::getControlPanel()->getCurrenciesSize() == 0){
                cout<<"No currencies yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }
            for(auto& valuta : ControlPanel::getControlPanel()->getCurrencies())
                cout<<*valuta;

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case AddCurrencies:
            cout<<"\n";
            clearConsole();

            ControlPanel::getControlPanel()->createCurrency();

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
        case DeleteCurrencies:
            cout<<"\n";
            clearConsole();

            cout<<"0 - Cancel\n";
            cout<<"1 - Delete currency by name (Ex: 'EUR')\n";
            cout<<"2 - Delete currency by ID\n";
            int suboption;
            cout<<"Option=";
            getInput(suboption, "Something went wrong, make sure you are inserting an integer!");
            while(suboption < 0 || suboption > 2){
                cout<<"Unknown option!\n";
                cout<<"Option=";
                getInput(suboption, "Something went wrong, make sure you are inserting an integer!");
            }
            if(suboption == 0){
                cout<<"Deletion cancelled.\n";
                break;
            }

            if(ControlPanel::getControlPanel()->getCurrenciesSize() == 0){
                cout<<"No currencies yet!\n";
                cout<<"\nPress anything to go back to the menu.\n";
                _getch();
                break;
            }

            if(suboption == 1){
                cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCurrencies()[0]->getName();
                for(int i=1; i < ControlPanel::getControlPanel()->getCurrenciesSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCurrencies()[i]->getName();
                cout<<"\n──────────────────────────────────────────────\n";

                cout<<"Currency name (Ex: RON, EUR) = ";
                string currencyName;

                getInput(currencyName, "Something went wrong! Make sure you insert a valid string.");
                currency= ControlPanel::getControlPanel()->getCurrency(currencyName);
                while(currency == NULL){
                    cout<<"Currency not found!\n";
                    cout<<"Currency name (Ex: RON, EUR) = ";

                    getInput(currencyName, "Something went wrong! Make sure you insert a valid string.");
                    currency= ControlPanel::getControlPanel()->getCurrency(currencyName);
                }
            }
            else if(suboption == 2){
                cout<<"\n──────────── Input Recommendation ────────────\n";
                cout << ControlPanel::getControlPanel()->getCurrencies()[0]->getCurrencyId();
                for(int i=1; i < ControlPanel::getControlPanel()->getCurrenciesSize(); i++)
                    cout << ", " << ControlPanel::getControlPanel()->getCurrencies()[i]->getCurrencyId();
                cout<<"\n──────────────────────────────────────────────\n";


                cout<<"Currency ID = ";
                int id;

                getInput(id, "Something went wrong, make sure you are inserting an integer!");
                currency= ControlPanel::getControlPanel()->getCurrency(id);
                while(currency == NULL){
                    cout<<"Currency not found!\n";
                    cout<<"Currency ID = ";

                    getInput(id, "Something went wrong, make sure you are inserting an integer!");
                    currency= ControlPanel::getControlPanel()->getCurrency(id);
                }
            }

            if(ControlPanel::getControlPanel()->deleteCurrency(currency->getName()))
                cout << "Currency " << currency->getName() << " was deleted!\n";
            else
                cout<<"Couldn't delete currency!\n";

            cout<<"\nPress anything to go back to the menu.\n";
            _getch();
            break;
    }
}
