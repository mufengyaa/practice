#include "hash.hpp"

namespace my_unordered_set
{
    template <class K, class V, class KeyOfValue, class HF>
    class HashBucket;

    template <class K, class V, class KeyOfValue, class HF>
    struct HBIterator
    {
        typedef HashBucket<K, V, KeyOfValue, HF> HashBucket;
        typedef HashBucketNode<V> *PNode;
        typedef HBIterator<K, V, KeyOfValue, HF> Self;

        HBIterator(PNode pNode = nullptr, HashBucket *pHt = nullptr);
        Self &operator++()
        {
            // 当前迭代器所指节点后还有节点时直接取其下一个节点
            if (_pNode->_pNext)
                _pNode = _pNode->_pNext;
            else
            {
                // 找下一个不空的桶，返回该桶中第一个节点
                size_t bucketNo = _pHt->HashFunc(KeyOfValue()(_pNode->_data)) + 1;
                for (; bucketNo < _pHt->BucketCount(); ++bucketNo)
                {
                    if (_pNode = _pHt->_ht[bucketNo])
                        break;
                }
            }

            return *this;
        }
        Self operator++(int);
        V &operator*();
        V *operator->();
        bool operator==(const Self &it) const;
        bool operator!=(const Self &it) const;
        PNode _pNode;     // 当前迭代器关联的节点
        HashBucket *_pHt; // 哈希桶--主要是为了找下一个空桶时候方便
    };

    template <class K, class HF = my_hash_bucket::HashFunc<K>>
    class unordered_set
    {
        typedef HashBucket<K, K, KeyOfValue, HF> HT;
        // 通过key获取value的操作
        struct KeyOfValue
        {
            const K &operator()(const K &data)
            {
                return data;
            }
        };

    public:
        typename typedef HT::Iterator iterator;

    public:
        unordered_set() : _ht()
        {
        }
        iterator begin() { return _ht.begin(); }
        iterator end() { return _ht.end(); }

        // capacity
        size_t size() const { return _ht.size(); }
        bool empty() const { return _ht.empty(); }

        // lookup
        iterator find(const K &key) { return _ht.Find(key); }
        size_t count(const K &key) { return _ht.Count(key); }

        // modify
        pair<iterator, bool> insert(const K &valye)
        {
            return _ht.Insert(valye);
        }

        iterator erase(iterator position)
        {
            return _ht.Erase(position);
        }

        // bucket
        size_t bucket_count() { return _ht.BucketCount(); }
        size_t bucket_size(const K &key) { return _ht.BucketSize(key); }

    private:
        HT _ht;
    };
}