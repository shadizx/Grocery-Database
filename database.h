#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cassert>
#include <sstream>
#include "grocery.h"

using namespace std;

class Database
{
private:
    int size = 0, cap = 10;
    Grocery *data = nullptr;
    const int product_code_threshold = 9999;
    const int product_price_threshold = 1000;

public:
    bool failed = false;
    Database() //constructor
    {
        data = new Grocery[cap];
        if (file_exists("database.txt") == true)
        {
            ifstream file("database.txt");
            if (!is_empty(file))
            {
                string line;
                if (file.is_open())
                {
                    while (getline(file, line))
                    {
                        Grocery g = groc_from_file(line);
                        if (failed == true)
                        {
                            cout << "Error:\nFile has invalid data\n";
                            break;
                        }
                        else
                            append(g);
                    }
                }
            }
            file.close();
        }
        else
        {
            ofstream file("database.txt");
            file.close();
        }
    }

    ~Database() // destructor
    {
        delete[] data;
    }

    bool isempty() { return size == 0; }

    void update()
    {
        ofstream file("database.txt");
        for (int i = 0; i < size; i++)
        {
            if (i != size - 1)
                file << data[i].groc_str() << endl;
            else
                file << data[i].groc_str();
        }
        file.close();
    }

    // ------------------making grocery item from file---------------------------------------------------
    Grocery groc_from_file(string line)
    {
        bool checkinput = true;
        double price = 0;

        int pos = line.find_first_of(",");          // gets string from beginning of line to fist comma
        string name = line.substr(0, pos);          // getting name
        line = line.substr(pos + 2, line.length()); // subtracting name from the line

        if (!isalpha(name))
        {
            checkinput = false;
        }

        pos = line.find_first_of(",");              // moving pos to next comma
        string code = line.substr(0, pos);          // getting code
        line = line.substr(pos + 2, line.length()); // subtracting code from the line

        if (!is_positive_int(code))
        {
            checkinput = false;
        }

        pos = line.find_first_of(",");              // moving pos to next comma
        string pricecheck = line.substr(0, pos);    // getting price
        line = line.substr(pos + 2, line.length()); // subtracting price from the line

        if (is_positive_double(pricecheck))
        {
            price = stod(pricecheck);
        }
        else
            checkinput = false;

        string dept = line.substr(0, line.length()); // getting dept

        if (checkinput == false)
        {
            failed = true;
        }
        else
            failed = false;
        Grocery g(name, code, price, dept);
        return g;
    }
    // -----------------------------------------------------------------------------------------------

    void append(Grocery g) //append grocery to data array
    {
        //appending grocery to data array
        size++;
        if (cap < size)
        {
            cap *= 2;
        }

        int newsize = size;
        Grocery *newgroc = new Grocery[cap];

        for (int i = 0; i < size - 1; i++)
        {
            newgroc[i] = data[i];
        }

        delete[] data;

        data = newgroc;
        size = newsize;
        data[newsize - 1] = g;
    }
    // -----------------------------------------------------------------------------------------------
    string namecheck()
    {
        string n;

        cout << "Enter item name: ";
        getline(cin >> ws, n);

        while (!isalpha(n))
        {
            cout << "Invalid name, try again: ";
            getline(cin >> ws, n);
        }
        return n;
    }

    string codecheck()
    {
        string c;

        cout << "\nEnter item code: ";
        cin >> c;

        while (!is_positive_int(c) || c.find_first_not_of("1234567890") != std::string::npos)
        {
            cout << "Invalid code, try again: ";
            cin >> c;
        }
        return c;
    }

    double pricecheck()
    {
        string price;
        double numprice;

        cout << "\nEnter item price: ";
        cin >> price;

        while (!is_positive_double(price))
        {
            cout << "Invalid price, try again: ";
            cin >> price;
        }
        numprice = stod(price);

        return numprice;
    }

    string deptcheck()
    {
        string d;

        cout << "\nEnter item department: ";
        cin >> d;
        while (!isalpha(d))
        {
            cout << "Invalid department, try again: ";
            cin >> d;
        }
        return d;
    }

    // ------------------finding----------------------------------------------------------------------
    //using linear search
    string findbyname(string s, string c)
    {
        string full = "";
        for (int i = 0; i < size; i++)
        {
            if (c == "e") //if searching for exact name
            {
                if (s == data[i].get_name())
                {
                    full += data[i].groc_str() + "\n";
                }
            }
            else // if searching for susbstring in name
            {
                if (data[i].get_name().find(s) != std::string::npos)
                {
                    full += data[i].groc_str() + "\n";
                }
            }
        }
        return full;
    }

