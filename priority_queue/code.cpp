#include "code.hpp"

void test1()
{
    bit::mypriority_queue<int> q;
    q.push(1);
    q.push(7);
    q.push(2);
    q.push(4);
    while (!q.empty())
    {
        cout << q.top() << " ";
        q.pop();
    }
    cout << endl;

    bit::mypriority_queue<int, vector<int>, bit::mygreater<int>> qq;
    qq.push(1);
    qq.push(7);
    qq.push(2);
    qq.push(4);
    while (!qq.empty())
    {
        cout << qq.top() << " ";
        qq.pop();
    }
    cout << endl;
}

class Date
{
public:
    Date(int year = 1900, int month = 1, int day = 1)
        : _year(year), _month(month), _day(day)
    {
    }

    bool operator<(const Date &d) const
    {
        return (_year < d._year) ||
               (_year == d._year && _month < d._month) ||
               (_year == d._year && _month == d._month && _day < d._day);
    }

    bool operator>(const Date &d) const
    {
        return (_year > d._year) ||
               (_year == d._year && _month > d._month) ||
               (_year == d._year && _month == d._month && _day > d._day);
    }

    friend ostream &operator<<(ostream &_cout, const Date &d);

private:
    int _year;
    int _month;
    int _day;
};
ostream &operator<<(ostream &_cout, const Date &d)
{
    _cout << d._year << "-" << d._month << "-" << d._day;
    return _cout;
}

void test2()
{
    bit::mypriority_queue<Date> q;
    q.push(Date(2020, 10, 1));
    q.push(Date(2020, 11, 1));
    q.push(Date(2020, 8, 1));
    while (!q.empty())
    {
        cout << q.top() << " ";
        q.pop();
    }
    cout << endl;
}

template <class T>
class LessPDate
{
public:
    bool operator()(const T *p1, const T *p2)
    {
        return *p1 < *p2;
    }
};
void test3()
{
    bit::mypriority_queue<Date *, vector<Date *>, LessPDate<Date>> q;
    // q.push(&Date(2020,10,1)); //获取了临时变量的地址 (它的生命周期是在该行创建，又在该行销毁)

    q.push(new Date(2020, 11, 1));
    q.push(new Date(2020, 12, 1));
    q.push(new Date(2020, 8, 1));
    while (!q.empty())
    {
        cout << *q.top() << " ";
        q.pop();
    }
    cout << endl;
}

int main()
{
    test3();
    return 0;
}