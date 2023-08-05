#include "test.hpp"

using namespace bit;

void test1()
{
    myvector<int> s1;
    s1.push_back(1);
    s1.push_back(2);
    s1.push_back(3);
    s1.push_back(4);
    s1.push_back(5);

    myvector<int>::iterator pos = s1.begin() + 2;
    pos = s1.insert(pos, 10);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;

    s1.resize(20, 2);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
}
void test2()
{
    myvector<int> s1;
    s1.push_back(1);
    s1.push_back(2);
    s1.push_back(3);
    s1.push_back(4);
    s1.push_back(5);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;

    myvector<int>::iterator pos = s1.begin() + 2;
    cout << *pos << endl;

    s1.pop_back();
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;

    pos = s1.erase(pos);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
}
void test3()
{
    myvector<int> s1;
    s1.push_back(1);
    s1.push_back(2);
    s1.push_back(2);
    s1.push_back(3);
    s1.push_back(4);
    s1.push_back(6);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
    auto it = s1.begin();
    while (it != s1.end())
    {
        if (*it % 2 == 0)
        {
            s1.erase(it);
        }
        else
        {
            ++it;
        }
    }
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
}
void test4()
{
    myvector<int> s1(10, 1);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
    string str = "asrfsdfsd";
    myvector<char> s2(str.begin(), str.end());
    for (auto c : s2)
    {
        cout << c << " ";
    }
    cout << endl;
    
    myvector<int> s3;
    s3 = s1;
    for (auto c : s3)
    {
        cout << c << " ";
    }
    cout << endl;

    myvector<int> s4(s1);
    for (auto c : s4)
    {
        cout << c << " ";
    }
    cout << endl;
}

void test5()
{
    myvector<int> s1;
    s1.push_back(1);
    s1.push_back(2);
    s1.push_back(2);
    s1.push_back(3);
    s1.push_back(4);
    s1.push_back(6);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
    auto pos = s1.end() - 1;
    pos = s1.erase(pos);
    for (auto c : s1)
    {
        cout << c << " ";
    }
    cout << endl;
    cout << *pos << endl;
}
int main()
{
    test5();
    return 0;
}