#include <pthread.h>
#include <vector>
#include <stdlib.h>
#include <semaphore.h>

using namespace std;

static const int def = 5;
template <class T>
class ring_queue
{
private:
    void P(sem_t *sem)
    {
        sem_wait(sem);
    }
    void V(sem_t *sem)
    {
        sem_post(sem);
    }
    void lock(pthread_mutex_t *mutex)
    {
        pthread_mutex_lock(mutex);
    }
    void unlock(pthread_mutex_t *mutex)
    {
        pthread_mutex_unlock(mutex);
    }

public:
    ring_queue(int num = def)
        : c_i(0), p_i(0), capacity_(num), rq_(num)
    {
        sem_init(&data_num_, 0, 0);
        sem_init(&cap_num_, 0, num);
        pthread_mutex_init(&c_mutex_, nullptr);
        pthread_mutex_init(&p_mutex_, nullptr);
    }
    ~ring_queue()
    {
        sem_destroy(&data_num_);
        sem_destroy(&cap_num_);
        pthread_mutex_destroy(&c_mutex_);
        pthread_mutex_destroy(&p_mutex_);
    }
    void push(const T data)
    {
        P(&cap_num_);

        lock(&p_mutex_);
        rq_[p_i] = data;
        p_i = (p_i + 1) % capacity_;
        unlock(&p_mutex_);

        V(&data_num_);
    }
    void pop(T &data)
    {
        P(&data_num_);

        lock(&c_mutex_);
        data = rq_[c_i];
        c_i = (c_i + 1) % capacity_;
        unlock(&c_mutex_);

        V(&cap_num_);
    }

private:
    vector<T> rq_;
    int c_i;
    int p_i;
    int capacity_;

    sem_t data_num_;
    sem_t cap_num_;

    pthread_mutex_t c_mutex_;
    pthread_mutex_t p_mutex_;
};