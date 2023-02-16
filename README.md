
# Gentix Bank | OOP Project

![FirstPage](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/FirstPage.png)

A project made in C++ using CLion that contains some OOP concepts.

The project is a bank managing system which offers features for multiple type of users (customers, moderators, administrators, root). The features are implemented in a command line menu which is designed in a testing way, providing access to all users from it (not really practical as a real application, is it?).

While the first screen is pretending to load using a nice progress bar animation, the ControlPanel generates random data for program testing. It will generate different data every run. It creates customers, moderators and administrators to which it assigns random accounts and cards with random transactions and balances.

![MainMenu](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/MainMenu.png)

In the main menu:
 - You can randomly get a customer for whom to experience the user menu.
 - You can randomly get a moderator/administrator for whom to experience the user menu. On Management Panel setting you will need a password. Moderator Password: **moderator2** | Admin Password: **amGoodAdmin1**
 - The Root Login requires a password right from the beginning (**unBreakable1**)
 - The Leaderboard option displays a top of all the accounts, organized based on balance to show the functionality of the Leaderboard template class.
 
**For more menu images scroll down after the General Overview.**

## General Overview
The project contains the following concepts which will be explained:
   - Class composition
   - Constructors (Initialization & Copy)
   - Destructors (Simple & Virtual)
   - Operator overloading
   - Constant class members
   - Class inheritance
   - Smart pointers (unique, shared & weak)
   - Polymorphism
   - Exception handling
   - STL objects
   - Templates
   - Design Patterns (singleton, factory, builder, decorator)

### Class Composition

Class composition refers to the creation of a new class which combines (composes) existing classes in it's member variables.

Examples inside this project:
  - **Account class** (found in account.h) which contains members variables of type User _(owner)_, Currency _(usedCurrency)_, Card _(cards)_.
  - **Card class** (found in card.h) which contains Account _(account)_.
  - **Customer class** (found in user.h) which contains Account _(accounts)_.
  - **Transaction and TransactionHistory classes** (found in transactions.h), both containing a member of Account type and a member of eachother's type.

### Constructors
Used to create (construct) instances of a specific class, they exist by default but can be overwritten and have multiple parameters. You can not have a pointer to them nor can they be inherited.

Some **Initialization Constructors** in this project:
  - **Currency(nameToSet, value)** | found in currency.h | is the only user defined constructor of the Currency class, which overrides the default constructor, therefore creating an instance with no parameters does not work. (It also has a default copy constructor)
    * `Currency c;` does not exist
    * `Currency c("EUR", 4);` works
  - **Transaction(historic1, date, value, deposit1, transfer1, account1)** | found in transactions.h | is the initialization constructor, a user defined copy constructor exists besides it and same as above, you can not create an instance with no parameters given.
  - **Root(pass)** | found in user.h | is declared in the private zone, meaning we don't want just anyone creating instances of this type. You'll notice that we have declared a **friend class** ControlPanel, therefore ControlPanel can create instances of Root.

Some **Copy Constructors** in this project:
  - **Transaction(const Transaction& t)** | found in transactions.h | is a user defined copy constructor which receives a const parameter due to the fact that when copying, we do not intend to modify the object that is copied.
  - Like the one above we have multiple copy constructors for classes like Account, User, TransactionHistory.
  - We have default copy constructors for classes like Card, Currency

We define copy constructors ourselves when we want to change the way members are copied (for some in the above classes we need to use **a deep copy** mechanism, more details in the polymorphism section), otherwise we just keep the default one. For example if we have pointers to some objects we might want to copy the object it points to rather than copying the pointer itself which results in having the two instances use the same object as member.

The constructors also can have **an initialization list** through which we can call the constructor for base classes in inheritance or member variables.

```c++
  //initialization list
  Currency(const std::string& nameToSet, const double value)
   : currencyId(generateCurrencyId()), name(nameToSet), unitValue(value) {};

  //without initialization list
  Currency(const std::string& nameToSet, const double value) {
      this->currencyId=generateCurrencyId();
      this->name=nameToSet;
      this->unitValue=value;
  };

  //both initialization list and separate assignation
  User::User(string last_name1, string first_name1, string email)
   : userId(generateId()){
    this->last_name=last_name1;
    this->first_name=first_name1;
    this->email=email;
  };
```

