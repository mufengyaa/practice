#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
using namespace std;

// 哈希桶
namespace my_hash_bucket
{
    // hsfunc
    // 用于拿到数据对应的整型值->然后可以得到对应的hashi
    template <class T>
    class HSFunc
    {
    public:
        size_t operator()(const T& val)
        {
            return val;
        }
    };
    template <>
    class HSFunc<string>
    {
    public:
        size_t operator()(const string& s)
        {
            int size = s.size();
            unsigned int seed = 131; // 31 131 1313 13131 131313 都可以
            unsigned int hashi = 0;
            for (size_t i = 0; i < size; ++i)
            {
                hashi = hashi * seed + s[i];
            }
            return hashi;
        }
    };

    // 结点
    template <class T>
    struct HashBucketNode // 每个位置下链接的结点
    {
        HashBucketNode(const T& data)
            : _next(nullptr), _data(data)
        {
        }
        HashBucketNode<T>* _next;
        T _data;
    };

    // 迭代器
    template <class K, class T, class KeyOfValue, class HF>
    class HashBucket; // 前置声明,因为迭代器中要用到哈希桶,而哈希桶也要用到迭代器
                      // 但迭代器肯定是先的那个,所以给迭代器一个前置声明,让他可以先使用哈希桶(声明不需要给默认值)

    template <class Key, class Data, class Ptr, class Ref, class KeyOfData, class HFc>
    struct HBIterator
    {
        typedef HashBucket<Key, Data, KeyOfData, HFc> HB;
        typedef HashBucketNode<Data>* PNode;
        typedef HBIterator<Key, Data, Ptr, Ref, KeyOfData, HFc> Self; // 自己,用于返回

        typedef HBIterator<Key, Data, Data*, Data&, KeyOfData, HFc> iterator; //用于解决set中插入的问题

        KeyOfData kot;

        HBIterator(PNode pNode = nullptr, const HB* pHt = nullptr)  //因为会出现传入的指针是底层const类型的情况,所以这里也改
            : _pnode(pNode), _pHt(pHt) // 需要结点指针+哈希桶对象的指针
        {
        }
        HBIterator(const iterator& it)
            : _pnode(it._pnode), _pHt(it._pHt) // 需要结点指针+哈希桶对象的指针
            //虽然增加了构造,但会导致,用普通对象构造
        {
        }
        Self& operator++()
        {
            // 当前迭代器所指节点后还有节点时直接取其下一个节点
            if (_pnode->_next)
            {
                _pnode = _pnode->_next;
            }

            else
            {
                // 找下一个不空的桶，返回该桶中第一个节点

                size_t hashi = _pHt->HashFunc(kot(_pnode->_data)) + 1;
                _pnode = nullptr; // 这里提前赋值,以防;没有下一个桶后,可以返回指向空的迭代器
                for (; hashi < _pHt->BucketCount(); ++hashi)
                {
                    if (_pnode = _pHt->_table[hashi]) // 注意,这里用到了哈希桶的私密成员,所以需要让迭代器成为哈希桶的友元
                    {                                 // 好妙
                        break;
                    }
                    // if (_pHt->_ht[hashi])
                    // {
                    //     _pnode = _pHt->_ht[hashi];
                    //     break;
                    // }
                }
            }

            return *this;
        }
        Self operator++(int)
        {
            Self tmp(*this);
            ++(*this);
            return tmp;
        }
        Ref operator*()
        {
            return _pnode->_data;
        }
        Ptr operator->()
        {
            return &_pnode->_data;
        }
        bool operator==(const Self& it) const
        {
            return _pnode == it._pnode;
        }
        bool operator!=(const Self& it) const
        {
            return _pnode != it._pnode;
        }

        PNode _pnode; // 当前迭代器关联的节点(也就是迭代器的本质)
        const HB* _pHt;     // 哈希桶--为了找下一个位置
        //因为,会出现传入的指针是底层const类型的情况,所以这里可以直接定义为底层const类型的指针,因为迭代器内部不修改哈希桶
    };

    // 哈希桶
    //  这里的key是唯一的
    template <class K, class T, class KeyOfValue, class HF = HSFunc<K>>
    // KeyOfValue用于不同类型的元素,返回key
    // HF用于不同类型的key,返回整型
    class HashBucket
    {
        template <class Key, class Data, class Ptr, class Ref, class KeyOfData, class HFc>
        friend struct HBIterator;

    public:
        typedef HashBucketNode<T> Node;
        typedef Node* PNode;

        typedef HashBucket<K, T, KeyOfValue, HF> Self;

        typedef HBIterator<K, T, T*, T&, KeyOfValue, HF> iterator;
        typedef HBIterator<K, T, const T*, const T&, KeyOfValue, HF> const_iterator;

    public:
        HashBucket(size_t capacity = 5)
            : _size(0)
        {
            _table.resize(capacity, nullptr);
        }

