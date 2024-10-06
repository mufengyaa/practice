#include "unordered_set.hpp"
#include "unordered_map.hpp"
void test1()
{
    my_unordered_set::unordered_set<int> hs;
    hs.insert(1);
    hs.insert(1);
    hs.insert(5);
    hs.insert(2);
    hs.insert(23);
    hs.insert(3);
    hs.insert(6);
    for (auto tmp : hs)
    {
        cout << tmp << " ";
    }
    cout << endl;
    auto it = hs.begin();
    hs.erase(it);
    hs.erase(hs.end());
    for (auto tmp : hs)
    {
        cout << tmp << " ";
    }
    cout << endl;

    it = hs.begin();
    for (auto tmp : hs) {
        it=hs.erase(it);
    }
    for (auto tmp : hs)
    {
        cout << tmp << " ";
    }
    cout << endl;
}
void test2() {
    my_unordered_map::unordered_map<int,int> hs;
    hs.insert(make_pair(1,1));
    hs.insert(make_pair(12, 1));
    hs.insert(make_pair(1, 1));
    hs.insert(make_pair(13, 1));
    hs.insert(make_pair(13, 1));
    hs.insert(make_pair(55, 1));
    hs.insert(make_pair(541, 1));
    for (auto tmp : hs)
    {
        cout << tmp.first << " " ;
    }
    cout << endl;
    auto it = hs.begin();
    hs.erase(it);
    hs.erase(hs.end());
    for (auto tmp : hs)
    {
        cout << tmp.first << " ";
    }
    cout << endl;

    it = hs.begin();
    for (auto tmp : hs) {
        it = hs.erase(it);
    }
    for (auto tmp : hs)
    {
        cout << tmp.first << " ";
    }
    cout << endl;
}
int main()
{
    test2();
    return 0;
}