#include <iostream>
#include <string>

using namespace std;

class Grocery
{
    string name = "", code;
    double price;
    string dept;

public:
    Grocery() {}

    Grocery(string name, string code, double price, string dept)
        : name(name), code(code), price(stod(round(price))), dept(dept) {} 

    string round(double number) //function i made and used in a4,
    {
        //string round:
        //  takes in a double and returns a string version of the double that
        //  only has decimals that it needs
        int decimal = (number * 100) - ((int)number * 100);
        if (decimal > 10)
        {
            return to_string((int)number) + "." + to_string(decimal);
        }
        else
        {
            return to_string((int)number) + ".0" + to_string(decimal);
        }
    }

    string groc_str() { return name + ", " + code + ", " + round(price) + ", " + dept; }
    string get_name() { return name; }
    void set_name(string &n) { name = n; }
    string get_dept() { return dept; }
    void set_dept(string &d) { dept = d; }
    double get_price() { return price; }
    void set_price(double &p) { price = p; }
    string get_code() { return code; }
    void set_code(string &c) { code = c; }
};
