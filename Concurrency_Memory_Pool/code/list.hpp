#pragma once

#include <iostream>
#include <cassert>
#include <mutex>

class FreeList
{
    void *free_list_ = nullptr;
    void *tail_ = nullptr;
    int benchmark_ = 1; // 对于每个链表,都有自己申请对象个数的基准值
    int size_ = 0;      // 链表长度

public:
    void push_front(void *ptr)
    {
        // 头插
        next(ptr) = free_list_;
        if (free_list_ == nullptr)
        {
            tail_ = ptr;
        }
        free_list_ = ptr;
    }
    void push_back(void *ptr)
    {
        // 尾插
        if (tail_ != nullptr)
        {
            next(tail_) = ptr;
        }
        if (free_list_ == nullptr)
        {
            free_list_ = ptr;
        }
        tail_ = ptr;
    }
    void *pop()
    {
        assert(free_list_ != nullptr);
        void *ptr = free_list_;
        free_list_ = next(ptr); // 指向下一个结点
        return ptr;
    }
    void clear()
    {
        free_list_ = nullptr;
        tail_ = nullptr;
        benchmark_ = 1;
        size_ = 0;
    }
    inline bool empty() const
    {
        return free_list_ == nullptr;
    }
    inline int &size() // 针对分配给thread cache的内存块个数
    {
        return size_;
    }
    inline int &benchmark()
    {
        return benchmark_;
    }
    inline void *&next(void *ptr)
    {
        return *(void **)ptr; // 返回指向前4/8个字节的指针
    }
    inline void *&head()
    {
        return free_list_;
    }
};

struct Span
{
    int n_ = 0;
    size_t start_pageid_ = 0;
    FreeList free_list_;

    // 内部以双向循环链表结构组织起来
    Span *next_ = nullptr;
    Span *prev_ = nullptr;

    int use_count_ = 0; // 已分配出去的块数
    bool is_use_ = false;
};
class SpanList // 带头双向循环链表
{
    Span *head_ = nullptr; // 头结点
public:
    static std::mutex mtx_;
    static std::recursive_mutex rcs_mtx_;

public:
    SpanList()
    {
        head_ = new Span;
        head_->next_ = head_;
        head_->prev_ = head_;
    }
    void insert(Span *node)
    {
        // 头插
        Span *start = begin();
        head_->next_ = node;
        node->next_ = start;
        node->prev_ = head_;
        start->prev_ = node;
    }
    Span *pop_front() // 头删
    {
        Span *r_node = begin();
        head_->next_ = r_node->next_;
        r_node->next_->prev_ = head_;
        return r_node;
    }
    void remove(Span *pos) // 指定位置删除
    {
        Span *next = pos->next_;
        Span *prev = pos->prev_;
        prev->next_ = next;
        next->prev_ = prev;
    }
    Span *get_nonnull_span()
    {
        Span *head = head_;
        while (head != end())
        {
            head = head->next_;
            if (!(head->free_list_).empty())
            {
                return head;
            }
        }
        return nullptr;
    }
    bool empty()
    {
        return head_->next_ == head_;
    }

private:
    Span *end()
    {
        return head_->prev_;
    }
    Span *begin()
    {
        return head_->next_;
    }
};
std::mutex SpanList::mtx_;
std::recursive_mutex SpanList::rcs_mtx_;
