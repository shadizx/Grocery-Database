#include "database.h"

using namespace std;

class Menu
{
    Database database;

public:
    Menu()
    {
        menu_main();
    }

    void menu_main()
    {
        if (database.failed == true)
        {
            return;
        }
        string choice;
        cout << "----------------------------------------------------------------\n"
             << "========================GROCERY DATABASE========================\n"
             << "----------------------------------------------------------------\n\n";
        cout << "(A) Add item\n"
             << "(F) Find an item\n"
             << "(R) Remove item\n"
             << "(L) List items\n"
             << "(M) Modify item\n"
             << "(C) Clear database\n"
             << "(Q) Quit\n\n"
             << "Enter a letter of your choice: ";
        cin >> choice;
        string_lower(choice);

        choice_check(choice);
    }

    void choice_check(string choice)
    {
        if (choice == "a")
        {
            add_item();
        }
        else if (choice == "f")
        {
            if (database.isempty())
            {
                cout << "Database is empty\n\n";
                menu_main();
            }
            else
                find_item();
        }
        else if (choice == "r")
        {
            if (database.isempty())
            {
                cout << "Database is empty\n\n";
                menu_main();
            }
            else
                remove_item();
        }
        else if (choice == "l")
        {
            if (database.isempty())
            {
                cout << "Database is empty\n\n";
                menu_main();
            }
            else
                list_item();
        }
        else if (choice == "m")
        {
            if (database.isempty())
            {
                cout << "Database is empty\n\n";
                menu_main();
            }
            else
            {
                modify_item();
            }
        }
        else if (choice == "c")
        {
            if (database.isempty())
            {
                cout << "Database is empty\n\n";
                menu_main();
            }
            else
            {
                database.wipe();
                database.update();
                menu_main();
            }
        }
        else if (choice == "q")
        {
            database.update();
            return;
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
            menu_main();
        }
    }

    void add_item()
    {
        string name, code, dept;
        double price;

        cout << "\nAdd an item...\n-----------------\n";

        name = database.namecheck();
        code = database.codecheck();
        price = database.pricecheck();
        dept = database.deptcheck();

        Grocery g(name, code, price, dept);
        cout << "\nAdding " << g.groc_str() << "\n";

        if (database.confirm_change() == true)
        {
            database.append(g); // append to array
            database.update();
            cout << "\nItem Successfully added" << endl;
            menu_main();
        }
        else
        {
            cout << "No changes have been made\n";
            menu_main();
        }
    }

    void find_item()
    {
        string choice;
        cout << "\nFind an item...\n-----------------\n\n"
             << "You can search by:\n\n"
             << "(N) Name\n"
             << "(C) Code\n"
             << "(P) Price\n"
             << "(D) Department\n\n"
             << "(R) Return to main menu\n\n"
             << "Enter the letter of your choice: ";
        cin >> choice;
        string_lower(choice);
        find_choice_check(choice);
    }

    void find_choice_check(string choice)
    {
        string searchchoice = "", found;

        if (choice == "n")
        {
            cout << "\nWould you like to search for (E)xact name or (S)ubstring of the name?\n"
                 << "Enter the letter of your choice: ";
            cin >> searchchoice;
            string_lower(searchchoice);

            while (searchchoice != "e" && searchchoice != "s")
            {
                cout << "Invalid input, try again: ";
                cin >> searchchoice;
                string_lower(searchchoice);
            }

            string name = database.namecheck();
            found = database.findbyname(name, searchchoice);

            if (found != "")
                cout << "Item(s) :\n"
                     << found;
            else
                cout << "Item(s) not found\n";
            find_item();
        }
        else if (choice == "c")
        {
            cout << "\nWould you like to search for (E)xact code or (S)ubstring of the code?\n"
                 << "Enter the letter of your choice: ";
            cin >> searchchoice;
            string_lower(searchchoice);

            while (searchchoice != "e" && searchchoice != "s")
            {
                cout << "Invalid input, try again: ";
                cin >> searchchoice;
                string_lower(searchchoice);
            }

            string code = database.codecheck();
            found = database.findbycode(code, searchchoice);

            if (found != "")
                cout << "Item(s) :\n"
                     << found;
            else
                cout << "Item(s) not found\n";
            find_item();
        }
        else if (choice == "p")
        {
            cout << "\nWould you like to search for (E)xact price or (R)ange of prices?\n"
                 << "Enter the letter of your choice: ";
            cin >> searchchoice;
            string_lower(searchchoice);

            while (searchchoice != "e" && searchchoice != "r")
            {
                cout << "Invalid input, try again: ";
                cin >> searchchoice;
                string_lower(searchchoice);
            }

            double price = -1, p1 = -1, p2 = -1;

            if (searchchoice == "e")
            {
                price = database.pricecheck();
            }
            else if (searchchoice == "r")
            {
                string p1str, p2str;
                while (true)
                {
                    cout << "\nEnter a range of prices of the item(s) you would like to find: \n";
                    cout << "From: ";
                    cin >> p1str;
                    cout << "To: ";
                    cin >> p2str;
                    cout << "\n";

                    if (database.is_positive_double(p1str) && database.is_positive_double(p2str))
                    {
                        p1 = stod(p1str);
                        p2 = stod(p2str);
                        if (p2 >= p1)
                            break;
                        else
                        {
                            cout << "Invalid input, try again\n";
                        }
                    }
                    else
                    {
                        cout << "Invalid input, try again\n";
                    }
                }
            }

            found = database.findbyprice(price, p1, p2);

            if (found != "")
                cout << "Item(s):\n"
                     << found;
            else
                cout << "Item(s) not found\n";
            find_item();
        }
        else if (choice == "d")
        {
            cout << "\nWould you like to search for (E)xact department or (S)ubstring of the department?\n"
                 << "Enter the letter of your choice: ";
            cin >> searchchoice;
            string_lower(searchchoice);

            while (searchchoice != "e" && searchchoice != "s")
            {
                cout << "Invalid input, try again: ";
                cin >> searchchoice;
                string_lower(searchchoice);
            }

            string dept = database.deptcheck();

            found = database.findbydept(dept, searchchoice);

            if (found != "")
                cout << "Item(s) :\n"
                     << found;
            else
                cout << "Item(s) not found\n";
            find_item();
        }
        else if (choice == "r")
        {
            menu_main();
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
            find_item();
        }
    }