- In the above example the initialization list will call the copy constructor of each variabile that is in it. **One operation** will take place.
- In the constructor without the initialization list, where the variables are set inside the constructor's body, **two operations** will take place. Firstly the **initialization constructor** will assign a random/empty value to each variable, then **operator=** is called for each of them.

**Important:** Using inheritance, if you declare a copy constructor in a **derived class**, it will call by default the initialization constructor of the **base class**!

### Destructors

There is a default destructor but same as above, you can override it when you need to change the way member variables are deleted, or you might want to trigger an event on an object deletion.

By **simple destructors** I am referring to the non-virtual ones. A **virtual destructor** is needed in class inheritance to cascade the destruction of a derived object through the base class (more details in the polymorphism section).

Some **destructors** in this project:

```c++
//transactions.h

//this destructor has it's inside commented
//because the class now uses smart pointers
//but with raw pointers that code is required

TransactionsHistory::~TransactionsHistory(){
    /*for(int i=0;i<100;i++){
        if(transactions[i]==NULL)
            break;
        delete transactions[i];
    }
    delete[] transactions;*/
}
```

```c++
//card.h

//a virtual destructor
//with the default settings

//it needs to be virtual so if a call
//on a Card occurs, if it's an instance of a
//derived class, everything will be properly removed.

virtual ~Card()=default;
```

### Operator Overloading

We overload the operators for specific classes so working with them feels more natural.

Examples:
 - `Account += double` should increase the account's balance.
 - `double - Account` should return double - account's balance.
 - `(double) Account` should cast the Account to it's balance.
 - `(string) Currency` should cast the Currency to it's name.
 - `Leaderboard += OB` adds the OB to it's internal list.
 - `Currency1 = Currency2` sets Currency1's unit value to Currency2's unit value without changing the name
 - `OutputStream << Account/Card/Transaction/TransactionHistory/Currency` displays proper information about the objects

We can overload them as methods, where the first argument of the operation is always the object that it is defined in.

Sometimes, we need to reverse the order so we have to overload the operators outside the class. For example the `<<` operator, if declared as method, will work as `object<<cout` which is counterintuitive, therefore we declare it externally. Same for `-`,`+` operations, `object+double` for example works fine as method, but you will most likely need `double+object` too which needs to be declared externally.

### Constant Class Members

We use **const** to declare constant variables and methods which should not be modifiable. Also required if we are going to work with rvalues and we want to tell the program that we are not going to try to modify it.

A declared **const variable** can not be assigned, it can only be created via a constructor. In the code below we have a const userId which is created via the copy constructor triggered by the initialization list.

```c++
const int userId;

User::User(string last_name1, string first_name1, string email)
 : userId(generateId()){
    this->last_name=last_name1;
    this->first_name=first_name1;
    this->email=email;
  };
```

A **const method** guarantees that **the variables of the instance it belongs to** will not be modified inside it. If you want to protect it's parameters, you'd have to declare them const separately. Good for getters mostly.

```c++
//card.h

//these functions guarantee cardNumber, cvv, expireDate variables
//will not be modified by the method call

int getCardNumber() const {return cardNumber;};
int getCVV() const {return cvv;};
long getExpireDate() const {return expireDate;};
```

### Class Inheritance

Through inheritance we add code reusability and polmyorphism to the program.

The class inheritance scheme is displayed in the loading page of the command line but we will repeat it here as well.

- Account = **base class**
- AccountBusiness = derived class of Account

* Card = **base class**
* CardStandard = derived class of Card
* CardPremium = derived class of CardStandard, indirectly of Card

- User = **abstract class** (contains pure virtual methods) which means you can not create instances of this specific type and you have to use a derived class.
- Customer = derived class of User
- Employee = derived class of User
- Root = derived class of User
- Moderator = derived class of Customer and Employee **(diamond problem)**
- Administrator = derived class of Customer and Employee **(diamond problem)**

The **diamond problem** that arises for Moderator and Administrator classes is as follows: We have our object (Moderator or Administrator type) that inherits a Customer object and an Employee object, each of them having their own User object which will result in having **duplicate variables and actions** that are User related inside the Moderator/Administrator object. To avoid such an inconvenience we must virtually inherit the Customer and Employee classes, such that their common ancestor will be shared and the duplication disappears.

