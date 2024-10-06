#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "employee.hpp"

using namespace std;

// Info类：信息处理类。该类的作用是处理以上定义的各个类的信息输入和输出。
class Info
{
private:
    vector<Employee *> employees;

public:
    void addNatural(Employee *emp)
    {
        employees.push_back(emp);
    }

    void showInfo()
    {
        if (employees.empty())
        {
            lg(WARNING, "data empty");
        }
        for (auto emp : employees)
        {
            emp->displayEmployeeInfo();
            emp->showType();
            emp->pay();
            cout << "Salary: " << emp->getPay() << endl;
            cout << "-----------------------------" << endl;
        }
    }

    void writeInfo()
    {
        for (auto emp : employees)
        {
            emp->writeFile();
        }
    }

    void readInfo()
    {
        ifstream infile("employee_data.txt");
        if (infile.is_open())
        {
            string line;
            while (getline(infile, line))
            {
                cout << line << endl;
            }
            infile.close();
        }
        else
        {
            lg(ERROR, "open fail");
        }
    }
};
