#define _CRT_SECURE_NO_WARNINGS

#include "hash.hpp"

namespace my_unordered_set
{
    template <class K>
    class unordered_set
    {
        struct KeyOfV
        {
            const K& operator()(const K& data)
            {
                return data;
            }
        };

    public:
        typedef my_hash_bucket::HashBucket<K, K, KeyOfV> HT; // 使用哈希桶封装无序set
        typedef typename HT::const_iterator iterator;
        typedef typename HT::const_iterator const_iterator;

    public:
        unordered_set() : _ht()
        {
        }
        iterator begin() const
        {
            return _ht.begin(); //这里const修饰的是*this,那么哈希桶对象是const的
        }
        iterator end() const
        {
            return _ht.end();
        }

        // capacity
        size_t size() const
        {
            return _ht.size();
        }
        bool empty() const
        {
            return _ht.empty();
        }

        // lookup
        iterator find(const K& key)
        {
            return _ht.Find(key);
        }
        size_t count(const K& key)
        {
            return _ht.Count(key);
        }

        // modify
        pair<iterator, bool> insert(const K& value)
        {
            //这里和红黑树一样,都存在普通迭代器转化为const迭代器的问题
            //所以,和之前一样,要为迭代器增加一个构造函数
            auto it = _ht.Insert(value);
            return make_pair(it.first, it.second);
        }

        iterator erase(iterator position) //接收的是哈希桶的const迭代器
        {
            auto it = _ht.Erase(position); //返回的是普通迭代器
            return it;
        }

        // bucket
        size_t bucket_count()
        {
            return _ht.BucketCount();
        }
        size_t bucket_size(const K& key)
        {
            return _ht.BucketSize(key);
        }

    private:
        HT _ht;
    };
}