#include "code.hpp"

using namespace bit;

void test1()
{
    mylist<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    for (auto c : l)
    {
        cout << c << " ";
    }
    cout << endl;
    l.pop_back();
    for (auto c : l)
    {
        cout << c << " ";
    }
    cout << endl;
    l.push_front(3);
    for (auto c : l)
    {
        cout << c << " ";
    }
    cout << endl;
    l.pop_front();
    for (auto c : l)
    {
        cout << c << " ";
    }
    cout << endl;
}
void test2()
{
    mylist<int> l(10, 1);
    for (auto c : l)
    {
        cout << c << " ";
    }
    cout << endl;
    mylist<int>::iterator it = l.begin();
    mylist<int>::iterator a = --l.begin(), b = l.end(), c = ++l.end();
    mylist<int>::iterator d = l.begin()++;
    cout << (it == a) << " " << (it == c) << endl;
    cout << (it == d) << endl;
}

struct A
{
    A(int a1 = 0, int a2 = 0)
        : _a1(a1), _a2(a2)
    {
    }

    int _a1;
    int _a2;
};
void test3()
{
    mylist<A> lt;
    lt.push_back(A(1, 1));
    lt.push_back(A(2, 2));
    lt.push_back(A(3, 3));
    lt.push_back(A(4, 4));

    mylist<A>::iterator it = lt.begin();
    while (it != lt.end())
    {
        cout << (*it)._a1 << " " << (*it)._a2 << endl;
        cout << it->_a1 << " " << it->_a2 << endl;

        ++it;
    }
    cout << lt.size() << endl;
}
void test4()
{
    mylist<string> l;
    l.push_back("aa");
    l.push_back("bb");
    l.push_back("cc");
    for (auto c : l)
    {
        cout << c << " ";
    }
    cout << endl;
    mylist<string> tmp(l.begin(), l.end());
    for (auto c : tmp)
    {
        cout << c << " ";
    }
    cout << endl;
    mylist<string> a(l);
    for (auto c : a)
    {
        cout << c << " ";
    }
    cout << endl;
}
void test5()
{
    mylist<string> l;
    l.push_back("aa");
    l.push_back("bb");
    l.push_back("cc");
    cout << l.front() << endl;
    cout << l.back() << endl;
}
void test6()
{
    mylist<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    for (mylist<int>::reverse_iterator it = l.rbegin(); it != l.rend(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
    auto it =l.rbegin();
    cout<<*(++it)<<endl;
    cout<<*(it++)<<endl;
}
void test7(){
    
}
int main()
{
    test6();
    return 0;
}