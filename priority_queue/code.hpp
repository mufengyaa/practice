#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>

using namespace std;

namespace bit
{
    template <class T>
    class myless
    {
    public:
        bool operator()(const T &a, const T &b)
        {
            return a < b;
        }
    };
    template <class T>
    class mygreater
    {
    public:
        bool operator()(const T &a, const T &b)
        {
            return a > b;
        }
    };

    template <class T, class Container = vector<T>, class Compare = myless<T>> // 默认是大堆
    class mypriority_queue
    {
    private:
        void adjust_down(int parent)
        { // 大堆找大往上调,往下走
            int child = parent * 2 + 1;
            while (child < size())
            {
                if (child + 1 < size() && comp(c[child], c[child + 1]))
                {
                    child++;
                }
                if (comp(c[parent], c[child]))
                {
                    swap(c[parent], c[child]);
                    parent = child;
                    child = parent * 2 + 1;
                }
                else
                {
                    break;
                }
            }
        }
        void adjust_up(int child)
        {
            int parent = (child - 1) / 2;
            while (child > 0)
            {
                if (comp(c[parent], c[child]))
                {
                    swap(c[parent], c[child]);
                    child = parent;
                    parent = (child - 1) / 2;
                }
                else
                {
                    break;
                }
            }
        }

    public:
        mypriority_queue()
        {
        }

        template <class InputIterator>
        mypriority_queue(InputIterator first, InputIterator last)
        {
            while (first != last)
            {
                c.push(*first);
                ++first;
            }
            for (size_t i = (size() - 1 - 1) / 2; i >= 0; --i)
            { // 建堆(光靠push的向上调整 不足以建立完善的堆结构)
                adjust_down(i);
            }
        }

        bool empty() const
        {
            return c.empty();
        }
        size_t size() const
        {
            return c.size();
        }
        T &top()
        {
            return c.front();
        }
        void push(const T &x)
        {
            c.push_back(x);
            adjust_up(size() - 1);
        }
        void pop()
        {
            swap(top(), c[size() - 1]);
            c.pop_back();
            adjust_down(0);
        }

    private:
        Container c;
        Compare comp;
    };
};