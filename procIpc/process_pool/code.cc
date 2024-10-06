#include <iostream>
#include <unistd.h>
#include <cstring>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>
#include <vector>
#include "task.hpp"
#include <time.h>

#define num 5

using namespace std;

int wait_command(int fd, bool &quit)
{
    uint32_t command = 0;
    ssize_t size = read(fd, &command, sizeof(uint32_t));
    if (size == 0)
    {
        quit = true;
        return -1;
    }
    assert(size == sizeof(uint32_t));
    return command;
}

void send_wakeup(pid_t pid, int fd, uint32_t command)
{
    // cout << "cin command: " << command << endl;
    // cout << "descripe : " << descripe[command] << endl;
    write(fd, &command, sizeof(command));
    cout << "call process : " << pid << ", execute : " << descripe[command] << ", though : " << fd << endl;
}

int main()
{
    vector<pair<pid_t, int>> mapping_table;
    load();
    // 创建多个进程
    for (int i = 0; i < num; i++)
    {
        int pipefd[2] = {0};
        pipe(pipefd); // 创建管道
        pid_t id = fork();
        assert(id != -1);
        (void)id;

        if (id == 0)
        {
            // child
            close(pipefd[1]); 
            while (true) // 等待命令+执行任务
            {
                bool quit = false;
                int command = wait_command(pipefd[0], quit);
                if (quit)
                    break;
                if (command >= 0 && command < task_size())
                {
                    callbacks[command]();
                    cout << "success" << endl;
                }
                else
                {
                    cout << "非法command" << endl;
                }
            }
            cout << "process exit : " << getpid() << endl;
            exit(0);
        }
        // parent
        close(pipefd[0]);
        mapping_table.push_back(pair<pid_t, int>(id, pipefd[1])); // 保存[通过哪个fd发送命令到子进程]
        // 在派发任务时,需要使每个进程都能被使用 -- 单机版的负载均衡
    }
    // 派发任务
    srand((unsigned int)time(nullptr) ^ getpid() ^ 1243225324);
    cout << "****************************************" << endl;
    cout << "*  1. show functions  2. send command  *" << endl;
    cout << "****************************************" << endl;
    cout << "please select : " << endl;
    while (true)
    {
        int flag = 0;
        int select = 0, command = 0;
        cin >> select;
        if (select == 1)
        {
            show_handler();
        }
        else if (select == 2)
        {
            cout << "please enter your command : " << endl;
            cin >> command; // 任务
            // cout << "command: " << command << endl;
            cout << endl;
            int i = rand() % mapping_table.size(); // 进程
            send_wakeup(mapping_table[i].first, mapping_table[i].second, command);
        }
        else
        {
            cout << "select error" << endl;
        }
        sleep(1);
        while (true)
        {
            cout << endl
                 << "continue : y/n ?" << endl;
            char c = 0;
            cin >> c;
            getchar();
            if (c == 'y' || c == 'Y')
            {
                cout << "****************************************" << endl;
                cout << "*  1. show functions  2. send command  *" << endl;
                cout << "****************************************" << endl;
                cout << "please select : " << endl;
                flag = 1;
                break;
            }
            else if (c == 'n' || c == 'N')
            {
                cout << "exit" << endl;
                flag = 2;
                break;
            }
            else
            {
                cout << "select error , please retry" << endl;
                continue;
            }
            getchar();
        }
        if (flag == 1)
        {
            continue;
        }
        else if (flag == 2)
        {
            break;
        }
    }
    // 关闭fd+进程
    for (const auto &i : mapping_table)
    {
        close(i.second); // 关闭每个进程之间通信的写端,使子进程读到0从而退出
    }
    for (const auto &i : mapping_table)
    {
        waitpid(i.first, nullptr, 0); // 等待每个退出的子进程,回收资源
    }
    return 0;
}
