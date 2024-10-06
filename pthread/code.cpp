#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <cstring>
#include <sys/syscall.h>
#include <vector>
#include <fcntl.h>

using namespace std;

void *func(void *args)
{
    string arr = (char *)args;
    while (true)
    {
        cout << "im " << arr << " : " << getpid() << endl;
        sleep(1);
    }
    return nullptr;
}
void test1()
{
    pthread_t tid;
    char arr[64];
    for (int i = 0; i < 2; ++i)
    {
        snprintf(arr, sizeof(arr), "%s %d", "thread", i + 1);
        pthread_create(&tid, nullptr, func, (void *)arr);
        sleep(1);
    }
    while (true)
    {
        cout << "im main thread  : " << getpid() << endl;
        sleep(3);
    }
}

void *func2(void *args) // 5s后退出
{
    int count = 0;
    // int a = 1;
    // a /= 0;

    char *arr = new char[20];
    strcpy(arr, "im dead"); // 注意,返回的信息最好是全局变量/动态申请的资源,否则在其他线程使用该变量时,该线程已经退出了,属于野指针

    pthread_exit((void *)arr);
    while (true)
    {
        cout << "im new thread " << endl;
        sleep(1);
        ++count;
        if (count == 4)
        {
            break;
        }
    }
    cout << "new thread quit" << endl;
    // exit(1);
    return (void *)arr;
}
void test2()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, func2, nullptr);
    int count = 0;

    void *retval = nullptr; // 验证阻塞等待
    int ret = pthread_join(tid, &retval);

    // cout << "join " << strerror(ret) << ", the message : " << (char *)retval << endl;
    cout << "join " << strerror(ret) << ", the message : " << (const char *)retval << endl;

    delete[] (char *)retval;

    while (true)
    {
        cout << "im main thread  : " << getpid() << endl;
        sleep(1);
        count++;
        if (count == 3)
        {
            break;
        }
    }
    // void *retval = nullptr;
    // int ret = pthread_join(tid, &retval);
    // cout << "join " << strerror(ret) << ", the message : " << (char *)retval << endl;
    cout << "main quit " << endl;
}

void *func3(void *args) // 验证取消
{
    char *arr = new char[20];
    strcpy(arr, "im dead"); // 注意,返回的信息最好是全局变量/动态申请的资源,否则在其他线程使用该变量时,该线程已经退出了,属于野指针
    while (true)
    {
        cout << "im new thread " << endl;
        sleep(1);
    }
    cout << "new thread quit" << endl;
    return (void *)arr;
}
void test3()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, func3, nullptr);
    int count = 0;
    while (true)
    {
        cout << "im main thread  : " << getpid() << endl;
        sleep(2);
        count++;
        if (count == 3)
        {
            break;
        }
    }

    pthread_cancel(tid);
    cout << "cancel success " << endl;

    void *retval = nullptr; // 验证阻塞等待
    int ret = pthread_join(tid, &retval);
    cout << "join " << strerror(ret) << ", the message : " << (char *)retval << endl;
    // cout << "join success" << (long long)retval << endl;
    cout << "main quit " << endl;
}

void *func4(void *args)
{
    sleep(1);
    pthread_t tid2 = pthread_self();
    // cout << "my tid : " << tid2 << endl;

    pthread_t tid1 = syscall(SYS_gettid);
    cout << "my tid : " << tid1 << endl;
    sleep(5);
    return nullptr;
}
void test4()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, func4, nullptr);
    // printf("%lu %p\n", tid, tid);
    pthread_join(tid, nullptr);
}

int count = 1000;
void *func5(void *args)
{
    while (true)
    {
        if (count >= 0)
        {
            usleep(1000); // 模拟可能花费的时间
            cout << count << endl;
            --count;
        }
    }
    return nullptr;
}
void test5()
{
    pthread_t tid;
    vector<pthread_t> tids;
    for (size_t i = 0; i < 3; ++i)
    {
        pthread_create(&tid, nullptr, func5, nullptr);
        tids.push_back(tid);
    }
    for (auto it : tids)
    {
        pthread_join(it, nullptr);
    }
}

__thread int g_val = 0;
void *func6(void *args)
{
    int fd = *((int *)args);
    while (true)
    {
        int n=write(fd, "111", 3);
        if(n<=0){
            cout<<"test quit"<<endl;
        }
        sleep(1);
        ++g_val;
        cout << "im new : " << g_val << " " << &g_val << endl;
    }
}
void test6()
{
    pthread_t tid;
    int fd = open("test.txt", O_CREAT | O_RDWR);
    pthread_create(&tid, nullptr, func6, &fd);

    while (true)
    {
        write(fd, "xxx", 3);
        sleep(2);
        close(fd);
        ++g_val;
        cout << "im main : " << g_val << " " << &g_val << endl;
    }
}
int main()
{
    test6();
    return 0;
}