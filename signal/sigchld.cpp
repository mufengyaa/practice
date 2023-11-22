#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <vector>
#include <sys/wait.h>
#include <sys/types.h>
#include <functional>

using namespace std;

void func(int signum)
{
    cout << "im " << getpid() << "i got a signal : " << signum << endl;
}

void test1()
{
    if (fork() == 0)
    {
        cout << "im child : " << getpid() << endl;
        sleep(2);
        exit(0);
    }
    while (true)
    {
        ;
    }
}
void test2()
{
    signal(SIGCHLD, func);
    if (fork() == 0)
    {
        cout << "im child : " << getpid() << endl;
        sleep(2);
        exit(0);
    }
    while (true)
    {
        ;
    }
}
void func_wait(int signum)
{
    cout << "im " << getpid() << "i got a signal : " << signum << endl;
    wait(nullptr);
    cout << "success waited :" << getpid() << endl;
}
void test3()
{
    // signal(SIGCHLD,SIG_IGN);
    signal(SIGCHLD, func_wait);
    if (fork() == 0)
    {
        cout << "im child : " << getpid() << endl;
        sleep(100);
    }
    while (true)
    {
        ;
    }
}
int main()
{
    test3();
    return 0;
}