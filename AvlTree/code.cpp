#include "avl.hpp"

using namespace std;

void test1()
{
    my_AvlTree::AvlTree<int> t;
    t.Insert(1);
    t.Insert(4);
    t.Insert(564);
    t.Insert(2);
    t.Insert(8);
    t.Insert(345);
    t.Insert(67);
    t.Insert(23);
    t.levelOrder();
    //cout << t.IsAvlTree() << endl;
}
int main()
{
    test1();
    return 0;
}