    void
    list_item()
    {
        string choice;
        cout << "\nList items...\n-----------------\n\n"
             << "You can list by:\n\n"
             << "(N) Name\n"
             << "(C) Code\n"
             << "(P) Price\n"
             << "(D) Department\n\n"
             << "(R) Return to main menu\n\n"
             << "Enter the letter of your choice: ";
        cin >> choice;
        string_lower(choice);
        list_choice_check(choice);
    }

    void list_choice_check(string c)
    {
        string choice;
        if (c == "n")
        {
            cout << "\nList items by name...\n-----------------\n\n"
                 << "You can sort names by:\n\n"
                 << "(A) Alphabetical Order\n"
                 << "(R) Reverse Alphabetical Order\n"
                 << "(Q) Return to main menu\n\n"
                 << "Enter the letter of your choice: ";
            cin >> choice;
            string_lower(choice);
            namesortcheck(choice);
        }
        else if (c == "c")
        {
            cout << "\nList items by code...\n-----------------\n\n"
                 << "You can sort codes by:\n\n"
                 << "(A) Ascending Order\n"
                 << "(D) Descending Order\n"
                 << "(Q) Return to main menu\n\n"
                 << "Enter the letter of your choice: ";
            cin >> choice;
            string_lower(choice);
            codesortcheck(choice);
        }
        else if (c == "p")
        {
            cout << "\nList items by price...\n-----------------\n\n"
                 << "You can sort prices by:\n\n"
                 << "(A) Ascending Order\n"
                 << "(D) Descending Order\n"
                 << "(Q) Return to main menu\n\n"
                 << "Enter the letter of your choice: ";
            cin >> choice;
            string_lower(choice);
            pricesortcheck(choice);
        }
        else if (c == "d")
        {
            cout << "\nList items by department...\n-----------------\n\n"
                 << "You can sort departments by:\n\n"
                 << "(A) Alphabetical Order\n"
                 << "(R) Reverse Alphabetical Order\n"
                 << "(Q) Return to main menu\n\n"
                 << "Enter the letter of your choice: ";
            cin >> choice;
            string_lower(choice);
            deptsortcheck(choice);
        }
        else if (c == "r")
        {
            menu_main();
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
            list_item();
        }
    }

    //sorting checks---------------------------------------------------
    void namesortcheck(string choice)
    {
        if (choice == "a")
        {
            database.namesortalpha();
            database.listdb();
            list_item();
        }
        else if (choice == "r")
        {
            database.namesortralpha();
            database.listdb();
            list_item();
        }
        else if (choice == "q")
        {
            menu_main();
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
            list_choice_check("n");
        }
    }
    void codesortcheck(string choice)
    {
        if (choice == "a")
        {
            database.codesortasc();
            database.listdb();
            list_item();
        }
        else if (choice == "d")
        {
            database.codesortdesc();
            database.listdb();
            list_item();
        }
        else if (choice == "q")
        {
            menu_main();
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
            list_choice_check("c");
        }
    }
    void pricesortcheck(string choice)
    {
        if (choice == "a")
        {
            database.pricesortasc();
            database.listdb();
            list_item();
        }
        else if (choice == "d")
        {
            database.pricesortdesc();
            database.listdb();
            list_item();
        }
        else if (choice == "q")
        {
            menu_main();
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
            list_choice_check("p");
        }
    }
    void deptsortcheck(string choice)
    {
        if (choice == "a")
        {
            database.deptsortalpha();
            database.listdb();
            list_item();
        }
        else if (choice == "r")
        {
            database.deptsortralpha();
            database.listdb();
            list_item();
        }
        else if (choice == "q")
        {
            menu_main();
        }
        else
        {
            cout << "Please enter a valid choice" << endl;
            list_choice_check("d");
        }
    }
    //-----------------------------------------------------------------

