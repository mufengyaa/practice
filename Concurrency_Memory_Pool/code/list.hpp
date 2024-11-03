#pragma once

#include <iostream>
#include <cassert>
#include <mutex>

class FreeList
{
public:
    void *free_list_ = nullptr;
    void *tail_ = nullptr;
    int benchmark_ = 1; // 对于每个链表,都有自己申请对象个数的基准值
    int size_ = 0;      // 链表长度

public:
    void push_front(void *ptr)
    {
        if (ptr == nullptr)
            return; // 检查有效性
        next(ptr) = free_list_;
        if (free_list_ == nullptr)
        {
            tail_ = ptr;
        }
        free_list_ = ptr;
    }
    void push_back(void *ptr)
    {
        if (ptr == nullptr)
            return; // 检查有效性
        if (tail_ != nullptr)
        {
            next(tail_) = ptr;
        }
        else // 如果链表为空
        {
            free_list_ = ptr; // 新节点成为头节点
        }
        tail_ = ptr; // 更新尾指针
    }

    void *pop() // 头删
    {
        assert(free_list_ != nullptr);
        if (free_list_ == nullptr)
        {
            int x = 1;
        }
        if (next(free_list_) == nullptr)
        {
            int x = 1;
        }
        void *ptr = free_list_;
        free_list_ = next(free_list_); // 指向下一个结点
        next(ptr) = nullptr;
        if (free_list_ == nullptr)
        {
            tail_ = nullptr;
        }
        return ptr;
    }
    void clear()
    {
        free_list_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }
    bool empty()
    {
        bool ret = nullptr == free_list_;
        return ret;
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
        if (r_node == head_)
        {
            assert(false);
            return nullptr;
        }
        head_->next_ = r_node->next_;
        r_node->next_->prev_ = head_;
        return r_node;
    }
    void remove(Span *pos) // 指定位置删除
    {
        if (pos == head_)
        {
            assert(false);
        }
        Span *next = pos->next_;
        Span *prev = pos->prev_;
        prev->next_ = next;
        next->prev_ = prev;

        pos->next_ = nullptr;
        pos->prev_ = nullptr;
    }
    Span *get_nonnull_span()
    {
        Span *head = head_, *node = head->next_;
        while (node != head)
        {
            if (node->free_list_.free_list_ != nullptr)
            {
                return node;
            }
            node = node->next_;
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
