#include <pthread.h>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <semaphore.h>
#include <iostream>

struct thread
{
    pthread_t tid_;
    std::string name_;
};

template <class T>
class thread_pool
{
private:
    void lock()
    {
        pthread_mutex_lock(&mutex_);
    }
    void unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }
    void wait()
    {
        pthread_cond_wait(&cond_, &mutex_);
    }
    void signal()
    {
        pthread_cond_signal(&cond_);
    }
    T pop()
    {
        T t = task_.front();
        task_.pop();
        return t;
    }
    bool is_empty()
    {
        return task_.size() == 0;
    }

    static void *entry(void *args) // 类成员会有this参数,但入口函数不允许有多余参数
    {
        thread_pool<T> *tp = static_cast<thread_pool<T> *>(args); // this指针,用于拿到成员变量/函数
        while (true)
        {
            tp->lock();
            while (tp->is_empty())
            {
                tp->wait();
            }
            T t = tp->pop();
            tp->unlock();

            t();
        }
        return nullptr;
    }

public:
    static thread_pool<T> *get_instance(int num = 5)
    {
        // 如果这样写,虽然保证了安全,但会在创建对象后,线程依然线性运行
        //  pthread_mutex_lock(&single_mutex_);
        //  if (myself_ == nullptr)
        //  {
        //      myself_ = new thread_pool<T>(num);
        //  }
        //  pthread_mutex_unlock(&single_mutex_);

        if (myself_ == nullptr) // 再加一层判断,就可以提高效率
        {
            pthread_mutex_lock(&single_mutex_);
            if (myself_ == nullptr)
            {
                myself_ = new thread_pool<T>(num);
                //std::cout << "get instance success" << std::endl;
            }
            pthread_mutex_unlock(&single_mutex_);
        }

        return myself_;
    }
    void init()
    {
        for (size_t i = 0; i < num_; ++i)
        {
            pthread_create(&(threads_[i].tid_), nullptr, entry, this);
            pthread_detach(threads_[i].tid_);
        }
    }
    void push(const T data)
    {
        lock();

        task_.push(data);
        signal(); // 放在锁内,确保只有当前线程执行唤醒操作,不然可能会有多次操作

        unlock();
    }

private:
    thread_pool(int num = 5)
        : num_(num), threads_(num)
    {
        pthread_cond_init(&cond_, nullptr);
        pthread_mutex_init(&mutex_, nullptr);
    }
    ~thread_pool()
    {
        pthread_cond_destroy(&cond_);
        pthread_mutex_destroy(&mutex_);
    }

private:
    std::vector<thread> threads_;
    std::queue<T> task_;
    int num_;

    pthread_cond_t cond_;
    pthread_mutex_t mutex_;

    static thread_pool<T> *myself_; // 每次外部想要线程池对象时,返回的都是这一个(只有静态成员变量,才能保证一个类只有一个)
    static pthread_mutex_t single_mutex_;
};

template <class T>
thread_pool<T> *thread_pool<T>::myself_ = nullptr;

template <class T>
pthread_mutex_t thread_pool<T>::single_mutex_ = PTHREAD_MUTEX_INITIALIZER;