<<<<<<< HEAD
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
        typedef my_hash_bucket::HashBucket<K, const K, KeyOfV> HT; // ä½¿ç”¨å“ˆå¸Œæ¡¶å°è£…æ— åºset
        typedef typename HT::const_iterator iterator;
        typedef typename HT::const_iterator const_iterator;

    public:
        unordered_set() : _ht()
        {
        }
        iterator begin() const
        {
            return _ht.begin(); //è¿™é‡Œconstä¿®é¥°çš„æ˜¯*this,é‚£ä¹ˆå“ˆå¸Œæ¡¶å¯¹è±¡æ˜¯constçš„
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
            //è¿™é‡Œå’Œçº¢é»‘æ ‘ä¸€æ ·,éƒ½å­˜åœ¨æ™®é€šè¿­ä»£å™¨è½¬åŒ–ä¸ºconstè¿­ä»£å™¨çš„é—®é¢˜
            //æ‰€ä»¥,å’Œä¹‹å‰ä¸€æ ·,è¦ä¸ºè¿­ä»£å™¨å¢åŠ ä¸€ä¸ªæ„é€ å‡½æ•°
            auto it = _ht.Insert(value);
            return make_pair(it.first, it.second);
        }

        iterator erase(iterator position) //æ¥æ”¶çš„æ˜¯å“ˆå¸Œæ¡¶çš„constè¿­ä»£å™¨
        {
            auto it = _ht.Erase(position); //è¿”å›çš„æ˜¯æ™®é€šè¿­ä»£å™¨
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
=======
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
        typedef my_hash_bucket::HashBucket<K, K, KeyOfV> HT; // Ê¹ÓÃ¹şÏ£Í°·â×°ÎŞĞòset
        typedef typename HT::const_iterator iterator;
        typedef typename HT::const_iterator const_iterator;

    public:
        unordered_set() : _ht()
        {
        }
        iterator begin() const
        {
            return _ht.begin(); //ÕâÀïconstĞŞÊÎµÄÊÇ*this,ÄÇÃ´¹şÏ£Í°¶ÔÏóÊÇconstµÄ
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
            //ÕâÀïºÍºìºÚÊ÷Ò»Ñù,¶¼´æÔÚÆÕÍ¨µü´úÆ÷×ª»¯Îªconstµü´úÆ÷µÄÎÊÌâ
            //ËùÒÔ,ºÍÖ®Ç°Ò»Ñù,ÒªÎªµü´úÆ÷Ôö¼ÓÒ»¸ö¹¹Ôìº¯Êı
            auto it = _ht.Insert(value);
            return make_pair(it.first, it.second);
        }

        iterator erase(iterator position) //½ÓÊÕµÄÊÇ¹şÏ£Í°µÄconstµü´úÆ÷
        {
            auto it = _ht.Erase(position); //·µ»ØµÄÊÇÆÕÍ¨µü´úÆ÷
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
>>>>>>> f22b09835d87e0591a93683df1ff2698e109f32e
}