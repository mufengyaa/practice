<<<<<<< HEAD
#include "hash.hpp"

namespace my_unordered_map
{
    template <class K, class V>
    class unordered_map
    {
        // é€šè¿‡keyè·å–valueçš„æ“ä½œ
        struct KeyOfValue
        {
            const K& operator()(const pair<K, V>& data)
            {
                return data.first;
            }
        };

    public:
        typedef my_hash_bucket::HashBucket<K, pair<const K, V>, KeyOfValue> HT;
        typedef typename HT::iterator iterator;
        typedef typename HT::const_iterator const_iterator;

    public:
        unordered_map() : _ht()
        {
        }

        iterator begin() {
            return _ht.begin();
        }
        iterator end() { 
            return _ht.end();
        }
        const_iterator begin() const {
            return _ht.begin();
        }
        const_iterator end() const{
            return _ht.end();
        }

        // capacity
        size_t size() const {
            return _ht.size();
        }
        bool empty() const { 
            return _ht.empty();
        }

        // Acess
        V& operator[](const K& key)
        {
            pair<iterator, bool> ret = _ht.Insert(pair<K, V>(key, V()));
            return ret.first->second;
        }
        const V& operator[](const K& key) const {
            pair<iterator, bool> ret = _ht.Insert(pair<K, V>(key, V()));
            return (ret.first)->second;
        }

        // lookup

        iterator find(const K& key) {
            return _ht.Find(key);
        }
        size_t count(const K& key) {
            return _ht.Count(key); 
        }

        // modify
        pair<iterator, bool> insert(const pair<K, V>& valye)
        {
            return _ht.Insert(valye);
        }

        iterator erase(const_iterator position)
        {
            return _ht.Erase(position);
        }

        // bucket
        size_t bucket_count() { 
            return _ht.BucketCount();
        }
        size_t bucket_size(const K& key) { 
            return _ht.BucketSize(key); 
        }

    private:
        HT _ht;
    };
=======
#define _CRT_SECURE_NO_WARNINGS

#include "hash.hpp"

namespace my_unordered_map
{
    template <class K, class V>
    class unordered_map
    {
        // Í¨¹ıkey»ñÈ¡valueµÄ²Ù×÷
        struct KeyOfValue
        {
            const K& operator()(const pair<K, V>& data)
            {
                return data.first;
            }
        };

    public:
        typedef my_hash_bucket::HashBucket<const K, pair<K, V>, KeyOfValue> HT;
        typedef typename HT::iterator iterator;
        typedef typename HT::const_iterator const_iterator;

    public:
        unordered_map() : _ht()
        {
        }

        iterator begin() {
            return _ht.begin();
        }
        iterator end() { 
            return _ht.end();
        }
        const_iterator begin() const {
            return _ht.begin();
        }
        const_iterator end() const{
            return _ht.end();
        }

        // capacity
        size_t size() const {
            return _ht.size();
        }
        bool empty() const { 
            return _ht.empty();
        }

        // Acess
        V& operator[](const K& key)
        {
            pair<iterator, bool> ret = _ht.Insert(pair<K, V>(key, V()));
            return ret.first->second;
        }
        const V& operator[](const K& key) const {
            pair<iterator, bool> ret = _ht.Insert(pair<K, V>(key, V()));
            return (ret.first)->second;
        }

        // lookup

        iterator find(const K& key) {
            return _ht.Find(key);
        }
        size_t count(const K& key) {
            return _ht.Count(key); 
        }

        // modify
        pair<iterator, bool> insert(const pair<K, V>& valye)
        {
            return _ht.Insert(valye);
        }

        iterator erase(const_iterator position)
        {
            return _ht.Erase(position);
        }

        // bucket
        size_t bucket_count() { 
            return _ht.BucketCount();
        }
        size_t bucket_size(const K& key) { 
            return _ht.BucketSize(key); 
        }

    private:
        HT _ht;
    };
>>>>>>> f22b09835d87e0591a93683df1ff2698e109f32e
}