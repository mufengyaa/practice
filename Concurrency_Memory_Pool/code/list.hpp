#pragma once

#include <iostream>
#include <assert.h>
#include <mutex>

class FreeList
{
    void *free_list_ = nullptr;

public:
    void push(void *ptr)
    {
        next(ptr) = free_list_;
        free_list_ = ptr;
    }
    void *pop()
    {
        assert(free_list_ != nullptr);
        void *ptr = free_list_;
        free_list_ = next(ptr); // 指向下一个结点
        return ptr;
    }
    inline bool empty() const
    {
        return free_list_ == nullptr;
    }

private:
    inline void *&next(void *ptr)
    {
        return *(void **)ptr; // 返回指向前4/8个字节的指针
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

    int use_count_ = 0;
};
class SpanList
{
    Span *head_ = nullptr; // 头结点
public:
    static std::mutex mtx_;
};
std::mutex SpanList::mtx_;
