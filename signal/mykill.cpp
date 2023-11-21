#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

void usage(string arr)
{
    cout << arr << " "
         << "illegal";
}
int main(int argc, char *argv[]) // 模拟kill命令 -- ./mykill 2 pid
{
    if (argc != 3)
    {
        usage(argv[0]);
        cout << endl;
        exit(0);
    }
    int signum = atoi(argv[1]);
    int pid = atoi(argv[2]);
    kill(pid, signum);

    return 0;
}
