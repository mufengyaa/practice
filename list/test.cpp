#include "mylist.hpp"
#include "mystring.hpp"

#include <list>

void test1()
{
    std::list<std::pair<int, bit::mystring>> l;
    l.push_back(make_pair(1, "22"));
    l.push_back({1, "1434"});
    cout << endl;
    l.emplace_back(1, "111");
    cout << endl;
    std::pair<int, bit::mystring> a(1, "23442");
    l.emplace_back(a);
    cout << endl;
    l.emplace_back(make_pair(1, "22"));

    // bit::mylist<std::pair<int, bit::mystring>> l;
    // l.push_back(make_pair(1, "22"));
    // l.push_back({1, "1434"});
    // cout << endl;
    // l.emplace_back(1, "111");
    // cout << endl;
    // std::pair<int, bit::mystring> a(1, "23442");
    // l.emplace_back(a);
    // cout << endl;
    // l.emplace_back(make_pair(1, "22"));
}
int main()
{
    test1();
    return 0;
}