    string findbycode(string s, string c)
    {
        string full = "";
        for (int i = 0; i < size; i++)
        {
            if (c == "e") //if searching for exact code
            {
                if (s == data[i].get_code())
                {
                    full += data[i].groc_str() + "\n";
                }
            }
            else // if searching for susbstring in code
            {
                if (data[i].get_code().find(s) != std::string::npos)
                {
                    full += data[i].groc_str() + "\n";
                }
            }
        }
        return full;
    }

    string findbyprice(double price, double p1, double p2)
    {
        string full = "";
        for (int i = 0; i < size; i++)
        {
            if (price > -1) //if looking for exact price
            {
                if (price == data[i].get_price())
                {
                    full += data[i].groc_str() + "\n";
                }
            }
            else //if looking for range of prices
            {
                if (data[i].get_price() >= p1 && data[i].get_price() <= p2)
                {
                    full += data[i].groc_str() + "\n";
                }
            }
        }
        return full;
    }

    string findbydept(string s, string c)
    {
        string full = "";
        for (int i = 0; i < size; i++)
        {
            if (c == "e") //if searching for exact dept
            {
                if (s == data[i].get_dept())
                {
                    full += data[i].groc_str() + "\n";
                }
            }
            else // if searching for susbstring in dept
            {
                if (data[i].get_dept().find(s) != std::string::npos)
                {
                    full += data[i].groc_str() + "\n";
                }
            }
        }
        return full;
    }
    //------------------------------------------------------------------------------------------------

    // ------------------sorting----------------------------------------------------------------------
    //sorting via insertion sort
    //https://www.geeksforgeeks.org/insertion-sort/
    void namesortalpha()
    {
        for (int i = 0; i < size; i++)
        {
            Grocery key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j].get_name() > key.get_name())
            {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }

    void namesortralpha()
    {
        for (int i = 0; i < size; i++)
        {
            Grocery key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j].get_name() < key.get_name())
            {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }

    void codesortasc()
    {
        for (int i = 0; i < size; i++)
        {
            Grocery key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j].get_code() > key.get_code())
            {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }

    void codesortdesc()
    {
        for (int i = 0; i < size; i++)
        {
            Grocery key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j].get_code() < key.get_code())
            {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }

    void pricesortasc()
    {
        for (int i = 0; i < size; i++)
        {
            Grocery key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j].get_price() > key.get_price())
            {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }

    void pricesortdesc()
    {
        for (int i = 0; i < size; i++)
        {
            Grocery key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j].get_price() < key.get_price())
            {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }

    void deptsortalpha()
    {
        for (int i = 0; i < size; i++)
        {
            Grocery key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j].get_dept() > key.get_dept())
            {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }

    void deptsortralpha()
    {
        for (int i = 0; i < size; i++)
        {
            Grocery key = data[i];
            int j = i - 1;
            while (j >= 0 && data[j].get_dept() < key.get_dept())
            {
                data[j + 1] = data[j];
                j--;
            }
            data[j + 1] = key;
        }
    }
    // -----------------------------------------------------------------------------------------------
    //deleting everything in the Database
    void wipe()
    {
        if (confirm_change() == true)
        {
            Grocery *new_data = new Grocery[cap];
            delete[] data;
            data = new_data;
            size = 0;
            cout << "Database has been cleared." << endl;
        }
        else
        {
            cout << "No changes were made." << endl;
        }
    }
    // ------------------listing database--------------------------------------------------------------
    void listdb()
    {
        cout << "\nName\tCode\tPrice\tDep\n"
             << "----------------------------\n";

        for (int i = 0; i < size; i++)
        {
            cout << data[i].groc_str() << endl;
        }
    }

    //checking if the database.txt is empty
    bool is_empty(std::ifstream &pFile) { return pFile.peek() == std::ifstream::traits_type::eof(); }

