#include "head.h"

//拷贝时,将被拷贝对象置空
namespace my_auto_ptr
{
    template <class T>
    class auto_ptr
    {
    public:
        auto_ptr(T *p) : _ptr(p)
        {
        }
        auto_ptr(auto_ptr<T> &p) : _ptr(p._ptr)
        {
            p._ptr = nullptr;
        }
        auto_ptr(auto_ptr<T> &&p) : _ptr(p._ptr)
        {
            p._ptr = nullptr;
        }
        auto_ptr<T> &operator=(auto_ptr<T> &p)
        {
            _ptr = p._ptr;
            p._ptr = nullptr;
            return *this;
        }
        T &operator*(){
            return *_ptr;
        }
        T *operator->(){
            return _ptr;
        }

        ~auto_ptr()
        {
            delete _ptr;
        }

    private:
        T *_ptr;
    };
}