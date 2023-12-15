#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <cassert>
#include <queue>

using namespace std;

const int def_capacity = 10;

template <class T>
class BlockQueue
{
public:
    BlockQueue(int capacity = def_capacity)
        : capacity_(capacity)
    {
        pthread_mutex_init(&mutex_, nullptr);
        pthread_cond_init(&is_there_, nullptr);
        pthread_cond_init(&is_full_, nullptr);
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&is_there_);
        pthread_cond_destroy(&is_full_);
    }
    void push(const T &in)
    { // push需要保证和pop互斥,且队列不能满
        pthread_mutex_lock(&mutex_);
        if (isfull())
        {
            pthread_cond_wait(&is_full_, &mutex_);
        }
        bq_.push(in);
        // 生产完,就可以通知消费者
        pthread_cond_signal(&is_there_);
        pthread_mutex_unlock(&mutex_);
    }
    void pop(T &out) // 和push同理
    {
        pthread_mutex_lock(&mutex_);
        if (isempty())
        {
            pthread_cond_wait(&is_there_, &mutex_);
        }
        out = bq_.front();
        bq_.pop();
        // 消费完,就可以通知生产者
        pthread_cond_signal(&is_full_);
        pthread_mutex_unlock(&mutex_);
    }
    bool isfull()
    {
        return capacity_ == bq_.size();
    }
    bool isempty()
    {
        return 0 == bq_.size();
    }

private:
    queue<T> bq_;
    pthread_mutex_t mutex_;
    pthread_cond_t is_there_;
    pthread_cond_t is_full_;
    int capacity_;
};