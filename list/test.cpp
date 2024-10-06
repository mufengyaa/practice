#include "mylist.hpp"
#include "mystring.hpp"
#include <typeinfo>
#include <iostream>

#include <list>

void test1()
{
    // std::list<std::pair<int, bit::mystring>> l;
    // l.push_back(make_pair(1, "22"));
    // l.push_back({1, "1434"});
    // cout << endl;
    // l.emplace_back(1, "111");
    // cout << endl;
    // std::pair<int, bit::mystring> a(1, "23442");
    // l.emplace_back(a);
    // cout << endl;
    // l.emplace_back(make_pair(1, "22"));

    bit::mylist<std::pair<int, bit::mystring>> l;
    // l.push_back(make_pair(1, "22"));
    // cout << endl;
    l.push_back({1, "1434"});
    cout << endl;
    // l.emplace_back(1, "111");
    // cout << endl;
    // std::pair<int, bit::mystring> a(1, "23442");
    // l.emplace_back(a);
    // cout << endl;
    // l.emplace_back(make_pair(1, "22"));
}

// int get()
// {
//     int x = 1;
//     return x;
// }
// void test(int &&x)
// {
//     cout << "im && here" << endl;
// }
// void test(int &x)
// {
//     cout << "im & here" << endl;
// }
int main()
{
    // int &&t1 = 1;
    // int &&t2 = get();

    // test(get());
    // test(static_cast<typename std::remove_reference<int &&>::type &&>(1));

    test1();
    return 0;
}