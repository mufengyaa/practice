#include <iostream>
#include <unistd.h>
#include <cstring>
#include <assert.h>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/types.h>

using namespace std;

void test1()
{
    int pipefd[2] = {0}; // 会打开两个文件(用于读,写)
    int ret = pipe(pipefd);
    assert(ret != -1);
    (void)ret; // release下assert不会显示,如果不使用这个ret,会有警告

#ifdef DEBUG
    cout << pipefd[0] << endl;
    cout << pipefd[1] << endl;
#endif

    pid_t fd = fork(); // 创建子进程,让他和父进程通信
    assert(fd != -1);
    if (fd == 0)
    {
        // 子进程 -- 读
        close(pipefd[1]);
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        while (true)
        {
            ssize_t size = read(pipefd[0], buffer, sizeof(buffer) - 1);
            if (size > 0)
            {
                buffer[size] = 0;
                cout << "im child ,"
                     << "message: " << buffer;
                // exit(0);
            }
            else if (size == 0)
            {
                cout << "read end , im quit" << endl;
                break;
            }
        }
        exit(0);
    }
    // 父进程 -- 写
    // cout<<getpid()<<endl;
    close(pipefd[0]);
    string message = "im parent , im writing";
    char buffer[1024];
    int count = 0;
    memset(buffer, 0, sizeof(buffer));
    while (true)
    {
        snprintf(buffer, sizeof(buffer) - 1, "pid:%d,%s,%d\n", getpid(), message.c_str(), count++);
        ssize_t size = write(pipefd[1], buffer, strlen(buffer));
        // cout << count << endl;
        if (size < 0)
        {
            cout << "write fail" << endl;
        }
        // if(count==5){
        //     //close(pipefd[1]);
        //     exit(0);
        //     cout<<"parent quit success"<<endl;
        //     break;
        // }
        sleep(1);
    }
    pid_t flag = waitpid(fd, nullptr, 0);
    if (flag <= 0)
    {
        cout << "wait fail" << endl;
    }
    assert(flag > 0);
    (void)flag;

    cout << "wait success" << endl;
}

void test2()
{
    int pipefd[2] = {0}; // 会打开两个文件(用于读,写)
    int ret = pipe(pipefd);
    assert(ret != -1);
    (void)ret; // release下assert不会显示,如果不使用这个ret,会有警告

    pid_t pid = fork();
    if (pid == 0)
    {
        char arr[1024];
        memset(arr, 0, sizeof(arr));
        close(pipefd[1]);
        int size = read(pipefd[0], arr, sizeof(arr) - 1);
        cout << arr << endl;
        exit(0);
    }
    const char *arr = "im father";
    close(pipefd[0]);
    int size = write(pipefd[1], arr, strlen(arr));
    if (size < 0)
    {
        perror("write");
        exit(2);
    }
    wait(nullptr);
}
int main()
{
    test1();
    return 0;
}
