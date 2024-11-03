#include "helper.hpp"
#include "thread_cache.hpp"
#include <cassert>

void *concurrent_alloc(size_t size)
{
    if (size <= helper::THREAD_CACHE_MAX_SIZE)
    {
        // 通过TLS 每个线程无锁的获取自己的专属的ThreadCache对象
        if (TLS_ThreadCache == nullptr)
        {
            TLS_ThreadCache = new Thread_Cache;
        }
        return TLS_ThreadCache->alloc(size);
    }
    else // 直接走page cache流程
    {
        int page_size = 1 << helper::PAGE_SHIFT;
        // int page = (size % page_size > 0) ? (size / page_size + 1) * page_size : size >> helper::PAGE_SHIFT;
        int page = (size + page_size - 1) >> helper::PAGE_SHIFT;

        std::unique_lock<std::mutex> lock(Page_Cache::get_instance()->mtx_);
        Span *span = Page_Cache::get_instance()->alloc(page);

        return helper::pageid_to_ptr(span->start_pageid_);
    }
}

void concurrent_free(void *ptr, size_t size)
{
    Span *span = Page_Cache::get_instance()->obj_to_span(ptr);
    if (size <= helper::THREAD_CACHE_MAX_SIZE)
    {
        assert(TLS_ThreadCache);
        TLS_ThreadCache->dealloc(ptr, size);
    }
    else
    {
        std::unique_lock<std::mutex> lock(Page_Cache::get_instance()->mtx_);
        Page_Cache::get_instance()->recycle(span);
    }
}