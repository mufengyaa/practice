<<<<<<< HEAD
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
using namespace std;

// å“ˆå¸Œæ¡¶
namespace my_hash_bucket
{
    // hsfunc
    // ç”¨äºæ‹¿åˆ°æ•°æ®å¯¹åº”çš„æ•´å‹å€¼->ç„¶åå¯ä»¥å¾—åˆ°å¯¹åº”çš„hashi
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
            unsigned int seed = 131; // 31 131 1313 13131 131313 éƒ½å¯ä»¥
            unsigned int hashi = 0;
            for (size_t i = 0; i < size; ++i)
            {
                hashi = hashi * seed + s[i];
            }
            return hashi;
        }
    };

    // ç»“ç‚¹
    template <class T>
    struct HashBucketNode // æ¯ä¸ªä½ç½®ä¸‹é“¾æ¥çš„ç»“ç‚¹
    {
        HashBucketNode(const T& data)
            : _next(nullptr), _data(data)
        {
        }
        HashBucketNode<T>* _next;
        T _data;
    };

    // è¿­ä»£å™¨
    template <class K, class T, class KeyOfValue, class HF>
    class HashBucket; // å‰ç½®å£°æ˜,å› ä¸ºè¿­ä»£å™¨ä¸­è¦ç”¨åˆ°å“ˆå¸Œæ¡¶,è€Œå“ˆå¸Œæ¡¶ä¹Ÿè¦ç”¨åˆ°è¿­ä»£å™¨
                      // ä½†è¿­ä»£å™¨è‚¯å®šæ˜¯å…ˆçš„é‚£ä¸ª,æ‰€ä»¥ç»™è¿­ä»£å™¨ä¸€ä¸ªå‰ç½®å£°æ˜,è®©ä»–å¯ä»¥å…ˆä½¿ç”¨å“ˆå¸Œæ¡¶(å£°æ˜ä¸éœ€è¦ç»™é»˜è®¤å€¼)

    template <class Key, class Data, class Ptr, class Ref, class KeyOfData, class HFc>
    struct HBIterator
    {
        typedef HashBucket<Key, Data, KeyOfData, HFc> HB;
        typedef HashBucketNode<Data>* PNode;
        typedef HBIterator<Key, Data, Ptr, Ref, KeyOfData, HFc> Self; // è‡ªå·±,ç”¨äºè¿”å›

        typedef HBIterator<Key, Data, Data*, Data&, KeyOfData, HFc> iterator; //ç”¨äºè§£å†³setä¸­æ’å…¥çš„é—®é¢˜

        KeyOfData kot;

        HBIterator(PNode pNode = nullptr, const HB* pHt = nullptr)  //å› ä¸ºä¼šå‡ºç°ä¼ å…¥çš„æŒ‡é’ˆæ˜¯åº•å±‚constç±»å‹çš„æƒ…å†µ,æ‰€ä»¥è¿™é‡Œä¹Ÿæ”¹
            : _pnode(pNode), _pHt(pHt) // éœ€è¦ç»“ç‚¹æŒ‡é’ˆ+å“ˆå¸Œæ¡¶å¯¹è±¡çš„æŒ‡é’ˆ
        {
        }
        HBIterator(const iterator& it)
            : _pnode(it._pnode), _pHt(it._pHt) // éœ€è¦ç»“ç‚¹æŒ‡é’ˆ+å“ˆå¸Œæ¡¶å¯¹è±¡çš„æŒ‡é’ˆ
            //è™½ç„¶å¢åŠ äº†æ„é€ ,ä½†ä¼šå¯¼è‡´,ç”¨æ™®é€šå¯¹è±¡æ„é€ 
        {
        }
        Self& operator++()
        {
            // å½“å‰è¿­ä»£å™¨æ‰€æŒ‡èŠ‚ç‚¹åè¿˜æœ‰èŠ‚ç‚¹æ—¶ç›´æ¥å–å…¶ä¸‹ä¸€ä¸ªèŠ‚ç‚¹
            if (_pnode->_next)
            {
                _pnode = _pnode->_next;
            }

            else
            {
                // æ‰¾ä¸‹ä¸€ä¸ªä¸ç©ºçš„æ¡¶ï¼Œè¿”å›è¯¥æ¡¶ä¸­ç¬¬ä¸€ä¸ªèŠ‚ç‚¹

                size_t hashi = _pHt->HashFunc(kot(_pnode->_data)) + 1;
                _pnode = nullptr; // è¿™é‡Œæå‰èµ‹å€¼,ä»¥é˜²;æ²¡æœ‰ä¸‹ä¸€ä¸ªæ¡¶å,å¯ä»¥è¿”å›æŒ‡å‘ç©ºçš„è¿­ä»£å™¨
                for (; hashi < _pHt->BucketCount(); ++hashi)
                {
                    if (_pnode = _pHt->_table[hashi]) // æ³¨æ„,è¿™é‡Œç”¨åˆ°äº†å“ˆå¸Œæ¡¶çš„ç§å¯†æˆå‘˜,æ‰€ä»¥éœ€è¦è®©è¿­ä»£å™¨æˆä¸ºå“ˆå¸Œæ¡¶çš„å‹å…ƒ
                    {                                 // å¥½å¦™
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

        PNode _pnode; // å½“å‰è¿­ä»£å™¨å…³è”çš„èŠ‚ç‚¹(ä¹Ÿå°±æ˜¯è¿­ä»£å™¨çš„æœ¬è´¨)
        const HB* _pHt;     // å“ˆå¸Œæ¡¶--ä¸ºäº†æ‰¾ä¸‹ä¸€ä¸ªä½ç½®
        //å› ä¸º,ä¼šå‡ºç°ä¼ å…¥çš„æŒ‡é’ˆæ˜¯åº•å±‚constç±»å‹çš„æƒ…å†µ,æ‰€ä»¥è¿™é‡Œå¯ä»¥ç›´æ¥å®šä¹‰ä¸ºåº•å±‚constç±»å‹çš„æŒ‡é’ˆ,å› ä¸ºè¿­ä»£å™¨å†…éƒ¨ä¸ä¿®æ”¹å“ˆå¸Œæ¡¶
    };

    // å“ˆå¸Œæ¡¶
    //  è¿™é‡Œçš„keyæ˜¯å”¯ä¸€çš„
    template <class K, class T, class KeyOfValue, class HF = HSFunc<K>>
    // KeyOfValueç”¨äºä¸åŒç±»å‹çš„å…ƒç´ ,è¿”å›key
    // HFç”¨äºä¸åŒç±»å‹çš„key,è¿”å›æ•´å‹
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
            for (size_t i = 0; i < _table.size(); ++i) // éå†æ•°ç»„,æ‰¾åˆ°ç¬¬ä¸€ä¸ªæ¡¶
            {
                if (_table[i])
                {
                    return iterator(_table[i], this);
                }
            }
            return iterator(nullptr, this); // å¾ˆå¦™,thisæŒ‡é’ˆå°±æ˜¯æ‰€éœ€è¦çš„å“ˆå¸Œæ¡¶å¯¹è±¡çš„æŒ‡é’ˆ
        }
        iterator end()
        {
            return iterator(nullptr, this);
        }
        const_iterator begin() const
        {
            for (size_t i = 0; i < _table.size(); ++i) // éå†æ•°ç»„,æ‰¾åˆ°ç¬¬ä¸€ä¸ªæ¡¶
            {
                if (_table[i])
                {
                    return iterator(_table[i], this);
                }
            }
            return const_iterator(nullptr, this); // å¾ˆå¦™,thisæŒ‡é’ˆå°±æ˜¯æ‰€éœ€è¦çš„å“ˆå¸Œæ¡¶å¯¹è±¡çš„æŒ‡é’ˆ
        }
        const_iterator end() const
        {
            return const_iterator(nullptr, this); //è¿™é‡Œçš„const,ä¿®é¥°çš„æ˜¯*this,æ‰€ä»¥å“ˆå¸Œæ¡¶å¯¹è±¡æ˜¯constçš„
                    //ä½†æ˜¯,è¿­ä»£å™¨çš„æ„é€ ,æ˜¯ç›´æ¥å°†ä¼ å…¥çš„è¿­ä»£å™¨æŒ‡é’ˆåˆå§‹åŒ–,æ‰€ä»¥å°±ä¼šå‡ºç°"constæŒ‡é’ˆåˆå§‹åŒ–æ™®é€šæŒ‡é’ˆ"çš„é—®é¢˜
        }

        // å“ˆå¸Œæ¡¶ä¸­çš„å…ƒç´ ä¸èƒ½é‡å¤
        pair<iterator, bool> Insert(const T& data)
        {
            KeyOfValue kot;
            auto it = Find(kot(data));
            if (it != end())
            {
                return make_pair(it, false);
            }
            if (CheckCapacity()) // éœ€è¦æ‰©å®¹äº†
            {
                size_t newsize = _size * 2;
                Self newhsb(newsize);

                for (size_t i = 0; i < _size; ++i)
                {
                    PNode cur = _table[i];
                    while (cur) // æŠŠæ¡¶ä¸Šçš„ç»“ç‚¹æŒ‚åœ¨æ–°ä½ç½®
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
            // å¤´æ’
            newnode->_next = _table[hashi];
            _table[hashi] = newnode;
            ++_size;
            return make_pair(newnode, true);
        }

        // åˆ é™¤å“ˆå¸Œæ¡¶ä¸­ä¸ºdataçš„å…ƒç´ (dataä¸ä¼šé‡å¤)
        iterator Erase(const_iterator del)
        {
            KeyOfValue kot;
            if (del == end()) {
                return end();
            }
            if (del != Find(kot(*del)))  // è¯´æ˜è¿™ä¸ªè¿­ä»£å™¨ä¸æ­£ç¡®
            {
                return end();
            }

            auto cur = const_iterator(del);
            ++cur;
            size_t hashi = HashFunc(kot(*del));
            PNode prev = _table[hashi];
            if (prev == del._pnode) // å¦‚æœåˆ é™¤çš„æ˜¯ç¬¬ä¸€ä¸ªç»“ç‚¹
            {
                _table[hashi] = del._pnode->_next;
            }
            else
            {
                while (prev && prev->_next->_data != *del) // æ‰¾åˆ°ä¸Šä¸€ä¸ªç»“ç‚¹
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
        size_t HashFunc(const K& data) const// å°†æ•°æ®è½¬æ¢æˆhashi
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
        size_t _size; // å“ˆå¸Œè¡¨ä¸­æœ‰æ•ˆå…ƒç´ çš„ä¸ªæ•°
    };
}

=======
#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
using namespace std;

// ¹şÏ£Í°
namespace my_hash_bucket
{
    // hsfunc
    // ÓÃÓÚÄÃµ½Êı¾İ¶ÔÓ¦µÄÕûĞÍÖµ->È»ºó¿ÉÒÔµÃµ½¶ÔÓ¦µÄhashi
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
            unsigned int seed = 131; // 31 131 1313 13131 131313 ¶¼¿ÉÒÔ
            unsigned int hashi = 0;
            for (size_t i = 0; i < size; ++i)
            {
                hashi = hashi * seed + s[i];
            }
            return hashi;
        }
    };

    // ½áµã
    template <class T>
    struct HashBucketNode // Ã¿¸öÎ»ÖÃÏÂÁ´½ÓµÄ½áµã
    {
        HashBucketNode(const T& data)
            : _next(nullptr), _data(data)
        {
        }
        HashBucketNode<T>* _next;
        T _data;
    };

    // µü´úÆ÷
    template <class K, class T, class KeyOfValue, class HF>
    class HashBucket; // Ç°ÖÃÉùÃ÷,ÒòÎªµü´úÆ÷ÖĞÒªÓÃµ½¹şÏ£Í°,¶ø¹şÏ£Í°Ò²ÒªÓÃµ½µü´úÆ÷
                      // µ«µü´úÆ÷¿Ï¶¨ÊÇÏÈµÄÄÇ¸ö,ËùÒÔ¸øµü´úÆ÷Ò»¸öÇ°ÖÃÉùÃ÷,ÈÃËû¿ÉÒÔÏÈÊ¹ÓÃ¹şÏ£Í°(ÉùÃ÷²»ĞèÒª¸øÄ¬ÈÏÖµ)

    template <class K, class T, class Ptr, class Ref, class KeyOfValue, class HF>
    struct HBIterator
    {
        typedef HashBucket<K, T, KeyOfValue, HF> HB;
        typedef HashBucketNode<T>* PNode;
        typedef HBIterator<K, T, Ptr, Ref, KeyOfValue, HF> Self; // ×Ô¼º,ÓÃÓÚ·µ»Ø

        typedef HBIterator<K, T, T*, T&, KeyOfValue, HF> iterator; //ÓÃÓÚ½â¾ösetÖĞ²åÈëµÄÎÊÌâ

        KeyOfValue kot;

        HBIterator(PNode pNode = nullptr, const HB* pHt = nullptr)  //ÒòÎª»á³öÏÖ´«ÈëµÄÖ¸ÕëÊÇµ×²ãconstÀàĞÍµÄÇé¿ö,ËùÒÔÕâÀïÒ²¸Ä
            : _pnode(pNode), _pHt(pHt) // ĞèÒª½áµãÖ¸Õë+¹şÏ£Í°¶ÔÏóµÄÖ¸Õë
        {
        }
        HBIterator(const iterator& it)
            : _pnode(it._pnode), _pHt(it._pHt) // ĞèÒª½áµãÖ¸Õë+¹şÏ£Í°¶ÔÏóµÄÖ¸Õë
            //ËäÈ»Ôö¼ÓÁË¹¹Ôì,µ«»áµ¼ÖÂ,ÓÃÆÕÍ¨¶ÔÏó¹¹Ôì
        {
        }
        Self& operator++()
        {
            // µ±Ç°µü´úÆ÷ËùÖ¸½Úµãºó»¹ÓĞ½ÚµãÊ±Ö±½ÓÈ¡ÆäÏÂÒ»¸ö½Úµã
            if (_pnode->_next)
            {
                _pnode = _pnode->_next;
            }

            else
            {
                // ÕÒÏÂÒ»¸ö²»¿ÕµÄÍ°£¬·µ»Ø¸ÃÍ°ÖĞµÚÒ»¸ö½Úµã

                size_t hashi = _pHt->HashFunc(kot(_pnode->_data)) + 1;
                _pnode = nullptr; // ÕâÀïÌáÇ°¸³Öµ,ÒÔ·À;Ã»ÓĞÏÂÒ»¸öÍ°ºó,¿ÉÒÔ·µ»ØÖ¸Ïò¿ÕµÄµü´úÆ÷
                for (; hashi < _pHt->BucketCount(); ++hashi)
                {
                    if (_pnode = _pHt->_table[hashi]) // ×¢Òâ,ÕâÀïÓÃµ½ÁË¹şÏ£Í°µÄË½ÃÜ³ÉÔ±,ËùÒÔĞèÒªÈÃµü´úÆ÷³ÉÎª¹şÏ£Í°µÄÓÑÔª
                    {                                 // ºÃÃî
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

        PNode _pnode; // µ±Ç°µü´úÆ÷¹ØÁªµÄ½Úµã(Ò²¾ÍÊÇµü´úÆ÷µÄ±¾ÖÊ)
        const HB* _pHt;     // ¹şÏ£Í°--ÎªÁËÕÒÏÂÒ»¸öÎ»ÖÃ
        //ÒòÎª,»á³öÏÖ´«ÈëµÄÖ¸ÕëÊÇµ×²ãconstÀàĞÍµÄÇé¿ö,ËùÒÔÕâÀï¿ÉÒÔÖ±½Ó¶¨ÒåÎªµ×²ãconstÀàĞÍµÄÖ¸Õë,ÒòÎªµü´úÆ÷ÄÚ²¿²»ĞŞ¸Ä¹şÏ£Í°
    };

    // ¹şÏ£Í°
    //  ÕâÀïµÄkeyÊÇÎ¨Ò»µÄ
    template <class K, class T, class KeyOfValue, class HF = HSFunc<K>>
    // KeyOfValueÓÃÓÚ²»Í¬ÀàĞÍµÄÔªËØ,·µ»Økey
    // HFÓÃÓÚ²»Í¬ÀàĞÍµÄkey,·µ»ØÕûĞÍ
    class HashBucket
    {
        template <class K, class T, class Ptr, class Ref, class KeyOfValue, class HF>
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
            for (size_t i = 0; i < _table.size(); ++i) // ±éÀúÊı×é,ÕÒµ½µÚÒ»¸öÍ°
            {
                if (_table[i])
                {
                    return iterator(_table[i], this);
                }
            }
            return iterator(nullptr, this); // ºÜÃî,thisÖ¸Õë¾ÍÊÇËùĞèÒªµÄ¹şÏ£Í°¶ÔÏóµÄÖ¸Õë
        }
        iterator end()
        {
            return iterator(nullptr, this);
        }
        const_iterator begin() const
        {
            for (size_t i = 0; i < _table.size(); ++i) // ±éÀúÊı×é,ÕÒµ½µÚÒ»¸öÍ°
            {
                if (_table[i])
                {
                    return iterator(_table[i], this);
                }
            }
            return const_iterator(nullptr, this); // ºÜÃî,thisÖ¸Õë¾ÍÊÇËùĞèÒªµÄ¹şÏ£Í°¶ÔÏóµÄÖ¸Õë
        }
        const_iterator end() const
        {
            return const_iterator(nullptr, this); //ÕâÀïµÄconst,ĞŞÊÎµÄÊÇ*this,ËùÒÔ¹şÏ£Í°¶ÔÏóÊÇconstµÄ
                    //µ«ÊÇ,µü´úÆ÷µÄ¹¹Ôì,ÊÇÖ±½Ó½«´«ÈëµÄµü´úÆ÷Ö¸Õë³õÊ¼»¯,ËùÒÔ¾Í»á³öÏÖ"constÖ¸Õë³õÊ¼»¯ÆÕÍ¨Ö¸Õë"µÄÎÊÌâ
        }

        // ¹şÏ£Í°ÖĞµÄÔªËØ²»ÄÜÖØ¸´
        pair<iterator, bool> Insert(const T& data)
        {
            KeyOfValue kot;
            auto it = Find(kot(data));
            if (it != end())
            {
                return make_pair(it, false);
            }
            if (CheckCapacity()) // ĞèÒªÀ©ÈİÁË
            {
                size_t newsize = _size * 2;
                Self newhsb(newsize);

                for (size_t i = 0; i < _size; ++i)
                {
                    PNode cur = _table[i];
                    while (cur) // °ÑÍ°ÉÏµÄ½áµã¹ÒÔÚĞÂÎ»ÖÃ
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
            // Í·²å
            newnode->_next = _table[hashi];
            _table[hashi] = newnode;
            ++_size;
            return make_pair(newnode, true);
        }

        // É¾³ı¹şÏ£Í°ÖĞÎªdataµÄÔªËØ(data²»»áÖØ¸´)
        iterator Erase(const_iterator del)
        {
            KeyOfValue kot;
            if (del == end()) {
                return end();
            }
            if (del != Find(kot(*del)))  // ËµÃ÷Õâ¸öµü´úÆ÷²»ÕıÈ·
            {
                return end();
            }

            auto cur = const_iterator(del);
            ++cur;
            size_t hashi = HashFunc(kot(*del));
            PNode prev = _table[hashi];
            if (prev == del._pnode) // Èç¹ûÉ¾³ıµÄÊÇµÚÒ»¸ö½áµã
            {
                _table[hashi] = del._pnode->_next;
            }
            else
            {
                while (prev && prev->_next->_data != *del) // ÕÒµ½ÉÏÒ»¸ö½áµã
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
        size_t HashFunc(const K& data) const// ½«Êı¾İ×ª»»³Éhashi
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
        size_t _size; // ¹şÏ£±íÖĞÓĞĞ§ÔªËØµÄ¸öÊı
    };
}
>>>>>>> f22b09835d87e0591a93683df1ff2698e109f32e
