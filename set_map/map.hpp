#include "RB_Tree.hpp"

namespace my_map
{
    template <class K, class V>
    class map
    {
    public:
        typedef std::pair<const K, V> T; // map中key不能变，value可以变
        struct MapKeyOfT
        {
            const V &operator()(const T &data)
            {
                return data.second;
            }
        };
        typedef typename my_RB_Tree::RBTree<K, T, MapKeyOfT>::iterator iterator;
        typedef typename my_RB_Tree::RBTree<K, T, MapKeyOfT>::const_iterator const_iterator;

        iterator begin()
        {
            return _t.begin();
        }
        iterator end()
        {
            return _t.end();
        }

        const_iterator begin() const
        {
            return _t.begin();
        }
        const_iterator end() const
        {
            return _t.end();
        }

        std::pair<iterator, bool> insert(const T &data)
        {
            return _t.Insert(data);
        }
        V &operator[](const K &data)
        {
            auto ret = insert(std::make_pair(data,V()));
            return (ret.first)->second;
        }

    private:
        my_RB_Tree::RBTree<K, T, MapKeyOfT> _t;
    };
}