#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <atomic>
#include <time.h>

using namespace std;

int count = 10000;
void *func1(void *args)
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

struct data
{
    data(pthread_mutex_t *px, const string &n) : pmux(px), name(n)
    {
    }
    pthread_mutex_t *pmux;
    string name;
};
pthread_mutex_t g_mux = PTHREAD_MUTEX_INITIALIZER;

void *func2(void *args)
{
    data *info = (data *)args;
    while (true)
    {
        int n = pthread_mutex_lock(info->pmux); // 从if就开始访问count了
        assert(n == 0);
        if (count > 0)
        {
            usleep(1000); // 模拟可能花费的时间

            cout << info->name << " : " << count << endl;
            --count;
            n = pthread_mutex_unlock(info->pmux);
            assert(n == 0);
        }
        else
        {
            n = pthread_mutex_unlock(info->pmux); // 注意,一定要保证上锁后可以解锁
            assert(n == 0);
            break;
        }
        usleep(1000); // 模拟抢票成功后的花费时间(为了尽量让不同线程去抢票)
    }
    return nullptr;
}

atomic<int> num(10000);

void *func3(void *args)
{
    while (true)
    {
        int currentCount = num.load(); // 使用原子加载获取当前值
        if (currentCount >= 0)
        {
            usleep(1000); // 模拟可能花费的时间
            cout << currentCount << endl;
            num.store(currentCount - 1); // 使用原子存储更新值
        }
    }
    return nullptr;
}

#define thread_num 2
void test1()
{
    time_t start, end;
    start = time(nullptr);
    pthread_mutex_t mux;
    pthread_mutex_init(&mux, nullptr);

    pthread_t tid;
    vector<pthread_t> tids;
    for (size_t i = 0; i < thread_num; ++i)
    {
        string t = "thread";
        t += to_string(i + 1);
        data *pd = new data(&mux, t);
        pthread_create(&tid, nullptr, func2, (void *)pd);
        tids.push_back(tid);
    }
    for (auto it : tids)
    {
        pthread_join(it, nullptr);
    }
    pthread_mutex_destroy(&mux);
    end = time(nullptr);
    cout << "time : " << (end - start) << "s" << endl;
}

int main()
{
    test1();
    return 0;
}