#include <iostream>
#include <pthread.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <unistd.h>

using namespace std;

#define TH_NUM 3
// typedef void *(*func_t)(void *);
typedef void *(*func_t)(const string &name, pthread_mutex_t *pmutex, pthread_cond_t *pcond);
volatile bool quit = false; // 让编译器不做优化,每次访问都需要从内存中拿取数据

struct Data
{
    Data(const string &name, func_t func, pthread_mutex_t *pmutex, pthread_cond_t *pcond)
        : _name(name), _func(func), _pcond(pcond), _pmutex(pmutex) {}
    string _name;
    func_t _func;
    pthread_mutex_t *_pmutex;
    pthread_cond_t *_pcond;
};

void *func1(const string &name, pthread_mutex_t *pmutex, pthread_cond_t *pcond)
{
    // while (true)
    // {
    //     pthread_cond_wait(pcond, pmutex);
    //     cout << "im " << name << " , "
    //          << "work 1 ... " << endl;
    // }

    while (!quit)
    {
        pthread_mutex_lock(pmutex);
        pthread_cond_wait(pcond, pmutex);
        cout << "im " << name << " , "
             << "work 1 ... " << endl;
        pthread_mutex_unlock(pmutex);
    }
    cout << name << " quit" << endl;
    return nullptr;
}
void *func2(const string &name, pthread_mutex_t *pmutex, pthread_cond_t *pcond)
{
    // while (true)
    // {
    //     usleep(10);
    //     pthread_cond_wait(pcond, pmutex);
    //     cout << "im " << name << " , "
    //          << "work 2 ... " << endl;
    // }

    while (!quit)
    {
        pthread_mutex_lock(pmutex);
        pthread_cond_wait(pcond, pmutex);
        cout << "im " << name << " , "
             << "work 2 ... " << endl;
        pthread_mutex_unlock(pmutex);
    }
    cout << name << " quit" << endl;
    return nullptr;
}
void *func3(const string &name, pthread_mutex_t *pmutex, pthread_cond_t *pcond)
{
    // while (true)
    // {
    //     usleep(20);
    //     pthread_cond_wait(pcond, pmutex);
    //     cout << "im " << name << " , "
    //          << "work 3 ... " << endl;
    // }

    while (!quit)
    {
        pthread_mutex_lock(pmutex);
        pthread_cond_wait(pcond, pmutex);
        cout << "im " << name << " , "
             << "work 3 ... " << endl;
        pthread_mutex_unlock(pmutex);
    }
    cout << name << " quit" << endl;
    return nullptr;
}

void *entry(void *args)
{
    Data *pd = (Data *)args;
    pd->_func(pd->_name, pd->_pmutex, pd->_pcond); // 将data中的成员作为另一成员的参数
}

void test1()
{
    func_t funcs[TH_NUM] = {func1, func2, func3}; // 存放线程任务
    vector<pthread_t> tids;                       // 存放所有线程的tid

    // 初始化锁和条件变量
    pthread_cond_t cond;
    pthread_cond_init(&cond, nullptr);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    for (int i = 0; i < TH_NUM; ++i)
    {
        // string name = "pthread";
        // name += to_string(i + 1);
        // pthread_t tid;
        // pthread_create(&tid, nullptr, funcs[i], (void *)&name);
        // tids.push_back(tid);

        string arr = "pthread";
        arr += to_string(i + 1);
        Data *pdata = new Data(arr, funcs[i], &mutex, &cond); // 在这里为线程分配任务

        pthread_t tid;
        pthread_create(&tid, nullptr, entry, (void *)pdata); // 所有线程都可以使用统一的入口函数entry,但执行的是不同的任务
        tids.push_back(tid);
    }
    // 最开始,线程首先都按照顺序处于等待状态
    sleep(3);
    // 3s后,唤醒所有线程

    int count = 3;
    while (true)
    {
        // pthread_cond_broadcast(&cond);

        // pthread_cond_signal(&cond);

        --count;
        if (!count)
        {
            quit = true; // 让所有线程退出
            cout << "quit true" << endl;
            pthread_cond_broadcast(&cond); // 修改quit后,需要让线程醒来才能获取到quit
            break;
        }
        pthread_cond_broadcast(&cond);
        sleep(1);
    }

    for (auto tid : tids)
    {
        pthread_join(tid, nullptr);
    }
    cout << "join success" << endl;

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
int main()
{
    test1();
    return 0;
}