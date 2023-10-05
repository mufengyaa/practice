#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 哈希桶
namespace my_hash_bucket
{
    // func用于拿到数据对应的整型值->然后可以得到对应的hashi
    template <class T>
    class HashFunc
    {
    public:
        size_t operator()(const T &val)
        {
            return val;
        }
    };
    template <>
    class HashFunc<string>
    {
    public:
        size_t operator()(const string &s)
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

    template <class V>
    struct HashBucketNode // 每个位置下链接的结点
    {
        HashBucketNode(const V &data)
            : _next(nullptr), _data(data)
        {
        }
        HashBucketNode<V> *_next;
        V _data;
    };

    // 这里的哈希桶中key是唯一的
    template <class V, class HF = HashFunc<V>>
    class HashBucket // 哈希桶
    {
        typedef HashBucketNode<V> Node;
        typedef Node *PNode;

        typedef HashBucket<V, HF> Self;

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

        // 哈希桶中的元素不能重复
        PNode Insert(const V &data)
        {
            if (Find(data))
            {
                return nullptr;
            }
            if (CheckCapacity()) // 需要扩容了
            {
                size_t newsize = _size * 2;
                HashBucket<V> newhsb(newsize);

                for (size_t i = 0; i < _size; ++i)
                {
                    PNode cur = _table[i];
                    while (cur) // 把桶上的结点挂在新位置
                    {
                        PNode next = cur->_next;
                        size_t hashi = newhsb.HashFunc(cur->_data);

                        cur->_next = newhsb._table[hashi];
                        newhsb._table[hashi] = cur;

                        cur = next;
                    }
                    _table[i] = nullptr;
                }
                Swap(newhsb);
            }
            PNode newnode = new Node(data);
            size_t hashi = HashFunc(data);
            // 头插
            newnode->_next = _table[hashi];
            _table[hashi] = newnode;
            ++_size;
            return newnode;
        }

        // 删除哈希桶中为data的元素(data不会重复)
        bool Erase(const V &data)
        {
            PNode del = Find(data);
            if (del == nullptr) // 如果没找着
            {
                return false;
            }

            size_t hashi = HashFunc(data);
            PNode prev = _table[hashi];
            if (prev == del) // 如果删除的是第一个结点
            {
                _table[hashi] = del->_next;
            }
            else
            {
                while (prev && prev->_next->_data != data) // 找到上一个结点
                {
                    prev = prev->_next;
                }
                prev->_next = del->_next;
            }
            delete del;
            del = nullptr;
            --_size;
        }

        PNode Find(const V &data)
        {
            size_t hashi = HashFunc(data);
            PNode cur = _table[hashi];
            while (cur)
            {
                if (cur->_data == data)
                {
                    return cur;
                }
                cur = cur->_next;
            }
            return nullptr;
        }

        size_t Size() const
        {
            return _size;
        }

        bool Empty() const
        {
            return 0 == _size;
        }

        void Print()
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

        void Swap(Self &ht)
        {
            _table.swap(ht._table);
            swap(_size, ht._size);
        }

    private:
        size_t HashFunc(const V &data) // 将数据转换成hashi
        {
            HF hf;
            return hf(data) % _table.size();
        }

        bool CheckCapacity()
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

// 哈希表
namespace my_hash_table
{
    // func用于拿到数据对应的整型值->然后可以得到对应的hashi
    template <class T>
    class HashFunc
    {
    public:
        size_t operator()(const T &val)
        {
            return val;
        }
    };
    template <>
    class HashFunc<string>
    {
    public:
        size_t operator()(const string &s)
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

    enum State
    {
        EMPTY,
        EXIST,
        DELETE
    };

    template <class K, class V, class HF = HashFunc<K>>
    class HashTable
    {
        struct Elem
        {
            pair<K, V> _val;
            State _state;
        };
        typedef HashTable<K, V> Self;

    public:
        HashTable(size_t capacity = 5)
            : _ht(capacity), _size(0), _totalSize(0)
        {
            for (size_t i = 0; i < capacity; ++i)
            {
                _ht[i]._state = EMPTY;
            }
        }

        // 插入
        bool Insert(const pair<K, V> &val)
        {
            if (Find(val.first) != -1)
            {
                return false;
            }
            if (CheckCapacity()) // 设置当因子超过0.7就进行扩容
            {
                size_t newsize = _ht.size() * 2;
                Self newht(newsize);
                for (size_t i = 0; i < _ht.size(); ++i)
                {
                    if (_ht[i]._state == EXIST)
                    {
                        newht.Insert(_ht[i]._val);
                    }
                }
                Swap(newht);
            }

            size_t hashi = HashFunc(val.first);
            while (_ht[hashi]._state == EXIST)
            {
                ++hashi;
                hashi %= _ht.size();
            }
            _ht[hashi]._val = val;
            _ht[hashi]._state = EXIST;
            ++_size;
            ++_totalSize;
            return true;
        }

        // 查找
        size_t Find(const K &key)
        {
            size_t hashi = HashFunc(key);
            for (size_t i = 0; i < _size; ++i)
            {
                if (_ht[i]._state == EXIST && _ht[i]._val.first == key)
                {
                    return i;
                }
            }
            return -1;
        }

        // 删除
        bool Erase(const K &key)
        {
            size_t i = Find(key);
            if (i == -1)
            {
                return false;
            }
            else
            {
                _ht[i]._state = DELETE;
                --_size;
                return true;
            }
        }

        size_t Size() const
        {
            return _size;
        }

        bool Empty() const
        {
            return _size == 0;
        }

        void Print()
        {
            for (size_t i = 0; i < _ht.size(); ++i)
            {
                if (_ht[i]._state != EXIST)
                {
                    continue;
                }
                else
                {
                    Elem cur = _ht[i];
                    printf("[%d]->", i);
                    cout << cur._val.first << ":" << cur._val.second << endl;
                }
            }
            cout << endl;
        }

        void Swap(Self &ht)
        {
            swap(_size, ht._size);
            swap(_totalSize, ht._totalSize);
            _ht.swap(ht._ht);
        }

    private:
        size_t HashFunc(const K &key)
        {
            HF hf;
            return hf(key) % _ht.size();
        }

        bool CheckCapacity()
        {
            if (_totalSize * 10 / _ht.size() >= 7)
            {
                return true;
            }
            return false;
        }

    private:
        vector<Elem> _ht;
        size_t _size;
        size_t _totalSize; // 哈希表中的所有元素：有效和已删除, 扩容时候要用到
    };
}