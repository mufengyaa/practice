#pragma once

#include <algorithm>
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
    void recycle(char *start, char *end, size_t size) // 拿回一串对象,挂在对应链表中
    {
        assert(start != nullptr);
        assert(end != nullptr);
        assert(end != start);
        int bucket_id = helper::size_to_bucket(size);
        while (start < end)
        {
            // 挂到对应span下,更新已分配的个数
            Span *span = Page_Cache::get_instance()->obj_to_span(start);
            span->free_list_.push_front(start);
            --span->use_count_;

            start += size;

            // 如果分配出去的都已收回,就将该span会回收到page cache
            if (span->use_count_ == 0)
            {
                Page_Cache::get_instance()->recycle(span);
                span->is_use_ = false; // 完成回收流程后,空闲
            }
        }
    }
    int alloc(void *&start, void *&end, int id, int size) // 从链表中拿出一串
    {
        // 判断链表中是否有非空span
        Span *span = span_map_[id].get_nonnull_span();
        if (span != nullptr) // span下有对象,能取多少取多少
        {
            return alloc_obj(start, end, size, span);
        }
        else // 向page cache取span,自己切对象,然后再取
        {
            alloc_span(id, size);
            return alloc(start, end, id, size);
        }
    }

private:
    Central_Cache() {}
    ~Central_Cache() {}
    Central_Cache(const Central_Cache &) = delete;
    Central_Cache &operator=(const Central_Cache &) = delete;

    void alloc_span(int id, size_t size)
    {
        int page = helper::size_to_page(size);
        // 向page cache申请span
        Span *span = Page_Cache::get_instance()->alloc(page);
        assert(span != nullptr);
        span->is_use_ = true; // 因为会分配给central cache,处于正在使用状态

        size_t space = span->n_ * (1 << helper::PAGE_SHIFT);               // 拥有的总空间大小
        char *start = (char *)(span->start_pageid_ << helper::PAGE_SHIFT); // 空间的起始地址
        char *end = start + space;
        // 先切下来一个,方便后续尾插
        span->free_list_.head() = start;
        void *tail = start;
        start += size;

        // 切分
        while (start < end) // 尾插
        {
            if (end - start < size) // 如果不能整切,就不要了
            {
                break;
            }
            span->free_list_.next(tail) = start;
            tail = start;
            start += size;
        }
        span->free_list_.next(tail) = nullptr;

        // 将切分好的span链入到双向链表中
        span_map_[id].insert(span);
    }
    int alloc_obj(void *&start, void *&end, int size, Span *span)
    {
        int num = std::min(span->free_list_.benchmark(), helper::feedback_regulation(size));
        if (num == span->free_list_.benchmark()) // 说明被基准值限制了,增加1,直至与理论值相等
        {
            ++span->free_list_.benchmark();
        }
        // 从非空span中拿出一段链表
        start = span->free_list_.pop(); // 已经至少拿出了一个结点
        end = start;
        assert(start != nullptr);
        int i = 0, real_num = 1;
        while (i < num - 1 && !span->free_list_.empty())
        {
            end = span->free_list_.pop();
            ++i;
            ++real_num;
        }
        span->free_list_.next(end) = nullptr;
        span->use_count_ += real_num;

        return real_num;
    }
};
Central_Cache Central_Cache::instance_;