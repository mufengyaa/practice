#include "code.hpp"

using namespace bit;

void test()
{
    mystack<int, vector<int>> s;
    s.push(1);
    s.push(2);
    s.push(3);
    s.push(4);
    while (!s.empty())
    {
        cout << s.top() << " ";
        s.pop();
    }
    cout << endl;

    mystack<int, list<int>> ss;
    ss.push(1);
    ss.push(2);
    ss.push(3);
    ss.push(4);
    while (!ss.empty())
    {
        cout << ss.top() << " ";
        ss.pop();
    }
    cout << endl;
}
int main()
{
    test();
    return 0;
}