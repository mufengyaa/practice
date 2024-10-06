#pragma once
#include <bitset>
#include <string>
using namespace std;

namespace my_BloomFilter
{
    struct BKDRHash
    {
        size_t operator()(const string &str)
        {
            size_t hash = 0;
            for (auto ch : str)
            {
                hash = hash * 131 + ch;
            }
            return hash;
        }
    };

    struct APHash
    {
        size_t operator()(const string &str)
        {
            size_t hash = 0;
            for (size_t i = 0; i < str.size(); i++)
            {
                size_t ch = str[i];
                if ((i & 1) == 0)
                {
                    hash ^= ((hash << 7) ^ ch ^ (hash >> 3));
                }
                else
                {
                    hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
                }
            }
            return hash;
        }
    };

    struct DJBHash
    {
        size_t operator()(const string &str)
        {
            size_t hash = 5381;
            for (auto ch : str)
            {
                hash += (hash << 5) + ch;
            }
            return hash;
        }
    };

    template <size_t N, class K = string,
              class Hash1 = BKDRHash, // 这里选择三种哈希函数
              class Hash2 = APHash,
              class Hash3 = DJBHash>
    class BloomFilter
    {
    public:
        BloomFilter() : _b(N)
        {
        }
        void set(const K &data)
        {
            // 先将字符串通过哈希函数转换为整型值
            size_t hashi1 = h1(data) % N;
            size_t hashi2 = h2(data) % N;
            size_t hashi3 = h3(data) % N;
            _b.set(hashi1);
            _b.set(hashi2);
            _b.set(hashi3);
        }
        bool test(const K &data)
        {
            // size_t hashi1 = h1(data)%N;
            // size_t hashi2 = h2(data)%N;
            // size_t hashi3 = h3(data)%N;
            // if (_b.test(hashi1) && _b.test(hashi2) && _b.test(hashi3))
            // {
            //     return true;
            // }
            // else
            // {
            //     return false;
            // }
            // 其实没有必要固定比对三次,只要其中一次比对不成功,就说明data不存在
            size_t hashi1 = h1(data) % N;
            if (_b.test(hashi1) == false)
            {
                return false;
            }
            size_t hashi2 = h2(data) % N;
            if (_b.test(hashi2) == false)
            {
                return false;
            }
            size_t hashi3 = h3(data) % N;
            if (_b.test(hashi3) == false)
            {
                return false;
            }

            return true;
        }

    private:
        bitset<N> _b;
        Hash1 h1;
        Hash2 h2;
        Hash3 h3;
    };
}