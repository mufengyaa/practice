#pragma once

#include "list.hpp"
#include "helper.hpp"

template <class T>
class FixedLength_MemoryPool
{
    FreeList free_list_;      // 管理被释放的结点
    size_t remain_space_ = 0; // 剩余空间大小
    char *space_ = nullptr;

public:
    FixedLength_MemoryPool() {}
    ~FixedLength_MemoryPool() {}
    T *New()
    {
        T *obj = nullptr;
        // 如果自由链表有结点,优先从这拿
        if (!free_list_.empty())
        {
            obj = static_cast<T *>(free_list_.pop());
        }
        else
        {
            // 如果空间不足,向堆申请空间
            if (remain_space_ < sizeof(T))
            {
                const int proportion = 2;
                space_ = static_cast<char *>(helper::allocate_memory(sizeof(T) / proportion));
                remain_space_ = (sizeof(T) / proportion) << helper::PAGE_SHIFT;
            }

            // 空间足够
            // 对齐到指针大小
            size_t object_size = sizeof(T) < helper::PTR_SIZE ? helper::PTR_SIZE : sizeof(T);
            // 开始切分
            obj = (T *)space_;
            space_ += object_size;
            remain_space_ -= object_size;
        }

        new (obj) T;
        return obj;
    }
    void Delete(T *obj)
    {
        assert(obj != nullptr);
        obj->~T();
        free_list_.push_front(obj);
    }
};
