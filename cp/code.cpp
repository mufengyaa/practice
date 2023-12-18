#include "BlockQueue.hpp"

int func_add(int x, int y)
{
    return x + y;
}
void *c_func(void *args)
{
    // sleep(1);
    // BlockQueue<int> *bq = (BlockQueue<int> *)args;
    // int data;
    // while (true)
    // {
    //     bq->pop(data);
    //     cout << "我消费了 : " << data << endl;
    //     // sleep(2);
    // }

    BlockQueue<Task> *bq = (BlockQueue<Task> *)args;
    while (true)
    {
        Task t;
        bq->pop(t);
        cout << pthread_self() << " 我消费了 : " << t.get_x() << " + " << t.get_y() << " = " << (t.get_x() + t.get_y()) << endl;
        // sleep(2);
    }
    return nullptr;
}
void *p_func(void *args)
{
    // BlockQueue<int> *bq = (BlockQueue<int> *)args;
    // int data = 1;
    // while (true)
    // {
    //     cout << "我生产了 : " << data << endl;
    //     bq->push(data++);
    //     sleep(2);
    // }

    BlockQueue<Task> *bq = (BlockQueue<Task> *)args;
    while (true)
    {
        int x = rand() % 10 + 1;
        int y = rand() % 10 + 1;

        // int x, y;
        // cout << "please enter x : ";
        // cin >> x;
        // cout << "please enter y : ";
        // cin >> y;

        Task t(x, y, func_add);
        cout << pthread_self() << " 我生产了 : " << x << " + " << y << " = ? " << endl;
        bq->push(t);
        sleep(1);
    }
    return nullptr;
}

void test1()
{
    srand(getpid() ^ (unsigned int)time(nullptr) ^ 0x1233412);
    pthread_t tid1, tid2;
    // BlockQueue<int> *bq = new BlockQueue<int>;
    BlockQueue<Task> *bq = new BlockQueue<Task>;
    pthread_create(&tid1, nullptr, c_func, bq);
    pthread_create(&tid2, nullptr, p_func, bq);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
}
void test2()
{
    srand(getpid() ^ (unsigned int)time(nullptr) ^ 0x1233412);
    pthread_t tid1[2], tid2[2];
    BlockQueue<Task> *bq = new BlockQueue<Task>;
    pthread_create(tid1, nullptr, c_func, bq);
    pthread_create(tid1 + 1, nullptr, c_func, bq);

    pthread_create(tid2, nullptr, p_func, bq);
    pthread_create(tid2 + 1, nullptr, p_func, bq);

    pthread_join(tid1[0], nullptr);
    pthread_join(tid1[1], nullptr);

    pthread_join(tid2[0], nullptr);
    pthread_join(tid2[1], nullptr);
}
int main()
{
    test2();
    return 0;
}