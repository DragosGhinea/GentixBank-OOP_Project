#pragma once

class Currency{
    const int currencyId;
    const std::string name;
    double unitValue; //used for exchange rate

    static int generateCurrencyId(){
        static int id=0;
        id++;
        return id;
    }
public:
    Currency(const std::string& nameToSet, const double value) : currencyId(generateCurrencyId()), name(nameToSet), unitValue(value) {};

    double fromUnit(const double unit);
    double toUnit(const double currency);
    int getCurrencyId() {return currencyId;}
    std::string getName() {return name;}

    operator std::string() {return name;}

    friend std::ostream& operator<<(std::ostream& out, const Currency& v);

    Currency& operator=(Currency& v){ unitValue=v.unitValue; return *this;}
};

std::ostream& operator<<(std::ostream& out, const Currency& v);