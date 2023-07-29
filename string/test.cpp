#include "test.hpp"

void test()
{
    mystring::string s1("hello");
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
    mystring::string s1("hello");
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
    mystring::string s1("hello");
    cout << s1 << endl;
    s1.insert(2, "**");
    cout << s1 << endl;
    s1.erase(2, 3);
    cout << s1 << endl;
    s1.erase(1.20);
    cout << s1 << endl;
}
void test3()
{
    mystring::string s1("hello");
    ssize_t i = s1.find('l');
    cout << i << endl;
    i = s1.find("l0");
    cout << i << endl;
    s1.resize(3);
    cout << s1 << endl;
    s1.resize(8, 'x');
    cout << s1 << endl;
}

void test4(){
    mystring::string s1("hello");
    cout << s1 << endl;
    s1+='\0';
    s1+="124324";
    cout << s1 << endl;
    cout<<s1.c_str()<<endl;
}
void test5(){
    mystring::string s1("hello");
    mystring::string s2("helloccwse");
    cout<<(s1==s2)<<endl;
    cout<<(s1>s2)<<endl;
    cout<<(s1<s2)<<endl;
    cout<<(s1<=s2)<<endl;
    cout<<(s1>=s2)<<endl;
    cout<<(s1!=s2)<<endl;
}
void test6(){
    mystring::string s1("hello");
    mystring::string s2("helloccwse");
    swap(s1,s2);
    cout<<s1<<endl;
    cout<<s2<<endl;
    mystring::string s3;
    s3=s1;
    cout<<s3<<endl;
}

int main()
{
    test6();
    return 0;
}