    // ------------------removings----------------------------------------------------------------------
    //remove item with item name as input
    string removebyname(string s, string c)
    {
        string found = "", choice;
        int index_location;
        int arrsize = 0;
        for (int i = 0; i < size; i++)
        {
            if (c == "e") //if searching for exact name
            {
                if (data[i].get_name() == s)
                {
                    index_location = i;
                    arrsize++;
                    found += data[i].groc_str() + "\n";
                }
            }
            else // if searching for susbstring in name
            {
                if (data[i].get_name().find(s) != std::string::npos)
                {
                    index_location = i;
                    arrsize++;
                    found += data[i].groc_str() + "\n";
                }
            }
        }
        if (arrsize == 0) // if nothing is found
        {
            return "data does not exist\n";
        }
        if (arrsize == 1) //if only one word is found
        {
            while (true)
            {
                cout << "Item to be removed is " << found
                     << "\nAre you sure you want to remove this item? (Y/N) ";
                cin >> choice;
                lowerstring(choice);
                if (choice == "y")
                    break;
                else if (choice == "n")
                    return "";
                else
                {
                    cout << "Please enter a valid option\n";
                }
            }
            data[index_location] = data[size - 1];
            size--;
            return "Item successfully removed\n";
        }
        // if more than one item is found
        while (true)
        {
            cout << "Items to be removed are:\n"
                 << found
                 << "\nAre you sure you want to remove these items? (Y/N) ";
            cin >> choice;
            lowerstring(choice);
            if (choice == "y")
                break;
            else if (choice == "n")
                return "";
            else
            {
                cout << "Please enter a valid option\n\n";
            }
        }

        int n = 0;
        while (n < size)
        {
            if (c == "e")
            {
                if (data[n].get_name() == s)
                {
                    size--;
                    data[n] = data[size];
                    n = 0;
                }
                else
                    n++;
            }
            else
            {
                if (data[n].get_name().find(s) != std::string::npos)
                {
                    size--;
                    data[n] = data[size];
                    n = 0;
                }
                else
                    n++;
            }
        }
        return "Items successfully removed";
    }

    string removebycode(string s, string c)
    {
        string found = "", choice;
        int index_location;
        int arrsize = 0;
        for (int i = 0; i < size; i++)
        {
            if (c == "e") //if searching for exact code
            {
                if (data[i].get_code() == s)
                {
                    index_location = i;
                    arrsize++;
                    found += data[i].groc_str() + "\n";
                }
            }
            else // if searching for susbstring in code
            {
                if (data[i].get_code().find(s) != std::string::npos)
                {
                    index_location = i;
                    arrsize++;
                    found += data[i].groc_str() + "\n";
                }
            }
        }
        if (arrsize == 0) // if nothing is found
        {
            return "data does not exist\n";
        }
        if (arrsize == 1) //if only one code is found
        {
            while (true)
            {
                cout << "Item to be removed is " << found
                     << "\nAre you sure you want to remove this item? (Y/N) ";
                cin >> choice;
                lowerstring(choice);
                if (choice == "y")
                    break;
                else if (choice == "n")
                    return "";
                else
                {
                    cout << "Please enter a valid option\n";
                }
            }
            data[index_location] = data[size - 1];
            size--;
            return "Item successfully removed\n";
        }
        // if more than one item is found
        while (true)
        {
            cout << "Items to be removed are:\n"
                 << found
                 << "\nAre you sure you want to remove these items? (Y/N) ";
            cin >> choice;
            lowerstring(choice);
            if (choice == "y")
                break;
            else if (choice == "n")
                return "";
            else
            {
                cout << "Please enter a valid option\n\n";
            }
        }

        int n = 0;
        while (n < size)
        {
            if (c == "e")
            {
                if (data[n].get_code() == s)
                {
                    size--;
                    data[n] = data[size];
                    n = 0;
                }
                else
                    n++;
            }
            else
            {
                if (data[n].get_code().find(s) != std::string::npos)
                {
                    size--;
                    data[n] = data[size];
                    n = 0;
                }
                else
                    n++;
            }
        }
        return "Items successfully removed";
    }

