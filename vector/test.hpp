#include <iostream>
#include <assert.h>
#include <string.h>
#include <string>

using namespace std;

namespace bit
{
    template <class T>
    class myvector
    {
    public:
        // Vector的迭代器是一个原生指针
        typedef T *iterator;
        typedef const T *const_iterator;

        iterator begin()
        {
            return _start;
        }
        iterator end()
        {
            return _finish;
        }
        const_iterator begin() const
        {
            return _start;
        }
        const_iterator end() const
        {
            return _finish;
        }

        // construct and destroy

        myvector()
            : _start(nullptr), _finish(nullptr), _endOfStorage(nullptr){};
        myvector(int n, const T &value = T()) // 如果这里是size_t类型的n
                                              // 当传入参数都是整数时,会认为俩都是int,优先匹配两个参数是同样类型的模板函数
                                              // 但int无法解引用,就会报错
                                              // 所以这里的类型改为int,就可以匹配上了
            : _start(nullptr), _finish(nullptr), _endOfStorage(nullptr)
        {
            _start = new T[n];
            for (size_t i = 0; i < n; i++)
            {
                _start[i] = value;
            }
            _finish = _endOfStorage = _start + n;
        }

        template <class InputIterator> // 模板函数,为了支持其他类型的迭代器(string等)
        myvector(InputIterator first, InputIterator last)
        {
            int n = last - first;
            _finish = _start = new T[n];
            _endOfStorage = _start + n;
            for (auto begin = first; begin != last; ++begin)
            {
                push_back(*begin);
            }
        }

        myvector(const myvector<T> &v)
            : _start(nullptr), _finish(nullptr), _endOfStorage(nullptr)
        {
            reserve(v.capacity());

            // auto it = v.cbegin();
            // for (; it != v.cend(); ++it)
            // {
            //     push_back(*it);
            // }

            for(auto it: v){
                push_back(it);
            }
        }
        myvector<T> &operator=(myvector<T> v) // v传进来首先会进行拷贝构造,所以形参和实参空间不同
        {
            swap(v);
            return *this;
        }

        ~myvector()
        {
            delete[] _start;
            _start = _finish = _endOfStorage = nullptr;
        }

        // capacity

        size_t size() const
        {
            return _finish - _start;
        }
        size_t capacity() const
        {
            return _endOfStorage - _start;
        }
        void reserve(size_t n)
        {
            if (n > capacity())
            {
                int sz = size();
                T *tmp = new T[n];
                // memcpy(tmp,_start,sizeof(T)*sz);  //如果vector中的元素是自定义类型(string/vector),那这里就是浅拷贝(按字节拷贝)
                // 为了进行深拷贝,可以去赋值重载(内部是深拷贝)(拷贝构造不行,无法调用)
                for (size_t i = 0; i < sz; ++i)
                {
                    tmp[i] = _start[i];
                }
                delete[] _start;
                _start = tmp;
                _finish = _start + sz;
                _endOfStorage = _start + n;
            }
        }
        void resize(size_t n, const T &value = T()) // T() -- 匿名对象(调用默认构造)(对于自定义类型/对象)
        // 对于内置类型,c++在模板出来之后,对内置类型做了升级,也可以调用默认构造了
        {
            if (n > size())
            {
                reserve(n);
                for (iterator begin = _finish; begin != _endOfStorage; begin++)
                {
                    *begin = value;
                }
                _finish = _start + n;
            }
            else
            {
                _finish = _start + n;
            }
        }

        T &operator[](size_t pos)
        {
            assert(pos >= 0 && pos < size());
            return *(_start + pos);
        }
        const T &operator[](size_t pos) const
        {
            assert(pos >= 0 && pos < size());
            return *(_start + pos);
        }

        void push_back(const T &x)
        {
            if (size() == capacity())
            {
                reserve(size() + 1);
            }
            *(_finish++) = x;
        }
        void pop_back()
        {
            _start = erase(_start);
        }

        void swap(myvector<T> &v)
        {
            std::swap(_start, v._start);
            std::swap(_finish, v._finish);
            std::swap(_endOfStorage, v._endOfStorage);
        }
        iterator insert(iterator pos, const T &x)
        {
            assert(pos >= _start && pos <= _finish);
            int len = pos - _start;
            if (size() == capacity())
            {
                reserve(size() + 1);
            }
            pos = _start + len; // 如果扩容了,原pos指向的空间就已经被释放掉了,所以需要更新pos
            iterator end = _finish - 1;
            while (end >= pos)
            {
                *(end + 1) = *end;
                --end;
            }
            *pos = x;
            ++_finish;
            return pos; // 防止外部传入的pos失效,返回值可以让它更新
        }
        iterator erase(iterator pos)
        {
            assert(pos >= _start && pos < _finish);
            for (iterator begin = pos + 1; begin != _finish; ++begin)
            {
                *(begin - 1) = *begin;
            }
            _finish--;
            return pos; // vs中只要使用了原pos,就会报错
            // g++没有管,但尽量不要使用,虽然这里有返回值来进行迭代器更新,但难免会有疏漏(不同平台上的行为不同,所以不要再使用)
        }

    private:
        iterator _start;        // 指向数据块的开始
        iterator _finish;       // 指向有效数据的尾
        iterator _endOfStorage; // 指向存储容量的尾
    };

};