#include <iostream>
#include <assert.h>
#include <string>

using namespace std;

namespace bit
{
    // List的节点类
    template <class T>
    struct ListNode // struct默认公有(因为不会有人去访问结点成员的)
    {
        typedef ListNode<T> *PNode;

        // ListNode(const T &val)
        //     : _ppre(nullptr), _pnext(nullptr), _val(val){};
        // ListNode(T &&val)  //注意,这里有两个重载函数时,不能都有缺省值,所以这里不设置缺省值了
        //     : _ppre(nullptr), _pnext(nullptr), _val(forward<T>(val)){};

        // 我们也可以用通用引用,来实现不同类型的参数,调用不同的构造
        template <class Data>
        ListNode(Data &&val)
            : _ppre(nullptr), _pnext(nullptr), _val(forward<Data>(val)){};

        template <class... Args>
        ListNode(Args... args)
            : _ppre(nullptr), _pnext(nullptr), _val(args...)
        {
        }

        PNode _ppre;
        PNode _pnext;
        T _val;
    };

    // List的迭代器类 -- 因为无法直接使用指针作为迭代器,需要手动添加功能
    template <class T, class Ref, class Ptr> // ref用于标记*时对象的const属性,ptr用于标记->时返回对象的const属性
    class ListIterator
    {
    public:
        typedef ListNode<T> *PNode;             // 指针重命名
        typedef ListIterator<T, Ref, Ptr> Self; // 迭代器重命名

        PNode _pNode; // 将指针作为迭代器的底层
    public:
        ListIterator(PNode pnode = nullptr)
            : _pNode(pnode){};
        ListIterator(const Self &l)
            : _pNode(l._pNode){};

        T &operator*()
        {
            return _pNode->_val;
        }
        T *operator->()
        {
            return &(_pNode->_val);
        }
        Self &operator++()
        {
            _pNode = _pNode->_pnext;
            return (*this);
        }
        Self operator++(int)
        {
            Self tmp(*this);
            _pNode = _pNode->_pnext;
            return tmp;
        }
        Self &operator--()
        {
            _pNode = _pNode->_ppre;
            return (*this);
        }
        Self operator--(int)
        {
            Self tmp(*this);
            _pNode = _pNode->_ppre;
            return tmp;
        }
        bool operator!=(const Self &l) const
        {
            return _pNode != l._pNode;
        }
        bool operator==(const Self &l) const
        {
            return _pNode == l._pNode;
        }
    };

    // list类
    template <class T>
    class mylist
    {
        typedef ListNode<T> Node;
        typedef Node *PNode;

    public: // 两种迭代器
        typedef ListIterator<T, T &, T *> iterator;
        typedef ListIterator<T, const T &, const T &> const_iterator;

#include "reverse_iteritor.hpp" //反向迭代器
        typedef myreverse_iterator<iterator, T &, T *> reverse_iterator;
        typedef myreverse_iterator<const_iterator, const T &, const T *> const_reverse_iterator;

    public:
        // List的构造
        mylist()
        {
            CreateHead();
        }
        mylist(int n, const T &value = T())
        {
            CreateHead();
            PNode p = _pHead;
            for (size_t i = 0; i < n; ++i)
            {
                push_back(value);
            }
            _size += n;
        }

        template <class Iterator>
        mylist(Iterator first, Iterator last)
        {
            CreateHead();
            while (first != last)
            {
                push_back(*first);
                ++first;
            }
        }

        mylist(const mylist<T> &l)
        {
            CreateHead();
            for (auto c : l)
            {
                push_back(c);
            }
        }
        mylist<T> &operator=(const mylist<T> l)
        {
            swap(l);
            return (*this);
        }
        ~mylist()
        {
            // clear();
            // delete[] _pHead;
            while (!empty())
            {
                erase(begin());
            }
        }

        // List Iterator
        iterator begin()
        {
            return iterator(_pHead->_pnext);
        }
        iterator end()
        {
            return iterator(_pHead);
        }
        const_iterator begin() const
        {
            return const_iterator(_pHead->_pnext);
        }
        const_iterator end() const
        {
            return const_iterator(_pHead);
        }

        // List reverse_Iterator
        reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }
        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        // List Capacity
        size_t size() const
        {
            return _size;
        }
        bool empty() const
        {
            return _size == 0;
        }

        // List Access
        T &front()
        {
            return *begin();
        }
        const T &front() const
        {
            return *begin();
        }
        T &back()
        {
            return *(--end());
        }
        const T &back() const
        {
            return *(--end());
        }

        // List Modify
        void push_back(const T &val)
        {
            insert(end(), val);
        }
        void push_back(T &&val) // 移动构造,如果这里传入一个右值
        {
            insert(end(), std::forward<T>(val)); // 需要我们保持它的右值属性
        }

        void pop_back()
        {
            erase(--end());
        }

        void push_front(const T &val)
        {
            insert(begin(), val);
        }
        void push_front(T &&val) // 移动构造,如果这里传入一个右值
        {
            insert(begin(), std::forward<T>(val)); // 需要我们保持它的右值属性
        }

        void pop_front()
        {
            erase(begin());
        }

        template <class... Args>  
        void emplace_back (Args&&... args){
            insert(end(),args...);
        }

        // 在pos位置前插入值为val的节点
        iterator insert(iterator pos, const T &val)
        {
            PNode cur = pos._pNode;
            PNode pre = cur->_ppre;
            PNode newnode = new Node(val);

            newnode->_pnext = cur;
            pre->_pnext = newnode;

            cur->_ppre = newnode;
            newnode->_ppre = pre;

            _size++;

            return newnode;
        }
        template <class... Args>  
        iterator insert(iterator pos, Args&&... args)
        {
            PNode cur = pos._pNode;
            PNode pre = cur->_ppre;
            PNode newnode = new Node(args...);

            newnode->_pnext = cur;
            pre->_pnext = newnode;

            cur->_ppre = newnode;
            newnode->_ppre = pre;

            _size++;

            return newnode;
        }
        iterator insert(iterator pos, T &&val) // 继承push函数给的右值
        {
            PNode cur = pos._pNode;
            PNode pre = cur->_ppre;
            PNode newnode = new Node(std::forward<T>(val)); // 要保持它的右值属性

            newnode->_pnext = cur;
            pre->_pnext = newnode;

            cur->_ppre = newnode;
            newnode->_ppre = pre;

            _size++;

            return newnode;
        }

        // 删除pos位置的节点，返回该节点的下一个位置
        iterator erase(iterator pos)
        {
            PNode cur = pos._pNode;
            PNode pre = cur->_ppre, next = cur->_pnext;
            pre->_pnext = cur->_pnext;
            cur->_pnext->_ppre = pre;
            delete cur;
            --_size;
            return next;
        }
        void clear()
        {
            PNode cur = _pHead->_pnext;
            while (cur != _pHead)
            {
                PNode tmp = cur;
                cur = cur->_pnext;
                delete tmp;
            }
            _size = 0;
            // iterator it = begin();
            // while (it != end())
            // {
            //     it = erase(it);
            // }
        }
        void swap(mylist<T> &l)
        {
            std::swap(_pHead, l._pHead);
            std::swap(_size, l._size);
        }

    private:
        void CreateHead()
        {
            _pHead = new Node(T()); // 因为去掉了缺省值,所以这里给个默认构造
            _pHead->_pnext = _pHead;
            _pHead->_ppre = _pHead;
            _size = 0;
        }
        PNode _pHead;
        size_t _size;
    };
};