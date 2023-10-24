#include "head.h"

//不允许拷贝
namespace my_unique_ptr
{
    template <class T>
    class unique_ptr
    {
    public:
        unique_ptr(T *p) : _ptr(p)
        {
        }
        unique_ptr(unique_ptr<T> &p) = delete;
        unique_ptr(unique_ptr<T> &&p) : _ptr(p._ptr)
        {
            p._ptr = nullptr;
        }
        unique_ptr<T> &operator=(unique_ptr<T> &p) = delete;
        T &operator*()
        {
            return *_ptr;
        }
        T *operator->()
        {
            return _ptr;
        }

        ~unique_ptr()
        {
            delete _ptr;
        }

    private:
        T *_ptr;
    };
}