#include<iostream>
#include "../include/currency.h"
using namespace std;

ostream& operator<<(ostream& out, const Currency& v){
    out<<"────────────────┤ Currency Info ├────────────────\n";
    out << " • ID: " << v.currencyId << endl;
    out << " • Name: "+v.name << endl;
    out << " • 1 Unit = " << v.unitValue << " RON" << endl;
    out<<"───────────────────────────────────────────────\n\n";
    return out;
}

double Currency::fromUnit(const double unit){
    return unit / unitValue;
}

double Currency::toUnit(const double currency){
    return currency * unitValue;
}