    string removebyprice(double price, double p1, double p2)
    {
        string found = "", choice;
        int index_location;
        int arrsize = 0;
        for (int i = 0; i < size; i++)
        {
            if (price > -1) //if searching for exact price
            {
                if (data[i].get_price() == price)
                {
                    index_location = i;
                    arrsize++;
                    found += data[i].groc_str() + "\n";
                }
            }
            else // if looking for range of prices
            {
                if (data[i].get_price() >= p1 && data[i].get_price() <= p2)
                {
                    index_location = i;
                    arrsize++;
                    found += data[i].groc_str() + "\n";
                }
            }
        }
        if (arrsize == 0) // if nothing is found
        {
            return "data does not exist\n";
        }
        if (arrsize == 1) //if only one code is found
        {
            while (true)
            {
                cout << "Item to be removed is " << found
                     << "\nAre you sure you want to remove this item? (Y/N) ";
                cin >> choice;
                lowerstring(choice);
                if (choice == "y")
                    break;
                else if (choice == "n")
                    return "";
                else
                {
                    cout << "Please enter a valid option\n";
                }
            }
            size--;
            data[index_location] = data[size];
            return "Item successfully removed\n";
        }
        // if more than one item is found
        while (true)
        {
            cout << "Items to be removed are:\n"
                 << found
                 << "\nAre you sure you want to remove these items? (Y/N) ";
            cin >> choice;
            lowerstring(choice);
            if (choice == "y")
                break;
            else if (choice == "n")
                return "";
            else
            {
                cout << "Please enter a valid option\n\n";
            }
        }

        int n = 0;
        while (n < size)
        {
            if (price > -1)
            {
                if (data[n].get_price() == price)
                {
                    size--;
                    data[n] = data[size];
                    n = 0;
                }
                else
                    n++;
            }
            else
            {
                if (data[n].get_price() >= p1 && data[n].get_price() <= p2)
                {
                    size--;
                    data[n] = data[size];
                    n = 0;
                }
                else
                    n++;
            }
        }
        return "Items successfully removed";
    }

    string removebydept(string s, string c)
    {
        string found = "", choice;
        int index_location;
        int arrsize = 0;
        for (int i = 0; i < size; i++)
        {
            if (c == "e") //if searching for exact dept
            {
                if (data[i].get_dept() == s)
                {
                    index_location = i;
                    arrsize++;
                    found += data[i].groc_str() + "\n";
                }
            }
            else // if searching for susbstring in dept
            {
                if (data[i].get_dept().find(s) != std::string::npos)
                {
                    index_location = i;
                    arrsize++;
                    found += data[i].groc_str() + "\n";
                }
            }
        }
        if (arrsize == 0) // if nothing is found
        {
            return "data does not exist\n";
        }
        if (arrsize == 1) //if only one word is found
        {
            while (true)
            {
                cout << "Item to be removed is " << found
                     << "\nAre you sure you want to remove this item? (Y/N) ";
                cin >> choice;
                lowerstring(choice);
                if (choice == "y")
                    break;
                else if (choice == "n")
                    return "";
                else
                {
                    cout << "Please enter a valid option\n";
                }
            }
            data[index_location] = data[size - 1];
            size--;
            return "Item successfully removed\n";
        }
        // if more than one item is found
        while (true)
        {
            cout << "Items to be removed are:\n"
                 << found
                 << "\nAre you sure you want to remove these items? (Y/N) ";
            cin >> choice;
            lowerstring(choice);
            if (choice == "y")
                break;
            else if (choice == "n")
                return "";
            else
            {
                cout << "Please enter a valid option\n\n";
            }
        }

        int n = 0;
        while (n < size)
        {
            if (c == "e")
            {
                if (data[n].get_dept() == s)
                {
                    size--;
                    data[n] = data[size];
                    n = 0;
                }
                else
                    n++;
            }
            else
            {
                if (data[n].get_dept().find(s) != std::string::npos)
                {
                    size--;
                    data[n] = data[size];
                    n = 0;
                }
                else
                    n++;
            }
        }
        return "Items successfully removed";
    }
    // ------------------Modifying----------------------------------------------------------------------
    string modify_name(string UI)
    {
        int found = 0, index_location;
        string new_name;
        for (int i = 0; i < size; i++)
        {
            if (data[i].get_name() == UI)
            {
                found++;
                index_location = i;
            }
        }
        if (found == 0)
        {
            return "This item does not exist in the database\n";
        }
        cout << "Enter new name: ";
        getline(cin >> ws, new_name);
        cout << endl;
        if (isalpha(new_name) == false)
        {
            return "Invalid name for product\n";
        }
        else
        {
            cout << "\nModifying " << data[index_location].groc_str() << "\n";
            if (confirm_change() == true)
            {
                data[index_location].set_name(new_name);
                return "Name changed successfully\n";
            }
            else
            {
                return "No changes were made to product name\n";
            }
        }
        return "Item name changed successfully\n";
    }

