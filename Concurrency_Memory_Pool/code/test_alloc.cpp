#include "thread_cache.hpp"

void TestConcurrentAlloc1()
{
    Thread_Cache tc;
    void *p1 = tc.alloc(6);
    void *p2 = tc.alloc(8); // 剩1
    void *p3 = tc.alloc(1); // 拿1
    void *p4 = tc.alloc(7); // 剩2
    void *p5 = tc.alloc(8); // 拿1
    void *p6 = tc.alloc(8); // 拿1
    void *p7 = tc.alloc(8); // 剩3

    std::cout << p1 << std::endl;
    std::cout << p2 << std::endl;
    std::cout << p3 << std::endl;
    std::cout << p4 << std::endl;
    std::cout << p5 << std::endl;
    std::cout << p6 << std::endl;
    std::cout << p7 << std::endl;

    tc.dealloc(p1, 6);
    tc.dealloc(p2, 8);
    tc.dealloc(p3, 1);
    tc.dealloc(p4, 7);
    tc.dealloc(p5, 8);
    tc.dealloc(p6, 8);
    tc.dealloc(p7, 8);
}

int main()
{
    TestConcurrentAlloc1();
}
