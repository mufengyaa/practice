#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
using namespace std;

// ��ϣͰ
namespace my_hash_bucket
{
    // hsfunc
    // �����õ����ݶ�Ӧ������ֵ->Ȼ����Եõ���Ӧ��hashi
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
            unsigned int seed = 131; // 31 131 1313 13131 131313 ������
            unsigned int hashi = 0;
            for (size_t i = 0; i < size; ++i)
            {
                hashi = hashi * seed + s[i];
            }
            return hashi;
        }
    };

    // ���
    template <class T>
    struct HashBucketNode // ÿ��λ�������ӵĽ��
    {
        HashBucketNode(const T& data)
            : _next(nullptr), _data(data)
        {
        }
        HashBucketNode<T>* _next;
        T _data;
    };

    // ������
    template <class K, class T, class KeyOfValue, class HF>
    class HashBucket; // ǰ������,��Ϊ��������Ҫ�õ���ϣͰ,����ϣͰҲҪ�õ�������
                      // ���������϶����ȵ��Ǹ�,���Ը�������һ��ǰ������,����������ʹ�ù�ϣͰ(��������Ҫ��Ĭ��ֵ)

    template <class K, class T, class Ptr, class Ref, class KeyOfValue, class HF>
    struct HBIterator
    {
        typedef HashBucket<K, T, KeyOfValue, HF> HB;
        typedef HashBucketNode<T>* PNode;
        typedef HBIterator<K, T, Ptr, Ref, KeyOfValue, HF> Self; // �Լ�,���ڷ���

        typedef HBIterator<K, T, T*, T&, KeyOfValue, HF> iterator; //���ڽ��set�в��������

        KeyOfValue kot;

        HBIterator(PNode pNode = nullptr, const HB* pHt = nullptr)  //��Ϊ����ִ����ָ���ǵײ�const���͵����,��������Ҳ��
            : _pnode(pNode), _pHt(pHt) // ��Ҫ���ָ��+��ϣͰ�����ָ��
        {
        }
        HBIterator(const iterator& it)
            : _pnode(it._pnode), _pHt(it._pHt) // ��Ҫ���ָ��+��ϣͰ�����ָ��
            //��Ȼ�����˹���,���ᵼ��,����ͨ������
        {
        }
        Self& operator++()
        {
            // ��ǰ��������ָ�ڵ���нڵ�ʱֱ��ȡ����һ���ڵ�
            if (_pnode->_next)
            {
                _pnode = _pnode->_next;
            }

            else
            {
                // ����һ�����յ�Ͱ�����ظ�Ͱ�е�һ���ڵ�

                size_t hashi = _pHt->HashFunc(kot(_pnode->_data)) + 1;
                _pnode = nullptr; // ������ǰ��ֵ,�Է�;û����һ��Ͱ��,���Է���ָ��յĵ�����
                for (; hashi < _pHt->BucketCount(); ++hashi)
                {
                    if (_pnode = _pHt->_table[hashi]) // ע��,�����õ��˹�ϣͰ��˽�ܳ�Ա,������Ҫ�õ�������Ϊ��ϣͰ����Ԫ
                    {                                 // ����
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

        PNode _pnode; // ��ǰ�����������Ľڵ�(Ҳ���ǵ������ı���)
        const HB* _pHt;     // ��ϣͰ--Ϊ������һ��λ��
        //��Ϊ,����ִ����ָ���ǵײ�const���͵����,�����������ֱ�Ӷ���Ϊ�ײ�const���͵�ָ��,��Ϊ�������ڲ����޸Ĺ�ϣͰ
    };

    // ��ϣͰ
    //  �����key��Ψһ��
    template <class K, class T, class KeyOfValue, class HF = HSFunc<K>>
    // KeyOfValue���ڲ�ͬ���͵�Ԫ��,����key
    // HF���ڲ�ͬ���͵�key,��������
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
            for (size_t i = 0; i < _table.size(); ++i) // ��������,�ҵ���һ��Ͱ
            {
                if (_table[i])
                {
                    return iterator(_table[i], this);
                }
            }
            return iterator(nullptr, this); // ����,thisָ���������Ҫ�Ĺ�ϣͰ�����ָ��
        }
        iterator end()
        {
            return iterator(nullptr, this);
        }
        const_iterator begin() const
        {
            for (size_t i = 0; i < _table.size(); ++i) // ��������,�ҵ���һ��Ͱ
            {
                if (_table[i])
                {
                    return iterator(_table[i], this);
                }
            }
            return const_iterator(nullptr, this); // ����,thisָ���������Ҫ�Ĺ�ϣͰ�����ָ��
        }
        const_iterator end() const
        {
            return const_iterator(nullptr, this); //�����const,���ε���*this,���Թ�ϣͰ������const��
                    //����,�������Ĺ���,��ֱ�ӽ�����ĵ�����ָ���ʼ��,���Ծͻ����"constָ���ʼ����ָͨ��"������
        }

        // ��ϣͰ�е�Ԫ�ز����ظ�
        pair<iterator, bool> Insert(const T& data)
        {
            KeyOfValue kot;
            auto it = Find(kot(data));
            if (it != end())
            {
                return make_pair(it, false);
            }
            if (CheckCapacity()) // ��Ҫ������
            {
                size_t newsize = _size * 2;
                Self newhsb(newsize);

                for (size_t i = 0; i < _size; ++i)
                {
                    PNode cur = _table[i];
                    while (cur) // ��Ͱ�ϵĽ�������λ��
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
            // ͷ��
            newnode->_next = _table[hashi];
            _table[hashi] = newnode;
            ++_size;
            return make_pair(newnode, true);
        }

        // ɾ����ϣͰ��Ϊdata��Ԫ��(data�����ظ�)
        iterator Erase(const_iterator del)
        {
            KeyOfValue kot;
            if (del == end()) {
                return end();
            }
            if (del != Find(kot(*del)))  // ˵���������������ȷ
            {
                return end();
            }

            auto cur = const_iterator(del);
            ++cur;
            size_t hashi = HashFunc(kot(*del));
            PNode prev = _table[hashi];
            if (prev == del._pnode) // ���ɾ�����ǵ�һ�����
            {
                _table[hashi] = del._pnode->_next;
            }
            else
            {
                while (prev && prev->_next->_data != *del) // �ҵ���һ�����
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
        size_t HashFunc(const K& data) const// ������ת����hashi
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
        size_t _size; // ��ϣ������ЧԪ�صĸ���
    };
}
