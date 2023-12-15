#include "BlockQueue.hpp"

void *c_func(void *args)
{
    //sleep(1);
    BlockQueue<int> *bq = (BlockQueue<int> *)args;
    int data;
    while (true)
    {
        bq->pop(data);
        cout << "我消费了 : " << data << endl;
        //sleep(2);
    }
    return nullptr;
}
void *p_func(void *args)
{
    BlockQueue<int> *bq = (BlockQueue<int> *)args;
    int data = 1;
    while (true)
    {
        cout << "我生产了 : " << data << endl;
        bq->push(data++);
        sleep(2);
    }
    return nullptr;
}
void test1()
{
    pthread_t tid1, tid2;
    BlockQueue<int> *bq = new BlockQueue<int>;
    pthread_create(&tid1, nullptr, c_func, bq);
    pthread_create(&tid2, nullptr, p_func, bq);

    pthread_join(tid1, nullptr);
    pthread_join(tid2, nullptr);
}
int main()
{
    test1();
    return 0;
}