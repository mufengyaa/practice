#include <iostream>
#include <vector>
#include <string>
using namespace std;

namespace my_bitset
{
    template <size_t N> // 位图中只用传入容量即可(也就是最大数值是多少)
    class bitSet
    {
    public:
        bitSet()
        {
            _a.resize(N / 32 + 1); // 如果该数不是32的整数倍,就得多给一个int
        }
        void set(size_t x) // 置1
        {
            size_t i = x / 32; // 找到应该在第几个int
            size_t j = x % 32; // 找到int中的第几位
            //_a[i] |= (((_a[i] >> j) | 1) << j);
            _a[i] |= (1 << j);
        }
        void reset(size_t x) // 置0
        {
            size_t i = x / 32; // 找到应该在第几个int
            size_t j = x % 32; // 找到int中的第几位
            _a[i] &= (~(1 << j)); //这样除了第j位是0,其他位都是1
        }
        bool test(size_t x)
        {
            size_t i = x / 32; // 找到应该在第几个int
            size_t j = x % 32; // 找到int中的第几位
            return (_a[i] >> j) & 1;
        }

    private:
        vector<int> _a;
    };
}