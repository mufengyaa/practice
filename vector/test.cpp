#include "myvector.hpp"
#include "mystring.hpp"
#include "mylist.hpp"

using namespace bit;

void test1()
{
    myvector<int> s1;
    s1.push_back(1);
    s1.push_back(2);
    s1.push_back(3);
    s1.push_back(4);
    s1.push_back(5);

    myvector<int>::iterator pos = s1.begin() + 2;
    pos = s1.insert(pos, 10);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;

    s1.resize(20, 2);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
}
void test2()
{
    myvector<int> s1;
    s1.push_back(1);
    s1.push_back(2);
    s1.push_back(3);
    s1.push_back(4);
    s1.push_back(5);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;

    myvector<int>::iterator pos = s1.begin() + 2;
    cout << *pos << endl;

    s1.pop_back();
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;

    pos = s1.erase(pos);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
}
void test3()
{
    myvector<int> s1;
    s1.push_back(1);
    s1.push_back(2);
    s1.push_back(2);
    s1.push_back(3);
    s1.push_back(4);
    s1.push_back(6);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
    auto it = s1.begin();
    while (it != s1.end())
    {
        if (*it % 2 == 0)
        {
            s1.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
}
void test4()
{
    myvector<int> s1(10, 1);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
    string str = "asrfsdfsd";
    myvector<char> s2(str.begin(), str.end());
    for (auto c : s2)
    {
        cout << c << " ";
    }
    cout << endl;

    myvector<int> s3;
    s3 = s1;
    for (auto c : s3)
    {
        cout << c << " ";
    }
    cout << endl;

    myvector<int> s4(s1);
    for (auto c : s4)
    {
        cout << c << " ";
    }
    cout << endl;
}

void test5()
{
    myvector<int> s1;
    s1.push_back(1);
    s1.push_back(2);
    s1.push_back(2);
    s1.push_back(3);
    s1.push_back(4);
    s1.push_back(6);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
    auto pos = s1.end() - 1;
    pos = s1.erase(pos);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
    cout << *pos << endl;
}
void test6()
{
    // auto it = {1, 2, 3, 4};
    // myvector<int> s1(it);
    myvector<int> s2 = {1, 2, 3, 4};
    for (auto tmp : s2)
    {
        cout << tmp << " ";
    }
    cout << endl;
}

#include <vector>
// struct A
// {
//     A()
//     {
//         cout << "A()" << endl;
//     }
//     A(const A &tmp)
//     {
//         cout << "A(const A& tmp)" << endl;
//     }
//     int a;
// };
bit::myvector<int> func_move() // 返回右值
{
    cout << "func_move" << endl;
    auto it = {1, 2, 3, 4};
    myvector<int> tmp(it);
    return tmp;
}
// A func_move() // 返回右值
// {
//     cout << "func_move" << endl;
//     A tmp;
//     return tmp;
// }
void test7() // 验证移动语义
{
    bit::myvector<int> s1(func_move());
    bit::myvector<int> s2;
    // A s1(func_move());
}
#include <list>
void test8()
{
    vector<bit::mystring> s1;

    bit::mystring arr("1234");
    s1.push_back(arr);
    cout << endl;

    s1.push_back("243");
    // list<bit::mystring> s1;

    // bit::mystring arr("1234");
    // s1.push_back(arr);
    // cout<<endl;

    // s1.push_back("243");
}
void test9()
{
    bit::mylist<bit::mystring> l;
    l.push_back("123");
    cout << endl;
    bit::mystring arr("q34");
    l.push_back(arr);
}

int main()
{
    test9();
    return 0;
}