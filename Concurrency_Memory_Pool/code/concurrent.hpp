#include "thread_cache.hpp"
#include <cassert>

void *concurrent_alloc(size_t size)
{
    // 通过TLS 每个线程无锁的获取自己的专属的ThreadCache对象
    if (TLS_ThreadCache == nullptr)
    {
        TLS_ThreadCache = new Thread_Cache;
    }
    return TLS_ThreadCache->alloc(size);
}

void concurrent_free(void *ptr, size_t size)
{
    assert(TLS_ThreadCache);
    TLS_ThreadCache->dealloc(ptr, size);
}