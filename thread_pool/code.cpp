#include "thread_pool.hpp"
#include "Task.hpp"
#include <random>
#include <time.h>
#include <unistd.h>

int main()
{
    // thread_pool<Task> *tp = new thread_pool<Task>;
    // tp->init();
    // while (true)
    // {
    //     int x = rand() % 10 + 1;
    //     usleep(30);
    //     int y = rand() % 5;
    //     usleep(30);
    //     char op = symbol[rand() % (sym_size - 1)];
    //     Task task(x, y, op);
    //     cout << "task is " << task.get_task() << endl;

    //     tp->push(task);
    //     sleep(1);
    // }
    // tp->join();

    sleep(1);
    thread_pool<Task>::get_instance(5)->init();
    while (true)
    {
        int x = rand() % 10 + 1;
        usleep(30);
        int y = rand() % 5;
        usleep(30);
        char op = symbol[rand() % (sym_size - 1)];
        Task task(x, y, op);
        cout << "task is " << task.get_task() << endl;

        thread_pool<Task>::get_instance(5)->push(task);
        sleep(1);
    }
    thread_pool<Task>::get_instance(5)->join();

    return 0;
}