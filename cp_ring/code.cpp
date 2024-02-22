#include "ring_queue.hpp"
#include "Task.hpp"
#include <random>
#include <time.h>
#include <unistd.h>

struct thread
{
    thread(ring_queue<Task> *rq, string name)
        : rq_(rq), name_(name)
    {
    }
    ring_queue<Task> *rq_;
    string name_;
};

void *consume_single(void *args)
{
    ring_queue<Task> *rq = static_cast<ring_queue<Task> *>(args);
    while (true)
    {
        usleep(20);
        Task t;
        rq->pop(t);
        // 处理任务
        t();
        cout << "im consumer,task is " << t.get_task() << " ,result is " << t.get_result() << " ,code is " << t.get_code() << endl;
        // sleep(1);
    }
}
void *product_single(void *args)
{
    ring_queue<Task> *rq = static_cast<ring_queue<Task> *>(args);
    while (true)
    {
        // 生产任务
        int x = rand() % 10 + 1;
        int y = rand() % 5;
        char op = symbol[rand() % (sym_size - 1)];
        Task t(x, y, op);

        rq->push(t);
        cout << "im producer,task is " << t.get_task() << endl;
        sleep(1);
    }
}

void *consume_multiple(void *args)
{
    thread *t = static_cast<thread *>(args);
    ring_queue<Task> *rq = t->rq_;

    while (true)
    {
        Task task;
        rq->pop(task);

        // 处理任务
        task();
        cout << "im " << t->name_ << ",task is " << task.get_task() << " ,result is " << task.get_result() << " ,code is " << task.get_code() << endl;
        // sleep(1);
    }
}
void *product_multiple(void *args)
{
    thread *t = static_cast<thread *>(args);
    ring_queue<Task> *rq = t->rq_;
    while (true)
    {
        // 生产任务
        int x = rand() % 10 + 1;
        usleep(30);
        int y = rand() % 5;
        usleep(30);
        char op = symbol[rand() % (sym_size - 1)];
        Task task(x, y, op);

        rq->push(task);
        cout << "im " << t->name_ << ",task is " << task.get_task() << endl;
        sleep(1);
    }
}

void single()
{
    ring_queue<Task> *rq = new ring_queue<Task>(10);
    pthread_t c, p;

    pthread_create(&c, nullptr, consume_single, &rq);
    pthread_create(&p, nullptr, product_single, &rq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);
}

const int p_num = 4, c_num = 2;

void multiple()
{
    ring_queue<Task> *rq = new ring_queue<Task>;
    pthread_t c[c_num], p[p_num];

    for (int i = 0; i < p_num; ++i)
    {
        string name = "producer-" + to_string(i + 1);
        thread *t = new thread(rq, name);
        pthread_create(&p[i], nullptr, product_multiple, t);
    }
    for (int i = 0; i < c_num; ++i)
    {
        string name = "consumer-" + to_string(i + 1);
        thread *t = new thread(rq, name);
        pthread_create(&c[i], nullptr, consume_multiple, t);
    }

    for (size_t i = 0; i < p_num; ++i)
    {
        pthread_join(p[i], nullptr);
    }
    for (size_t i = 0; i < c_num; ++i)
    {
        pthread_join(c[i], nullptr);
    }
}
int main()
{
    srand(time(nullptr) ^ getpid());
    // single();
    multiple();

    return 0;
}