#include "head.h"

#pragma once

// 可以拷贝,但在循环引用的情况下,无法使用
// 外部开辟空间有多种方式:new/new[]/malloc,所以需要传入删除器
namespace my_shared_ptr
{
    template <class T>
    class shared_ptr
    {
    public:
        shared_ptr(T *p = nullptr) : _ptr(p)
        {
            _count = new int(1); // 每份资源对应一个计数
        }
        shared_ptr(const shared_ptr<T> &p, function<void(T *)> del) //不传默认是delete
            : _ptr(p._ptr), _count(p._count),_del(del)
        {
            ++(*_count);
        }
        shared_ptr(shared_ptr<T> &&p)
            : _ptr(p._ptr),_del(p._del)
        {
            --(p._count);
            p._ptr = nullptr;

            _count = new int(1);
        }
        shared_ptr<T> &operator=(shared_ptr<T> &p)
        {
            if (p._ptr == this->_ptr) // 防止自赋值(空间会提前释放)/引用同一片资源对其赋值(效率低)
            {
                return *this;
            }
            if (--this->_count == 0) // 如果this指向的空间已经没有人引用了,就手动释放
            {
                _del(_ptr);
                delete _count;
            }
            _ptr = p._ptr;
            _count = p._count;
            _del=p._del;
            ++(*_count);
            return *this;
        }
        T &operator*() const
        {
            return *_ptr;
        }
        T *operator->() const
        {
            return _ptr;
        }
        T *get() const
        {
            return _ptr;
        }

        ~shared_ptr()
        {
            if (--(*_count) == 0)
            {
                _del(_ptr);
            }
        }

    private:
        T *_ptr;
        int *_count; // 引用计数
        function<void(T *)> _del = [](T *p)
        { delete p; }; // 删除器
    };
}