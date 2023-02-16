#pragma once
#include<iostream>
#include<vector>
#include<memory>
#include "account.h"

class Card;

class User{

private:

    static int generateId(){
        static int id=0;
        id++;
        return id;
    }

protected:
    bool banned=false;
    const int userId;
    std::string last_name;
    std::string first_name;
    std::string email;

    virtual void extraInformation(std::ostream& out, User* whoRequests) const{std::cout << "Default";};

public:

    int getUserId(){return userId;}
    std::string getLastName() const {return last_name;}
    std::string getFirstName() const {return first_name;}
    std::string getEmail() const {return email;}
    void setEmail(const std::string mail){email=mail;};

    User(std::string last_name1="Unspecified", std::string first_name1="Unspecified", std::string email="Unspecified");
    virtual ~User(){};
    User(const User& u);

    bool setBanned(bool banned1);
    bool isBanned() const {return banned;}

    operator std::string() const {return last_name + " " + first_name;}
    friend std::istream& operator>>(std::istream& in, User& u);
    friend std::ostream& operator<<(std::ostream& out, const User& u);

    virtual bool rootAcces() const=0;
    virtual bool moderatorAcces() const=0;
    virtual bool adminAcces() const=0;
    virtual std::string getType() const=0;

    virtual User* Clone()=0;

    void display(std::ostream& out= std::cout, User* whoRequests= NULL) const;
    virtual void edit(std::istream& in);
    User& operator=(const User& u);
};

//user -> customer and employee
//employee and customer -> administrator, moderator

class Root: public User{
    std::string password;

    Root(std::string pass) : password(pass){};

public:
    bool rootAcces() const{
        return true;
    }

    bool moderatorAcces() const{
        return true;
    }

    bool adminAcces() const{
        return true;
    }

    std::string getType() const{
        return "Root";
    }

    virtual Root* Clone(){
        return nullptr;
    }

    std::string getPassword(){
        return password;
    }

    friend class ControlPanel;
};

class Customer : virtual public User{
    std::vector<std::shared_ptr<Account>> accounts;

    protected:
        void extraInformation(std::ostream& out, User* whoRequests) const;

    public:

        std::shared_ptr<Account> getAccount(int accountId);
        bool createAccount(bool business = false, Currency* currency= NULL);
        bool deleteAccount();
        bool deleteAccount(int accountId);

        Card* getCard(int cardId);

        int getAccountsCount() const{return accounts.size();};
        int getCardsCount() const;
        std::vector<std::shared_ptr<Account>> getAccounts() const {return accounts;};


        bool rootAcces() const {
            return false;
        }

        bool moderatorAcces() const{
            return false;
        }

        bool adminAcces() const{
            return false;
        }

        std::string getType() const{
            return "Customer";
        }

        virtual Customer* Clone(){
            return new Customer(*this);
        }

        Customer(const Customer& customer);

        Customer& operator=(const Customer& customer);

        Customer(std::string last_name1="Unspecified", std::string first_name1="Unspecified", std::string email="Unspecified")
        : User(last_name1, first_name1, email){}

};

class Employee : virtual public User{

protected:
    double salary; //in ron
    double salaryBonus=0; //in ron
    int warnings=0;
    std::string password2Factor;

    Employee(std::string pass, double sal=0) : salary(sal), password2Factor(pass){};

public:

    double getSalary() const{
        return salary;
    }

    void setSalary(double sal){
        salary=sal;
    }

    double getBonus() const{
        return salaryBonus;
    }

    void setBonus(double bon){
        salaryBonus=bon;
    }

    int getWarnings() const{
        return warnings;
    }

    void addWarn(){
        warnings++;
    }

    void removeWarn(){
        warnings--;
    }

    std::string getPassword2Factor(){
        return password2Factor;
    }

    void setPassword2Factor(std::string pass){
        password2Factor=pass;
    }

    virtual ~Employee(){};
    void edit(std::istream& in);
};

class Administrator: public Customer, public Employee{

    void extraInformation(std::ostream& out, User* whoRequests) const;

public:
    bool rootAccess() const{
        return false;
    }

    bool moderatorAcces() const{
        return true;
    }

    bool adminAcces() const{
        return true;
    }

    std::string getType() const{
        return "Administrator";
    }

    virtual Administrator* Clone(){
        return new Administrator(*this);
    }

    Administrator(std::string password2Factor, std::string last_name="Unspecified", std::string first_name="Unspecified", std::string email="Unspecified", double salary1=0)
    : User(last_name, first_name, email), Customer(last_name, first_name, email), Employee(password2Factor, salary1){};

    ~Administrator(){};

};

class Moderator: public Customer, public Employee{

    void extraInformation(std::ostream& out, User* whoRequests) const;

public:
    bool rootAccess() const{
        return false;
    }

    bool moderatorAcces() const{
        return true;
    }

    bool adminAcces() const{
        return false;
    }

    std::string getType() const{
        return "Moderator";
    }

    virtual Moderator* Clone(){
        return new Moderator(*this);
    }

    Moderator(std::string pass2Factor, std::string last_name="Unspecified", std::string first_name="Unspecified", std::string email="Unspecified", double salary=0)
    : User(last_name, first_name, email), Customer(last_name, first_name, email), Employee(pass2Factor, salary){};

    ~Moderator(){};
};


std::istream& operator>>(std::istream& in, User& u);
std::ostream& operator<<(std::ostream& out, const User& u);

class UserFactory{
public:
    static Customer StandardCustomer() {return Customer("New", "Customer", "email@domeniu.com");}
    static Moderator StandardModerator() {return Moderator("moderatorStandard1", "New", "Moderator", "mod@gmail.com", 3000);}
    static Administrator StandardAdministrator() {return Administrator("adminStandard!", "New", "Admin", "admin@gmail.com", 5000);}
};