        ~HashBucket()
        {
            Clear();
        }

        iterator begin()
        {
            for (size_t i = 0; i < _table.size(); ++i) // 遍历数组,找到第一个桶
            {
                if (_table[i])
                {
                    return iterator(_table[i], this);
                }
            }
            return iterator(nullptr, this); // 很妙,this指针就是所需要的哈希桶对象的指针
        }
        iterator end()
        {
            return iterator(nullptr, this);
        }
        const_iterator begin() const
        {
            for (size_t i = 0; i < _table.size(); ++i) // 遍历数组,找到第一个桶
            {
                if (_table[i])
                {
                    return iterator(_table[i], this);
                }
            }
            return const_iterator(nullptr, this); // 很妙,this指针就是所需要的哈希桶对象的指针
        }
        const_iterator end() const
        {
            return const_iterator(nullptr, this); //这里的const,修饰的是*this,所以哈希桶对象是const的
                    //但是,迭代器的构造,是直接将传入的迭代器指针初始化,所以就会出现"const指针初始化普通指针"的问题
        }

        // 哈希桶中的元素不能重复
        pair<iterator, bool> Insert(const T& data)
        {
            KeyOfValue kot;
            auto it = Find(kot(data));
            if (it != end())
            {
                return make_pair(it, false);
            }
            if (CheckCapacity()) // 需要扩容了
            {
                size_t newsize = _size * 2;
                Self newhsb(newsize);

                for (size_t i = 0; i < _size; ++i)
                {
                    PNode cur = _table[i];
                    while (cur) // 把桶上的结点挂在新位置
                    {
                        PNode next = cur->_next;
                        size_t hashi = newhsb.HashFunc(kot(cur->_data));

                        cur->_next = newhsb._table[hashi];
                        newhsb._table[hashi] = cur;

                        cur = next;
                        ++newhsb._size;
                    }
                    _table[i] = nullptr;
                }
                Swap(newhsb);
            }
            PNode newnode = new Node(data);
            size_t hashi = HashFunc(kot(data));
            // 头插
            newnode->_next = _table[hashi];
            _table[hashi] = newnode;
            ++_size;
            return make_pair(newnode, true);
        }

        // 删除哈希桶中为data的元素(data不会重复)
        iterator Erase(const_iterator del)
        {
            KeyOfValue kot;
            if (del == end()) {
                return end();
            }
            if (del != Find(kot(*del)))  // 说明这个迭代器不正确
            {
                return end();
            }

            auto cur = const_iterator(del);
            ++cur;
            size_t hashi = HashFunc(kot(*del));
            PNode prev = _table[hashi];
            if (prev == del._pnode) // 如果删除的是第一个结点
            {
                _table[hashi] = del._pnode->_next;
            }
            else
            {
                while (prev && prev->_next->_data != *del) // 找到上一个结点
                {
                    prev = prev->_next;
                }
                prev->_next = del._pnode->_next;
            }
            --_size;
            auto it = const_iterator(begin());
            int count = 0;
            while (it != cur) {
                ++it;
                ++count;
            }
            auto a = iterator(begin());
            for (int i = 0; i < count; ++i) {
                ++a;
            }
            return a;
        }

        iterator Find(const K& data) const
        {
            KeyOfValue kot;
            size_t hashi = HashFunc(data);
            PNode cur = _table[hashi];
            while (cur)
            {
                if (kot(cur->_data) == data)
                {
                    return iterator(cur);
                }
                cur = cur->_next;
            }
            return iterator(nullptr);
        }

        size_t Size() const
        {
            return _size;
        }

        bool Empty() const
        {
            return 0 == _size;
        }

        void Print() const
        {
            for (size_t i = 0; i < _table.size(); ++i)
            {
                PNode cur = _table[i];
                printf("[%d]:", i);
                while (cur)
                {
                    PNode next = cur->_next;
                    cout << cur->_data << " ";
                    cur = next;
                }
                cout << endl;
            }
            cout << endl;
        }

        void Clear()
        {
            for (size_t i = 0; i < _table.size(); ++i)
            {
                PNode cur = _table[i];
                while (cur)
                {
                    PNode next = cur->_next;
                    delete cur;
                    cur = next;
                }
                _table[i] = nullptr;
            }
        }

        size_t BucketCount() const
        {
            return _table.size();
        }

        void Swap(Self& ht)
        {
            _table.swap(ht._table);
            swap(_size, ht._size);
        }

    private:
        size_t HashFunc(const K& data) const// 将数据转换成hashi
        {
            HF hf;
            return hf(data) % _table.size();
        }

        bool CheckCapacity() const
        {
            if (_size == _table.size())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

    private:
        vector<PNode> _table;
        size_t _size; // 哈希表中有效元素的个数
    };
}