    //Modifying the code
    string modify_code(string UI)
    {
        int found = 0, index_location;
        string new_code;
        for (int i = 0; i < size; i++)
        {
            if (data[i].get_name() == UI)
            {
                found++;
                index_location = i;
            }
        }
        if (found == 0)
        {
            return "This item does not exist in the database\n";
        }
        cout << "Enter a new code: ";
        cin >> new_code;
        cout << endl;
        if (!is_positive_int(new_code) || new_code.find_first_not_of("1234567890") != std::string::npos)
        {
            return "Invalid code for product\n";
        }
        else
        {
            if (confirm_change() == true)
            {
                data[index_location].set_code(new_code);
                return "Code change for product successful\n";
            }
            else
            {
                return "No changes were made to the price\n";
            }
        }
        return "Item code changed successfully\n";
    }

    //Modifying the price
    string modify_price(string UI)
    {
        int found = 0, index_location;
        string new_price;
        double converted;
        for (int i = 0; i < size; i++)
        {
            if (data[i].get_name() == UI)
            {
                found++;
                index_location = i;
            }
        }
        if (found == 0)
        {
            return "This item does not exist in the database\n";
        }
        cout << "Enter a new price: ";
        cin >> new_price;
        cout << endl;
        if (!is_positive_double(new_price))
        {
            return "Invalid price change\n";
        }
        else
        {
            if (confirm_change() == true)
            {
                converted = stod(new_price);
                data[index_location].set_price(converted);
                return "Price change successfull\n";
            }
            else
            {
                return "No changes were made to price\n";
            }
        }
        return "Item price changed successfully\n";
    }

    //Modifying the department
    string modify_department(string UI)
    {
        int found = 0, index_location;
        string new_department;
        for (int i = 0; i < size; i++)
        {
            if (data[i].get_name() == UI)
            {
                found++;
                index_location = i;
            }
        }
        if (found == 0)
        {
            return "This item does not exist in the database\n";
        }
        cout << "Enter new department: ";
        cin >> new_department;
        cout << endl;
        if (isalpha(new_department) == false)
        {
            return "Invalid inpud\n";
        }
        if (confirm_change() == true)
        {
            data[index_location].set_dept(new_department);
            return "Department change successfull\n";
        }
        else
        {
            return "No changes have been made.\n";
        }
        return "Department change successfull\n";
    } //--------------------------------------------------------------------------------------------------

    //input validation----------------------------------------------------------------------------------
    //check for only alphabetic letters in the input
    bool isalpha(string word) { return word.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") == std::string::npos; }

    //check for positive doubles
    bool is_positive_double(string UI)
    {
        if (is_numeric(UI) == true)
        {
            double n = stod(UI);
            if (n < 0)
            {
                return false;
            }
            else if (n > product_price_threshold)
            {
                return false;
            }
            return true;
        }

        return false;
    }

    //check for positive integers
    bool is_positive_int(string UI)
    {
        if (is_int(UI) == true)
        {
            double n = stod(UI);
            if (n < 0)
            {
                return false;
            }
            else if (n > product_code_threshold)
            {
                return false;
            }
            return true;
        }

        return false;
    }

    //source: https://stackoverflow.com/questions/29169153/how-do-i-verify-a-string-is-valid-double-even-if-it-has-a-point-in-it
    //checks for double numbers
    bool is_numeric(std::string const &str)
    {
        auto result = double();
        auto i = std::istringstream(str);
        i >> result;
        return !i.fail() && i.eof();
    }

    //check if string is int
    bool is_int(const std::string &s)
    {
        std::string::const_iterator it = s.begin();
        while (it != s.end() && std::isdigit(*it))
            ++it;
        return !s.empty() && it == s.end();
    }

    //confirming user input
    bool confirm_change()
    {
        string UI;
        cout << "Are you sure you want to make these changes? [y/n] ";
        cin >> UI;
        if (UI == "y")
        {
            return true;
        }
        else if (UI == "n")
        {
            return false;
        }
        return false;
    }

    //source: https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    //check if a file already exists in the directory
    bool file_exists(const std::string &name)
    {
        ifstream f(name.c_str());
        return f.good();
    }
    //--------------------------------------------------------------------------------------------------
    void lowerstring(string &choice)
    {
        transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
    }
};