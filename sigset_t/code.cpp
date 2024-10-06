#include <signal.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
using namespace std;

void showset(sigset_t *s)
{
    for (int i = 1; i <= 31; ++i)
    {
        if (sigismember(s, i))
        {
            cout << "1";
        }
        else
        {
            cout << "0";
        }
    }
    cout << endl;
}
void func(int signum)
{
    cout << "我收到了" << signum << "号信号" << endl;
}
void test1()
{
    sigset_t s, olds;
    sigset_t pending;
    signal(2, func);
    sigemptyset(&s);
    sigemptyset(&olds);

    sigaddset(&s, 2);                  // 将2号信号添加进去
    sigprocmask(SIG_BLOCK, &s, &olds); // 屏蔽2号信号
    int count = 0;
    while (1)
    {
        ++count;
        sigpending(&pending);
        showset(&pending);
        sleep(1);
        if (count == 10)
        {
            sigprocmask(SIG_UNBLOCK, &s, &olds); // 取消2号信号的屏蔽
        }
    }
}
void block_sig(sigset_t *s)
{
    sigfillset(s);
    sigprocmask(SIG_SETMASK, s, nullptr); // 将所有信号阻塞
}
void test2()
{
    sigset_t s, pending;
    sigemptyset(&s);
    block_sig(&s);
    while (1)
    {
        sigpending(&pending);
        showset(&pending);
        sleep(1);
    }
}
void test3()
{
    for (int i = 1; i <= 31; ++i)
    {
        signal(i, func);
    }
    while (1)
    {
        sleep(1);
    }
}
int main()
{
    test3();
    return 0;
}