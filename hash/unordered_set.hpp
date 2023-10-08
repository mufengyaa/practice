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
        typedef my_hash_bucket::HashBucket<K, K, KeyOfV> HT; // ʹ�ù�ϣͰ��װ����set
        typedef typename HT::const_iterator iterator;
        typedef typename HT::const_iterator const_iterator;

    public:
        unordered_set() : _ht()
        {
        }
        iterator begin() const
        {
            return _ht.begin(); //����const���ε���*this,��ô��ϣͰ������const��
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
            //����ͺ����һ��,��������ͨ������ת��Ϊconst������������
            //����,��֮ǰһ��,ҪΪ����������һ�����캯��
            auto it = _ht.Insert(value);
            return make_pair(it.first, it.second);
        }

        iterator erase(iterator position) //���յ��ǹ�ϣͰ��const������
        {
            auto it = _ht.Erase(position); //���ص�����ͨ������
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