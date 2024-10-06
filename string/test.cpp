#include "mystring.hpp"
using namespace bit;

void test()
{
    mystring s1("hello");
    cout << s1 << endl;
    s1.append(" world");
    cout << s1 << endl;
    s1.push_back('!');
    cout << s1 << endl;

    s1 += " hello";
    cout << s1 << endl;

    s1 += '!';
    cout << s1 << endl;
}
void test1()
{
    mystring s1("hello");
    cout << s1 << endl;
    s1.insert(2, 3, '*');
    cout << s1 << endl;
    s1.insert(1, 3, '*');
    cout << s1 << endl;
    s1.insert(0, 3, '*');
    cout << s1 << endl;
}
void test2()
{
    mystring s1("hello");
    cout << s1 << endl;
    s1.insert(2, "**");
    cout << s1 << endl;
    s1.erase(2, 3);
    cout << s1 << endl;
    s1.erase(1,20);
    cout << s1 << endl;
}
void test3()
{
    mystring s1("hello");
    ssize_t i = s1.find('l');
    cout << i << endl;
    i = s1.find("l0");
    cout << i << endl;
    s1.resize(3);
    cout << s1 << endl;
    s1.resize(8, 'x');
    cout << s1 << endl;
}

void test4()
{
    mystring s1("hello");
    cout << s1 << endl;
    s1 += '\0';
    s1 += "124324";
    cout << s1 << endl;
    cout << s1.c_str() << endl;
}
void test5()
{
    mystring s1("hello");
    mystring s2("helloccwse");
    cout << (s1 == s2) << endl;
    cout << (s1 > s2) << endl;
    cout << (s1 < s2) << endl;
    cout << (s1 <= s2) << endl;
    cout << (s1 >= s2) << endl;
    cout << (s1 != s2) << endl;
}
void test6()
{
    mystring s1("hello");
    mystring s2("helloccwse");
    cout << s1 << endl;
    cout << s2 << endl;
    mystring s3;
    s3="hello";
    cout << s3 << endl;
}
void test7()
{
    mystring s1("hello");
    mystring s2;
    s2 = s1;
    cout << s2 << endl;
}

int main()
{
    test6();
    return 0;
}