### Smart pointers

Smart pointers are used to automatically manage memory based on ownership of an object and make it clear for the programmer who owns whom.

This project does not fully integrate smart pointers, but has some usage examples of the **unique, shared and weak pointers**.

In this project **unique pointers** are used for the sole purpouse of avoiding defining destructors for the dynamic linked objects. Examples:
 - Account class (found in account.h) owns a TransactionHistory object and multiple Card objects which will be deleted with the Account instance.
 - TransactionHistory class (found in transactions.h) owns multiple Transaction objects which will be deleted with the TransactionHistory instance.

 In this project **shared pointers** are used in combination with **weak pointers** to ensure that even a removed object will be kept alive as long as it is actively used somewhere else.
 - Customer class (found in user.h) owns multiple Account objects, but on Customer deletion, if the Account object is used somewhere else, it will not be deleted.
 - getAccount() method from Customer provides ownership to whoever is calling it.
 - Cards and Transactions have a **weak ownership** of the Account object, meaning they don't interact with it's destruction but can detect it and temporaly prevent it by generating a new **shared ownership instance** tbat is used inside some methods and menus.

How are the above shared and weak ownerships useful? Imagine you create a copy of a card to use privately which will point to an Account that does not have this copy inside it's cards list. If we want to execute actions on this card that involve the Account, we will check it's existance via the weak pointer. If it doesn't exist we display an error message, otherwise we grab temporary shared ownership of the account to execute our action. We grab temporary shared ownership to prevent the **undefined behaviour** that would happen if the Account gets deleted while we execute our action (since temporary shared ownership means the object can't be destroyed).

```c++
//card.cpp

//if the card's account exists we take temporary ownership
//to make sure we can do the deposit
//othwrsie, we show an error

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
```

In the code below we use the weak pointer `usedAccount` to check if the transaction's account still exists, if not we show a proper message.
```c++
//transactions.cpp

ostream& operator<<(ostream& out, const Transaction& t){
  //...
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
  //...
}
```

We also declare the Account class as `class Account : public std::enable_shared_from_this<Account>` so we can get the shared pointer reference from inside the class via `this->shared_from_this()`.

**Mention**: Since the project does not fully use smart pointers and we delete objects when removing them from a list, it can cause **undefined behaviour** of working with a reference to a deleted object. So we will assume an optimistic approach, in which concurrency does not happen, an object can't get deleted while am trying to use it and so on.

### Polymorphism

- **Compile-time polymorphism** appears through early binding and it reffers to the overloading of constructors/functions/templates/etc.
- **Run-time polymorphism** appears through late binding and it reffers to virtual methods and method overriding, which is done at runtime.

**Compile-time polymorphism examples:**
```c++
//account.h
bool bankTransfer(double value, std::shared_ptr<Account> &account, Currency* currency = NULL);
bool bankTransfer(double value, Card &card, Currency* currency = NULL);

//inputHandle.h
template <typename T> void getInput(T &toRead, std::string errorMessage);
template <typename T> void getInput(T &toRead, std::string errorMessage, int maxSize);
```

**Run-time polymorphism examples:**
```c++
//user.h
//pure virtual functions
virtual bool rootAcces() const=0;
virtual bool moderatorAcces() const=0;
virtual bool adminAcces() const=0;
virtual std::string getType() const=0;
virtual User* Clone()=0;

//card.h
virtual ~Card()=default;

//controlPanel.cpp
bool ControlPanel::loadRandomData(){
  //...
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
  //...
}
```

**Important**: we have to keep in mind using pointers and references when working with polymorphic classes to avoid **object slicing**. An object will always be sliced if it's class does not contain a virtual method/destructor. Inside constructors and destructors we can not use **late binding** (meaning the virtual methods will use the definition inside their class, not the one set by the virtual instance).

**Constructors can't use late binding** because they are the first thing to be created, from base to child, so the child's methods are not constructed yet. 

**Destructors can't use late binding** because they might use a method that was already deleted.

