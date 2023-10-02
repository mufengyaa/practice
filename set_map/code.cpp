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

int main()
{
    test2();
    return 0;
}