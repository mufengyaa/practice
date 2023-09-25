#include "RB_Tree.hpp"

using namespace std;

void test1()
{
    my_RB_Tree::RBTree<int> t;
    t.Insert(1);
    cout<<t.IsValidRBTRee()<<endl;
    t.Insert(543);
    t.Insert(21);
    t.Insert(345);
    cout<<t.IsValidRBTRee()<<endl;
    t.Insert(15);
    t.Insert(13);
    t.Insert(2);
    t.Insert(8);
    cout<<t.IsValidRBTRee()<<endl;
    t.Insert(45);
    t.Insert(3);
    t.Insert(235);
    t.Insert(13);
    t.Insert(34);
    t.Insert(41);
    t.Insert(12);
    cout<<t.IsValidRBTRee()<<endl;
}
int main()
{
    test1();
    return 0;
}