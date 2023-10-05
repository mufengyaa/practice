#include "map.hpp"
#include "set.hpp"

using namespace std;

void test1()
{
    // my_RB_Tree::RBTree<int> t;
    // t.Insert(1);
    // cout<<t.IsValidRBTRee()<<endl;
    // t.Insert(543);
    // t.Insert(21);
    // t.Insert(345);
    // cout<<t.IsValidRBTRee()<<endl;
    // t.Insert(15);
    // t.Insert(13);
    // t.Insert(2);
    // t.Insert(8);
    // cout<<t.IsValidRBTRee()<<endl;
    // t.Insert(45);
    // t.Insert(3);
    // t.Insert(235);
    // t.Insert(13);
    // t.Insert(34);
    // t.Insert(41);
    // t.Insert(12);
    // cout<<t.IsValidRBTRee()<<endl;
}

struct SetKeyOfT
{
    const int &operator()(const int &key)
    {
        return key;
    }
};

void test2()
{
    my_RB_Tree::RBTree<int, int, SetKeyOfT> t;
    t.Insert(1);
    t.Insert(1);
    for (auto it : t)
    {
        cout << it << " ";
    }
    cout << endl;
    t.Insert(543);
    t.Insert(21);
    t.Insert(345);
    for (auto it : t)
    {
        cout << it << " ";
    }
    cout << endl;
    t.Insert(15);
    t.Insert(13);
    t.Insert(2);
    t.Insert(8);
    for (auto it : t)
    {
        cout << it << " ";
    }
    cout << endl;
    t.Insert(45);
    t.Insert(3);
    t.Insert(235);
    t.Insert(13);
    t.Insert(34);
    t.Insert(41);
    t.Insert(12);
    for (auto it : t)
    {
        cout << it << " ";
    }
    cout << endl;
}

void test3()
{
    my_set::set<int> s;
    s.insert(8);
    s.insert(4);
    s.insert(5);
    s.insert(4);
    for (auto tmp : s)
    {
        cout << tmp << " ";
    }
}
void test4()
{
    my_map::map<string, string> m;
    m.insert(make_pair("sort", "排序"));
    m.insert(make_pair("apple", "苹果"));
    m.insert(make_pair("map", "地图"));
    for (auto tmp : m)
    {
        cout << tmp.first << ":" << tmp.second << endl;
    }
}
void test5()
{
    my_map::map<string, string> m;
    m["sort"] = "排序";
    m["apple"] = "苹果";
    m["map"] = "地图";
    for (auto tmp : m)
    {
        cout << tmp.first << ":" << tmp.second << endl;
    }
}
int main()
{
    test5();
    return 0;
}