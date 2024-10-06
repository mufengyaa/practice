#include "bit_set.hpp"
#include "BloomFilter.hpp"

void test1()
{
    my_bitset::bitSet<10> b;
    b.set(2);
    b.set(4);
    b.set(5);
    cout << b.test(2) << " " << b.test(5) << endl;
    b.reset(2);
    cout << b.test(2) << " " << b.test(5) << endl;
}
void test2()
{
    int a1[] = {1, 2, 3, 3, 4, 4, 4, 4, 4, 2, 3, 6, 3, 1, 5, 5, 8, 9};
    int a2[] = {8, 4, 8, 4, 1, 1, 1, 1};
    my_bitset::bitSet<10> bs1;
    my_bitset::bitSet<10> bs2;
    for (auto e : a1)
    {
        bs1.set(e);
    }
    for (auto e : a2)
    {
        bs2.set(e);
    }
    for (size_t i = 0; i < 10; i++)
    {
        if (bs1.test(i) && bs2.test(i))
        {
            cout << i << " ";
        }
    }
    cout << endl;
}
void test3()
{
    my_BloomFilter::BloomFilter<100> bf;
    bf.set("阴阳师");
    bf.set("无期迷途");
    bf.set("蔚蓝档案");
    cout << bf.test("阴阳师") << endl;
    cout << bf.test("yys") << endl;
}
int main()
{
    test3();
    return 0;
}