Due to the fact copy constructors can't do late binding, we can't directly copy polymorphic classes so we have to improvise a **deep copy** by creating a virtual Clone() method which calls the copy constructor of the proper instance.

### Exception Handling

We handle exceptions via `try{}catch(...){}` blocks. A try block can have multiple catch blocks for different data types. The blocks are tried in order but no implicit conversion will be made, **exception for upcast**. A derived class needs to be put before it's base. After the catch block is finished, the code continues right after the `try{}catch{}`. It will not go back in the try block.

Example in this project:

```c++
//account.cpp

//throwing error example
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

//menuUser.cpp

//and catching it inside the menu
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
```

In the code above, if account->addCard throws a string exception it will be catched and the try block will never reach the validCard=true instruction. It will set validCard to false in the catch block and retry reading s.

### STL Objects

STL (Standard Template Library) is a collection of data structures and algorithms that make programming in C++ easier.

In this project we use vector<T> for most object collections and sort() for the Leaderboard template.

Taking inspiration from the priority_queue class which implements customizable sort, the Leaderboard template implements customizable object copy.

### Templates

A template is a generalized version of a function/class which lets you use the same code for different data types.

```c++
//inputHandle.h

//these template functions allow you to read input
//of a type you don't know
template <typename T> void getInput(T &toRead, std::string errorMessage);
template <typename T> void getInput(T &toRead, std::string errorMessage, int maxSize);
```

The Leaderboard template class takes three template arguments `template<typename OB, typename Cast, typename COPY_F = artificialCopy<OB>>
class Leaderboard`.
 - The first argument OB is the data type that will be stored inside the Leaderboard instance
 - The second argument Cast is the data type that OB will be casted to so it can compare the objects
 - The third argument COPY_F is a functor which copies the object. The default value of this argument is a functor that applies the default copy constructor. In this project this argument is replaced with a custom functor that would do a deep copy for us, therefore preventing object slicing.

```c++
//default value for the third argument
//leaderboard.h
template<typename OB>
struct artificialCopy{

    OB* operator()(const OB& toCopy){
        return new OB(toCopy);
    }

};

//custom deep copy for third argument
//menuLeaderboard.h
struct customCopy{
    Account* operator()(Account& cont){
        return cont.Clone();
    }
};
```

### Design Patterns

This project contains four design patterns (more or less used...):
 - Singleton (controlPanel.h)
 - Factory (user.h)
 - Builder (account.h)
 - Decorator (card.h)

A singleton is a class which should only have one instance throughout the program's life. It can't be copied, assigned or initialized outside itself. It initializes itself via a static method getInstance(), in this case getControlPanel().

A factory is a class that offers implementations that are commonly used/required and are usually a pain to initialize (for example, the constructor has a lot of parameters). In this project, we have a user factory that generates default users of each type.

A builder is a class that gradually builds an instance of another class by specifing each property individually. In this project we have an AccountBuilder which can gradually build an Account.

A decorator is a class which allows you to convert an object into a different format. In this project's case, we can convert a card into it's JSON format or CSV format.

## Command Line Images

Customer User Interface:

![CustomerUserInterface](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/CustomerUserInterface.png)

Show Accounts Option:

![CustomerUserInterface](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/CustomerUserInterfaceAccounts1.png)
![CustomerUserInterface](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/CustomerUserInterfaceAccounts2.png)

Show Cards Option:

![CustomerUserInterface](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/CustomerUserInterfaceCards.png)

Root User Interface:

![RootUserInterface](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/RootUserInterfaceAccounts.png)

Root User Interface Management Panel:

![RootUserInterfacePanel](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/RootUserInterfaceManagementPanel.png)

Leaderboard Main Menu Option:

![Leaderboard1](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/Leaderboard1.png)
![Leaderboard2](https://github.com/DragosGhinea/GentixBank-OOP_Project/blob/main/images/Leaderboard2.png)

## Aditional Information

- This project was initially created in romanian and translated afterwards. The project might not have it's variables fully translated! (Sorry for that)

### Bibliography

- https://github.com/mcmarius/poo/tree/refacere-materiale/tema-3
- https://en.cppreference.com/w/
- **Object Oriented Programming Course** by Anca Dobrovăț & Andrei Păun, University of Bucharest, Faculty of Mathematics and Computer Science