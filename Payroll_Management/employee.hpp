#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Log.hpp"

using namespace std;

// Information类：职员基本信息类
class Information
{
private:
    int age;
    double weight;

public:
    Information() : age(0), weight(0.0) {}
    Information(int a, double w) : age(a), weight(w) {}

    void setAge(int a) { age = a; }
    int getAge() const { return age; }

    void setWeight(double w) { weight = w; }
    double getWeight() const { return weight; }

    void displayInfo() const
    {
        cout << "Age: " << age << ", Weight: " << weight << endl;
    }
};

// Employee类：职员类
class Employee
{
private:
    static string CompanyName;
    int id;
    string name;
    Information info;

protected:
    double money;

public:
    Employee() : id(0), name(""), money(0.0) {}
    Employee(int i, string n, Information inf) : id(i), name(n), info(inf), money(0.0) {}

    static void setCompanyName(const string &companyName) { CompanyName = companyName; }
    static string getCompanyName() { return CompanyName; }

    void setId(int i) { id = i; }
    int getId() const { return id; }

    void setName(const string &n) { name = n; }
    string getName() const { return name; }

    void setInfo(const Information &inf) { info = inf; }
    Information getInfo() const { return info; }

    virtual void pay() = 0;
    virtual void showType() = 0;

    double getPay()
    {
        pay();
        return money;
    }

    void displayEmployeeInfo()
    {
        cout << "Company Name: " << CompanyName << ", ID: " << id << ", Name: " << name << ", pay: " << getPay() << endl;
        info.displayInfo();
    }

    void writeFile()
    {
        ofstream outfile("employee_data.txt", ios::app);
        if (outfile.is_open())
        {
            outfile << "ID: " << id << ", Name: " << name << ", Company Name: " << CompanyName << ", pay: " << getPay() << endl;
            outfile.close();
            lg(INFO, "write success");
        }
        else
        {
            lg(ERROR, "open fail");
        }
    }
};

string Employee::CompanyName = "Mufeng Company";

// Technician类：技术员工类
class Technician : public Employee
{
private:
    int hour;
    double hourlyRate;

public:
    Technician() : Employee(), hour(0), hourlyRate(100.0) {}
    Technician(int i, string n, Information inf, int h, double rate)
        : Employee(i, n, inf), hour(h), hourlyRate(rate) {}

    void setHour(int h) { hour = h; }
    int getHour() const { return hour; }

    void setHourlyRate(double rate) { hourlyRate = rate; }
    double getHourlyRate() const { return hourlyRate; }

    void pay() override
    {
        money = hour * hourlyRate;
    }

    void showType() override
    {
        cout << "Technician" << endl;
    }
};

// Manager类：经理类。是Employee的子类。虚基类。
class Manager : virtual public Employee
{
protected:
    double monthlyPay;

public:
    Manager() : Employee(), monthlyPay(8000.0) {}
    Manager(int i, string n, Information inf, double pay)
        : Employee(i, n, inf), monthlyPay(pay) {}

    void setMonthlyPay(double pay) { monthlyPay = pay; }
    double getMonthlyPay() const { return monthlyPay; }

    void pay() override
    {
        money = monthlyPay;
    }

    void showType() override
    {
        cout << "Manager" << endl;
    }
};

// Salesman类：销售员类。是Employee的子类。虚基类。
class Salesman : virtual public Employee
{
protected:
    double sales;
    double salesRate;

public:
    Salesman() : Employee(), sales(0.0), salesRate(0.04) {}
    Salesman(int i, string n, Information inf, double s, double rate)
        : Employee(i, n, inf), sales(s), salesRate(rate) {}

    void setSales(double s) { sales = s; }
    double getSales() const { return sales; }

    void setSalesRate(double rate) { salesRate = rate; }
    double getSalesRate() const { return salesRate; }

    void pay() override
    {
        money = sales * salesRate;
    }

    void showType() override
    {
        cout << "Salesman" << endl;
    }
};

// ManagerSales类：销售经理类。公有继承Salesman类和Manager类
class ManagerSales : public Manager, public Salesman
{
public:
    ManagerSales() : Employee(), Manager(), Salesman() {}
    ManagerSales(int i, string n, Information inf, double s, double rate, double pay)
        : Employee(i, n, inf), Manager(i, n, inf, pay), Salesman(i, n, inf, s, rate) {}

    void pay() override
    {
        money = monthlyPay + sales * salesRate;
    }

    void showType() override
    {
        cout << "ManagerSales" << endl;
    }
};