#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "info.hpp"

using namespace std;

// Menu类：菜单类。该类的作用是显示菜单供用户使用
class Menu
{
private:
    Info info;

public:
    void select_menu()
    {
        int choice;
        while (true)
        {
            cout << "----------------------------------------------------------------" << endl;
            cout << "1. Add Employee" << endl;
            cout << "2. Show Employees" << endl;
            cout << "3. Save to File" << endl;
            cout << "4. Load from File" << endl;
            cout << "5. Exit" << endl;
            cout << "----------------------------------------------------------------" << endl;

            cout << "Enter your choice: ";
            cin >> choice;

            handle_menu(choice);
        }
    }

    void handle_menu(int choice)
    {
        switch (choice)
        {
        case 1:
            addEmployee();
            break;
        case 2:
            info.showInfo();
            break;
        case 3:
            info.writeInfo();
            break;
        case 4:
            info.readInfo();
            break;
        case 5:
            exit(0);
        default:
            lg(ERROR, "Invalid type. Try again.");
        }
    }

    void addEmployee()
    {
        int type;
        cout << "Select Employee Type: 1. Technician 2. Manager 3. Salesman 4. ManagerSales: ";
        cin >> type;

        int id, age, hour;
        double weight, rate, sales, pay;
        string name;
        Information tmp;

        cout << "Enter ID: ";
        cin >> id;
        cout << "Enter Name: ";
        cin >> name;
        cout << "Enter Age: ";
        cin >> age;
        cout << "Enter Weight: ";
        cin >> weight;

        tmp.setAge(age);
        tmp.setWeight(weight);

        switch (type)
        {
        case 1:

            cout << "Enter Hours Worked: ";
            cin >> hour;
            info.addNatural(new Technician(id, name, tmp, hour, 100));
            lg(INFO, "add success");
            break;
        case 2:
            info.addNatural(new Manager(id, name, tmp, 8000));
            lg(INFO, "add success");
            break;
        case 3:
            cout << "Enter Sales: ";
            cin >> sales;
            info.addNatural(new Salesman(id, name, tmp, sales, 0.04));
            lg(INFO, "add success");

            break;
        case 4:
            cout << "Enter Sales: ";
            cin >> sales;
            info.addNatural(new ManagerSales(id, name, tmp, sales, 0.005, 5000));
            lg(INFO, "add success");

            break;
        default:
            lg(ERROR, "Invalid type. Try again.");
        }
    }
};

int main()
{
    Employee::setCompanyName("Mufeng Yaa");
    Menu menu;
    menu.select_menu();
    return 0;
}
