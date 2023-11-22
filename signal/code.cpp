#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <functional>
using namespace std;

int count_num = 0;

void func(int signum)
{
    cout << "im " << getpid() << "i got a signal : " << signum << endl;
    // cout<<count_num<<endl;
    // exit(1);
}
void test1()
{
    signal(2, func);
    while (1)
    {
        cout << "im here : " << getpid() << endl;
        sleep(1);
    }
}
void test2()
{
    signal(SIGFPE, func);
    int a = 1;
    // a /= 0;
    while (1)
    {
        sleep(1);
    }
}
void test3()
{
    alarm(1);
    int count = 0;
    while (1)
    {
        cout << count << endl;
        ++count;
    }
}
void test4()
{
    alarm(1);
    signal(SIGALRM, func);
    while (1)
    {
        ++count_num;
    }
}

vector<function<void()>> callbacks;
void log()
{
    cout << "loging" << endl;
}
void show_user()
{
    if (fork() == 0)
    {
        execl("/usr/bin/who", "who", nullptr);
        exit(1);
    }
    wait(nullptr);
}
void func_test(int signum)
{
    signum = 0;
    for (auto &it : callbacks)
    {
        it();
    }
    alarm(1);
}
void test5()
{
    callbacks.push_back(log);
    callbacks.push_back(show_user);
    alarm(1);
    signal(SIGALRM, func_test);
    while (1)
    {
        ;
    }
}

void test6()
{
    struct sigaction sa;
    sigset_t block;
    sigemptyset(&block);

    sa.sa_handler = func;
    sa.sa_mask = block; // 没有屏蔽任何信号

    sigaction(2, &sa, NULL);
    while (1)
    {
        sleep(1);
    }
}

void show_sigset()
{
    sigset_t pending;
    sigpending(&pending);
    for (int i = 1; i <= 31; ++i)
    {
        if (sigismember(&pending, i))
        {
            cout << 1;
        }
        else
        {
            cout << 0;
        }
    }
    cout << endl;
}
void show_block()
{
    sigset_t oldset;
    sigprocmask(SIG_BLOCK, NULL, &oldset);
    for (int i = 1; i <= 31; ++i)
    {
        if (sigismember(&oldset, i))
        {
            cout << 1;
        }
        else
        {
            cout << 0;
        }
    }
    cout << endl;
}

void func1(int signum)
{
    cout << "im " << getpid() << "i got a signal : " << signum << endl;
    while (true)
    {
        show_sigset();
        show_block();
        cout<<endl;
        sleep(1);
    }
}
void test7()
{
    cout << "im " << getpid() << endl;
    signal(2, func1);
    while (true)
    {
        show_sigset();
        show_block();
        cout<<endl;
        sleep(1);
    }
}
int main()
{
    test7();
    return 0;
}
