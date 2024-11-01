#include <vector>
#include <thread>
#include <atomic>
#include "concurrent.hpp"

void TestConcurrentAlloc1()
{
    Thread_Cache tc;
    void *p1 = tc.alloc(6);
    void *p2 = tc.alloc(8); // 剩1
    void *p3 = tc.alloc(1); // 拿1
    void *p4 = tc.alloc(7); // 剩2
    void *p5 = tc.alloc(8); // 拿1
    void *p6 = tc.alloc(8); // 拿1
    // void *p7 = tc.alloc(8); // 剩3

    std::cout << p1 << std::endl;
    std::cout << p2 << std::endl;
    std::cout << p3 << std::endl;
    std::cout << p4 << std::endl;
    std::cout << p5 << std::endl;
    std::cout << p6 << std::endl;
    // std::cout << p7 << std::endl;

    tc.dealloc(p1, 6);
    tc.dealloc(p2, 8);
    tc.dealloc(p3, 1);
    tc.dealloc(p4, 7);
    tc.dealloc(p5, 8);
    tc.dealloc(p6, 8);
    // tc.dealloc(p7, 8);
}

void MultiThreadAlloc1()
{
    std::vector<void *> v;
    for (size_t i = 0; i < 70; ++i)
    {
        void *ptr = concurrent_alloc(6);
        v.push_back(ptr);
    }

    for (auto e : v)
    {
        concurrent_free(e, 6);
    }
}

void MultiThreadAlloc2()
{
    std::vector<void *> v;
    for (size_t i = 0; i < 70; ++i)
    {
        void *ptr = concurrent_alloc(6);
        v.push_back(ptr);
    }

    for (auto e : v)
    {
        concurrent_free(e, 6);
    }
}

void TestMultiThread()
{
    std::thread t1(MultiThreadAlloc1);
    std::thread t2(MultiThreadAlloc2);

    t1.join();
    t2.join();
}

void BenchmarkMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
    std::vector<std::thread> vthread(nworks);
    std::atomic<size_t> malloc_costtime;
    std::atomic<size_t> free_costtime;
    malloc_costtime.store(0);
    free_costtime.store(0);

    for (size_t k = 0; k < nworks; ++k)
    {
        vthread[k] = std::thread([&, k]()
                                 {
			std::vector<void*> v;
			v.reserve(ntimes);

			for (size_t j = 0; j < rounds; ++j)
			{
				size_t begin1 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					v.push_back(malloc(16));
					//v.push_back(malloc((16 + i) % 8192 + 1));
				}
				size_t end1 = clock();

				size_t begin2 = clock();
				for (size_t i = 0; i < ntimes; i++)
				{
					free(v[i]);
				}
				size_t end2 = clock();
				v.clear();

				malloc_costtime += (end1 - begin1);
				free_costtime += (end2 - begin2);
			} });
    }

    for (auto &t : vthread)
    {
        t.join();
    }
    size_t malloc_time = malloc_costtime.load();
    size_t free_time = free_costtime.load();
    printf("%u个线程并发执行%u轮次,每轮次malloc %u次: 花费：%u ms\n",
           nworks, rounds, ntimes, malloc_time);

    printf("%u个线程并发执行%u轮次,每轮次free %u次: 花费：%u ms\n",
           nworks, rounds, ntimes, free_time);

    printf("%u个线程并发malloc&free %u次,总计花费：%u ms\n",
           nworks, nworks * rounds * ntimes, malloc_time + free_time);
}

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <ctime>

// 假设有 concurrent_alloc 和 concurrent_free 函数定义

// 单轮次申请释放次数 线程数 轮次
void BenchmarkConcurrentMalloc(size_t ntimes, size_t nworks, size_t rounds)
{
    std::vector<std::thread> vthread(nworks);
    std::atomic<size_t> malloc_costtime;
    std::atomic<size_t> free_costtime;
    malloc_costtime.store(0);
    free_costtime.store(0);

    for (size_t k = 0; k < nworks; ++k)
    {
        vthread[k] = std::thread([&]()
                                 {
            std::vector<void*> v;
            v.reserve(ntimes);

            for (size_t j = 0; j < rounds; ++j)
            {
                size_t begin1 = clock();
                for (size_t i = 0; i < ntimes; i++)
                {
                    v.push_back(concurrent_alloc(16));
                }
                size_t end1 = clock();

                size_t begin2 = clock();
                for (size_t i = 0; i < ntimes; i++)
                {
                    concurrent_free(v[i], 16);
                }
                size_t end2 = clock();
                v.clear();

                malloc_costtime += (end1 - begin1);
                free_costtime += (end2 - begin2);
            } });
    }

    for (auto &t : vthread)
    {
        t.join();
    }
    size_t malloc_time = malloc_costtime.load();
    size_t free_time = free_costtime.load();
    // 使用 load() 获取 atomic 变量的值
    printf("%u个线程并发执行%u轮次,每轮次concurrent alloc %u次: 花费：%lu ms\n",
           static_cast<unsigned int>(nworks), static_cast<unsigned int>(rounds), static_cast<unsigned int>(ntimes), malloc_time);

    printf("%u个线程并发执行%u轮次,每轮次concurrent dealloc %u次: 花费：%lu ms\n",
           static_cast<unsigned int>(nworks), static_cast<unsigned int>(rounds), static_cast<unsigned int>(ntimes), free_time);

    printf("%u个线程并发concurrent alloc&dealloc %u次,总计花费：%lu ms\n",
           static_cast<unsigned int>(nworks), static_cast<unsigned int>(nworks * rounds * ntimes), malloc_time + free_time);
}

void test_time()
{
    size_t n = 1000;
    std::cout << "==========================================================" << std::endl;
    BenchmarkConcurrentMalloc(n, 4, 10);
    //std::cout << std::endl << std::endl;

    BenchmarkMalloc(n, 4, 10);
    std::cout << "==========================================================" << std::endl;
}

int main()
{
    test_time();
    return 0;
}