    //removing item-----------------------------------------------------

    void remove_item()
    {
        string choice;
        cout << "\nRemove items...\n-----------------\n\n"
             << "You can remove by:\n\n"
             << "(N) Name\n"
             << "(C) Code\n"
             << "(P) Price\n"
             << "(D) Department\n\n"
             << "(R) Return to main menu\n\n"
             << "Enter the letter of your choice: ";
        cin >> choice;
        string_lower(choice);
        remove_choice_check(choice);
    }

    void remove_choice_check(string c)
    {
        string choice;
        if (c == "n")
        {
            while (true)
            {
                cout << "\nWould you like to search for (E)xact name or (S)ubstring of the name?\n"
                     << "Enter the letter of your choice: ";
                cin >> choice;
                string_lower(choice);

                if (choice == "e" || choice == "s")
                    break;
                else
                    cout << "Please enter a valid choice\n";
            }
            string name = database.namecheck();

            cout << database.removebyname(name, choice) << "\n";
            database.update();

            remove_item();
        }
        else if (c == "c")
        {
            while (true)
            {
                cout << "\nWould you like to search for (E)xact code or (S)ubstring of the code?\n"
                     << "Enter the letter of your choice: ";
                cin >> choice;
                string_lower(choice);

                if (choice == "e" || choice == "s")
                    break;
                else
                    cout << "Please enter a valid choice\n";
            }
            string code = database.codecheck();

            cout << database.removebycode(code, choice) << "\n";
            database.update();

            remove_item();
        }
        else if (c == "p")
        {
            double price = -1, p1 = -1, p2 = -1;

            while (true)
            {
                cout << "\nWould you like to search for (E)xact price or a (R)ange of prices?\n"
                     << "Enter the letter of your choice: ";
                cin >> choice;
                string_lower(choice);

                if (choice == "e" || choice == "r")
                    break;
                else
                    cout << "Please enter a valid choice\n";
            }
            if (choice == "e")
            {
                price = database.pricecheck();
            }
            else if (choice == "r")
            {
                while (true)
                {
                    cout << "\nEnter a range of prices of the item(s) you would like to find: \n";
                    cout << "From: ";
                    cin >> p1;
                    cout << "To: ";
                    cin >> p2;
                    cout << "\n";

                    if (p1 >= 0 && p2 > p1)
                        break;
                    else
                    {
                        cout << "Invalid input, try again\n";
                    }
                }
            }

            cout << database.removebyprice(price, p1, p2) << "\n";
            database.update();

            remove_item();
        }
        else if (c == "d")
        {
            while (true)
            {
                cout << "\nWould you like to search for (E)xact department or (S)ubstring of the department?\n"
                     << "Enter the letter of your choice: ";
                cin >> choice;
                string_lower(choice);

                if (choice == "e" || choice == "s")
                    break;
                else
                    cout << "Please enter a valid choice\n";
            }
            string dept = database.deptcheck();

            cout << database.removebydept(dept, choice) << "\n";
            database.update();

            remove_item();
        }
        else if (c == "r")
        {
            menu_main();
        }
        else
        {
            cout << "Invalid option\n";
            remove_item();
        }
    }

    //Modifying item----------------------------------------------------------------

    void modify_item()
    {
        string choice;
        cout << "\nModify items...\n-----------------\n\n"
             << "You can modify item:\n\n"
             << "(N) Name\n"
             << "(C) Code\n"
             << "(P) Price\n"
             << "(D) Department\n\n"
             << "(R) Return to main menu\n\n"
             << "Enter the letter of your choice: ";

        cin >> choice;
        string_lower(choice);
        modify_choice_check(choice);
    }

    void modify_choice_check(string choice)
    {
        string name, code, dept, price;
        string searchchoice = "", found;
        if (choice == "n")
        {
            name = database.namecheck();
            cout << database.modify_name(name) << endl;
            database.update();
            modify_item();
        }
        else if (choice == "c")
        {
            code = database.namecheck();
            cout << database.modify_code(code) << endl;
            database.update();
            modify_item();
        }
        else if (choice == "p")
        {
            price = database.namecheck();
            cout << database.modify_price(price) << endl;
            database.update();
            modify_item();
        }
        else if (choice == "d")
        {
            dept = database.namecheck();
            cout << database.modify_department(dept) << endl;
            database.update();
            modify_item();
        }
        else if (choice == "r")
        {
            menu_main();
        }
        else
        {
            cout << "Invalid choice\n";
            modify_item();
        }
    }

    //-------------------------------------------------------------------------

    void string_lower(string &choice)
    {
        transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
    }
};