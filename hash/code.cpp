#include "hash.hpp"

void test1()
{
    my_hash_bucket::HashBucket<int> hs;
    hs.Insert(1);
    hs.Insert(5);
    hs.Insert(2);
    hs.Print();
    hs.Insert(23);
    hs.Insert(3);
    hs.Print();
    hs.Insert(6);
    hs.Print();

    hs.Erase(3);
    hs.Erase(6);
    hs.Print();
}
void test2()
{
    my_hash_table::HashTable<int, int> ht;
    ht.Insert(make_pair(1, 1));
    ht.Insert(make_pair(2, 1));
    ht.Insert(make_pair(9, 1));
    ht.Print();
    ht.Insert(make_pair(56, 1));
    ht.Insert(make_pair(3, 1));
    ht.Insert(make_pair(5, 1));
    ht.Print();

    ht.Insert(make_pair(5, 1));
    ht.Print();

    ht.Erase(1);
    ht.Print();
}
int main()
{
    test1();
    return 0;
}