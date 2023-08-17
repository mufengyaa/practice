#include"code.hpp"

using namespace bit;
void test(){
    myqueue<int,list<int>> q;
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    while (!q.empty())
    {
        cout << q.front() << " ";
        q.pop();
    }
    cout << endl;

    myqueue<int> qq;
    qq.push(1);
    qq.push(2);
    qq.push(3);
    qq.push(4);
    while (!qq.empty())
    {
        cout << qq.front() << " ";
        qq.pop();
    }
    cout << endl;
}
int main(){
    test();
    return 0;
}