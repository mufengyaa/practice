#include <iostream>
#include <vector>
#include <list>
#include <deque>
using namespace std;

namespace bit
{
    template <class T, class Con = deque<T>> // con是适配器(也就是容器适配器,其他容器经过包装 -> stack)
    class mystack
    {
    public:
        mystack() {}
        void push(const T &x)
        {
            _c.push_back(x);
        }
        void pop()
        {
            _c.pop_back();
        }
        T &top()
        {
            return _c.back();
        }
        const T &top() const
        {
            return _c.front();
        }
        size_t size() const
        {
            return _c.size();
        }
        bool empty() const
        {
            return _c.empty();
        }

    private:
        Con _c;
    };
};