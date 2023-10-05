#include "RB_Tree.hpp"

namespace my_set
{
    template <class K>
    class set
    {
        struct SetKeyOfT
        {
            const K& operator()(const K& key)
            {
                return key;
            }
        };
    public:
        typedef typename my_RB_Tree::RBTree<K, K, SetKeyOfT>::const_iterator iterator;
        typedef typename my_RB_Tree::RBTree<K, K, SetKeyOfT>::const_iterator const_iterator;

        const_iterator begin() const
        {
            return _t.begin();
        }
        const_iterator end() const
        {
            return _t.end();
        }

        std::pair<iterator, bool> insert(const K& data) {
            //return _t.Insert(data);

            //这里在构建时,set的insert调用tree的insert
            //而tree中insert的返回值,返回的pair中,第一个成员是tree的普通迭代器
            //然后回到该函数,该函数返回的pair的第一个成员是set中的普通迭代器(实质上是tree中的const迭代器)
            //所以我们本质上是用不同类型的pair在赋值
            //所以要先转换
            std::pair<typename my_RB_Tree::RBTree<K, K, SetKeyOfT>::iterator, bool> ret = _t.Insert(data); //这里是tree的普通迭代器
            iterator it(ret.first);
            return std::pair<iterator, bool>(it,ret.second); //这里是要用普通迭代器初始化一个const迭代器,所以需要在tree迭代器中增加这个功能
        }

    private:
        my_RB_Tree::RBTree<K, K, SetKeyOfT> _t;
    };
}