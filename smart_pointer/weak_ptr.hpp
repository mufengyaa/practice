#include "shared_ptr.hpp"

// 由shared_ptr构造,没有其他功能
namespace my_weak_ptr
{
    template <class T>
    class weak_ptr
    {
    public:
        weak_ptr()
            : _ptr(nullptr) {}
        weak_ptr(const my_shared_ptr::shared_ptr<T> &p)
            : _ptr(p.get()) {}

        weak_ptr<T> &operator=(const my_shared_ptr::shared_ptr<T> &p)
        {
            _ptr = p.get();
            return *this;
        }

        T &operator*()
        {
            return *_ptr;
        }
        T *operator->()
        {
            return _ptr;
        }

        ~weak_ptr()
        {
            _ptr=nullptr;
        }

    private:
        T *_ptr;
    };
}