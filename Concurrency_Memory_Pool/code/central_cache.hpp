#pragma once

#include <algorithm>
#include <iostream>
#include <thread>
#include "helper.hpp"
#include "list.hpp"
#include "page_cache.hpp"

class Central_Cache
{
    static Central_Cache instance_; // 懒汉模式(不用加锁)
    SpanList span_map_[helper::BUCKET_SIZE];

public:
    static Central_Cache *get_instance()
    {
        return &instance_;
    }

    void recycle(FreeList &list, size_t size) // 回收对象，挂到对应链表中
    {
        int bucket_id = helper::size_to_bucket(size);
        std::unique_lock<std::mutex> lock(span_map_[bucket_id].mtx_);

        while (!list.empty())
        {
            void *ptr = list.pop();
            Span *span = Page_Cache::get_instance()->obj_to_span(ptr);
            assert(span != nullptr);
            span->free_list_.push_front(ptr);
            --span->use_count_;

            if (span->use_count_ == 0)
            {
                // 删除该span和其他结构的链接关系
                span_map_[bucket_id].remove(span);
                span->free_list_.clear();

                lock.unlock();

                // 回收给page cache
                Page_Cache::get_instance()->mtx_.lock();
                Page_Cache::get_instance()->recycle(span);
                span->is_use_ = false;
                Page_Cache::get_instance()->mtx_.unlock();

                lock.lock();
            }
        }

        list.clear();
    }

    int alloc(void *&start, void *&end, int id, int size, int &benchmark) // 分配对象
    {
        Span *span = get_nonnull_span(id, size);

        // 要操作某条span链表了
        std::unique_lock<std::mutex> lock(span_map_[id].mtx_);
        int num = alloc_obj(start, end, size, span, benchmark);
        return num;
    }

private:
    Central_Cache() {}
    ~Central_Cache() {}
    Central_Cache(const Central_Cache &) = delete;
    Central_Cache &operator=(const Central_Cache &) = delete;
    Span *get_nonnull_span(int id, size_t size)
    {
        span_map_[id].mtx_.lock();
        Span *span = span_map_[id].get_nonnull_span();
        span_map_[id].mtx_.unlock();
        if (span != nullptr)
        {
            return span;
        }
        else // 没有span就去page cache里要
        {
            span = alloc_page_cache(id, size);
            // 切分成若干个对象
            size_t space = span->n_ << helper::PAGE_SHIFT;
            char *start = (char *)(span->start_pageid_ << helper::PAGE_SHIFT);
            char *end = start + space;

            while (start < end)
            {
                // 如果不能整切,就不要了
                if (end - start < size)
                {
                    break;
                }
                span->free_list_.push_back(start);
                start += size;
            }
            span->free_list_.next(start - size) = nullptr;

            // 插入到central cache结构中
            span_map_[id].mtx_.lock();
            span_map_[id].insert(span);
            span_map_[id].mtx_.unlock();

            return span;
        }
    }

    Span *alloc_page_cache(int id, size_t size)
    {
        int page = helper::size_to_page(size);

        // 向page cache获取span
        Page_Cache::get_instance()->mtx_.lock();
        Span *span = Page_Cache::get_instance()->alloc(page);
        assert(span != nullptr);
        span->is_use_ = true;
        Page_Cache::get_instance()->mtx_.unlock();
        return span;
    }

    int alloc_obj(void *&start, void *&end, int size, Span *span, int &benchmark) // 从某个span下获取一定数量的对象
    {
        // 确定取出的对象个数
        int theoretical_value = helper::feedback_regulation(size);
        int num = benchmark < theoretical_value ? benchmark : theoretical_value;
        if (num == benchmark)
        {
            ++benchmark;
        }

        // 首先获取一个
        start = span->free_list_.pop();
        end = start;
        int i = 0, real_num = 1;
        // 获取后续结点
        while (i < num - 1 && !span->free_list_.empty())
        {
            end = span->free_list_.pop();
            ++i;
            ++real_num;
        }
        span->free_list_.next(end) = nullptr;
        span->use_count_ += real_num; // 分配给thread cache的对象个数

        return real_num;
    }
};

Central_Cache Central_Cache::instance_;