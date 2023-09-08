#include <iostream>

namespace key
{
    template <class K>
    class BsTree_node
    {
    public:
        BsTree_node(const K &key)
            : _data(key), _left(nullptr), _right(nullptr) {}

        K _data;
        BsTree_node<K> *_left;
        BsTree_node<K> *_right;
    };

    template <class K>
    class BsTree
    {
        typedef BsTree_node<K> node;

    public:
        // 构造+析构+拷贝
        BsTree()
            : _root(nullptr) {}
        ~BsTree()
        {
            _destroy(_root);
        }
        BsTree(const BsTree<K> &t)
        {
            _root = copy(t._root);
        }
        BsTree<K> &operator=(BsTree<K> t)
        {
            std::swap(_root, t._root);
            return *this;
        }
        // 遍历
        void inOrder()
        {
            _inOrder(_root);
            std::cout << std::endl;
        }
        // 各种功能
        bool insert(const K &key);
        bool insert_R(const K &key)
        {
            return _insert_R(_root, key);
        }
        node *find(const K &key);
        node *find_R(const K &key)
        {
            return _find_R(_root, key);
        }
        bool erase(const K &key);
        bool erase_R(const K &key)
        {
            return _erase_R(_root, key);
        }

    private: // 所需的子函数
        void _destroy(node *root)
        {
            if (root == nullptr)
            {
                return;
            }
            _destroy(root->_left);
            _destroy(root->_right);
            delete root;
            root = nullptr;
        }
        void _inOrder(node *root) // 这里必须得传入参数,不然没法递归,但_root是私有成员,没法在类外调用,所以可以外部包装一层函数
        {
            if (root == nullptr)
            {
                return;
            }
            _inOrder(root->_left);
            std::cout << root->_data << " ";
            _inOrder(root->_right);
        }
        node *copy(node *root)
        {
            node *cp = nullptr;
            if (root == nullptr)
            {
                return nullptr;
            }
            cp = new node(root->_data);
            cp->_left = copy(root->_left);
            cp->_right = copy(root->_right);
            return cp;
        }

        bool _insert_R(node *&root, const K &key);
        node *_find_R(node *root, const K &key);
        bool _erase_R(node *&root, const K &key);

    private: // 成员
        node *_root;
    };

    // 函数实现
    template <class K>
    bool BsTree<K>::insert(const K &key)
    {
        if (_root == nullptr)
        {
            _root = new node(key);
        }
        else
        {
            node *cur = _root, *parent = cur;
            while (cur)
            {
                if (key > cur->_data)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else if (key < cur->_data)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else
                {
                    return false;
                }
            }
            if (key < parent->_data)
            {
                parent->_left = new node(key);
            }
            else
            {
                parent->_right = new node(key);
            }
        }
        return true;
    }
    template <class K>
    bool BsTree<K>::_insert_R(typename BsTree<K>::node *&root, const K &key)
    {
        if (root == nullptr)
        {                         // 该插入了
            root = new node(key); // 此时root是它父结点的左指针/右指针的别名,直接修改即可
            return true;
        }
        if (key > root->_data)
        {
            return _insert_R(root->_right, key); // return 是为了让该函数完成操作返回true/false时,可以快速出[递归出的一堆函数],因为只要返回值,当前函数就执行完成辽
        }
        else if (key < root->_data)
        {
            return _insert_R(root->_left, key);
        }
        else
        {
            return false;
        }
    }

    //
    template <class K>
    typename BsTree<K>::node *BsTree<K>::find(const K &key)
    {
        node *root = _root;
        while (root)
        {
            if (key > root->_data)
            {
                root = root->_right;
            }
            else if (key < root->_data)
            {
                root = root->_left;
            }
            else
            {
                return root;
            }
        }
        return nullptr;
    }
    template <class K>
    typename BsTree<K>::node *BsTree<K>::_find_R(typename BsTree<K>::node *root, const K &key)
    {
        if (root == nullptr)
        {
            return nullptr;
        }
        if (key > root->_data)
        {
            return _find_R(root->_right, key);
        }
        else if (key < root->_data)
        {
            return _find_R(root->_left, key);
        }
        else
        {
            return root;
        }
    }

    //
    template <class K>
    bool BsTree<K>::erase(const K &key)
    {
        node *cur = _root, *parent = cur;
        while (cur) // 找到删除的结点
        {
            if (key > cur->_data)
            {
                parent = cur;
                cur = cur->_right;
            }
            else if (key < cur->_data)
            {
                parent = cur;
                cur = cur->_left;
            }
            else
            {
                break;
            }
        }
        if (cur == nullptr)
        {
            return false;
        }
        // 此时cur指向要被删除的结点
        if (cur->_left == nullptr) // cur左为空
        {
            if (cur == _root) // 防止cur此时是根结点
            {
                _root = cur->_right;
            }
            else
            {
                // 判断cur在parent的左边还是右边,以此让父亲继承儿子的右子树
                if (parent->_left == cur)
                {
                    parent->_left = cur->_right;
                }
                else
                {
                    parent->_right = cur->_right;
                }
            }
        }
        else if (cur->_right == nullptr) // 同理
        {
            if (cur == _root)
            {
                _root = cur->_left;
            }
            else
            {
                if (parent->_left == cur)
                {
                    parent->_left = cur->_left;
                }
                else
                {
                    parent->_right = cur->_left;
                }
            }
        }
        else
        {
            // 当左右子树均在时,找到左子树中最大数,让他当这块的根结点
            node *tmp = cur->_left, *p = cur;
            while (tmp->_right != nullptr)
            {
                p = tmp;
                tmp = tmp->_right;
            }
            // tmp指向此时左子树的最大值,即tmp无右子树
            // 现在要判断tmp在p的左边还是右边(因为有可能tmp的位置就是最大值了)
            if (p->_left == tmp)
            {
                p->_left = tmp->_left;
            }
            else
            {
                p->_right = tmp->_left;
            }
            std::swap(cur->_data, tmp->_data);
            cur = tmp;
        }
        delete cur;
        return true;
    }
    template <class K>
    bool BsTree<K>::_erase_R(typename BsTree<K>::node *&root, const K &key)
    {
        if (root == nullptr)
        {
            return false;
        }
        if (key > root->_data)
        {
            return _erase_R(root->_right, key); // return 是为了让该函数完成操作返回true/false时,可以快速出[递归出的一堆函数],因为只要返回值,当前函数就执行完成辽
        }
        else if (key < root->_data)
        {
            return _erase_R(root->_left, key);
        }
        else // 找到了
        {
            node *tmp = root; // 后续删除用
            if (root->_left == nullptr)
            {
                root = root->_right; // 此时root是它父结点左指针/右指针的别名,直接将其修改至root的非空结点即可
            }
            else if (root->_right == nullptr)
            {
                root = root->_left;
            }
            else
            { // 两边都不为空的话,就得找该结点的代替结点了(左子树中最大的结点)
                node *tr = root->_left;
                while (tr->_right != nullptr)
                {
                    tr = tr->_right;
                }
                std::swap(root->_data, tr->_data);
                return _erase_R(root->_left, tr->_data); // 此时已经互换值了,如果还在root找,万一rt就是root的left,换了就不符合平衡树了,也就根本找不到
                // 但既然tr可以代替root,那root自然也能代替tr,也就是说tr的左右子树是符合平衡树的,所以直接在root的左子树找就行
            }
            delete tmp; // 左子树中寻找时,肯定最终会找到,并且处理掉了那个结点的左子树,就会走到这里,删除,然后层层返回true
            tmp = nullptr;
            return true;
        }
    }
}

namespace key_value
{
    template <class K, class V>
    class BsTree_node
    {
    public:
        BsTree_node(const K &key, const V &value)
            : _key(key), _value(value), _left(nullptr), _right(nullptr) {}

        K _key;
        V _value;
        BsTree_node<K,V> *_left;
        BsTree_node<K,V> *_right;
    };

    template <class K, class V>
    class BsTree
    {
        typedef BsTree_node<K,V> node;

    public:
        // 构造+析构+拷贝
        BsTree()
            : _root(nullptr) {}
        ~BsTree()
        {
            _destroy(_root);
        }
        BsTree(const BsTree<K, V> &t)
        {
            _root = copy(t._root);
        }
        BsTree<K, V> &operator=(BsTree<K, V> t)
        {
            std::swap(_root, t._root);
            return *this;
        }
        // 遍历
        void inOrder()
        {
            _inOrder(_root);
            std::cout << std::endl;
        }
        // 各种功能
        bool insert(const K &key, const V &value);
        node *find(const K &key);
        bool erase(const K &key);

    private: // 所需的子函数
        void _destroy(node *root)
        {
            if (root == nullptr)
            {
                return;
            }
            _destroy(root->_left);
            _destroy(root->_right);
            delete root;
            root = nullptr;
        }
        void _inOrder(node *root) // 这里必须得传入参数,不然没法递归,但_root是私有成员,没法在类外调用,所以可以外部包装一层函数
        {
            if (root == nullptr)
            {
                return;
            }
            _inOrder(root->_left);
            std::cout << root->_key << ": " << root->_value << " ";
            _inOrder(root->_right);
        }
        node *copy(node *root)
        {
            node *cp = nullptr;
            if (root == nullptr)
            {
                return nullptr;
            }
            cp = new node(root->_key, root->_value);
            cp->_left = copy(root->_left);
            cp->_right = copy(root->_right);
            return cp;
        }

    private: // 成员
        node *_root;
    };

    // 函数实现
    template <class K, class V>
    bool BsTree<K, V>::insert(const K &key, const V &value)
    {
        if (_root == nullptr)
        {
            _root = new node(key, value);
        }
        else
        {
            node *cur = _root, *parent = cur;
            while (cur)
            {
                if (key > cur->_key)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else if (key < cur->_key)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else
                {
                    return false;
                }
            }
            if (key < parent->_key)
            {
                parent->_left = new node(key, value);
            }
            else
            {
                parent->_right = new node(key, value);
            }
        }
        return true;
    }

    //
    template <class K, class V>
    typename BsTree<K, V>::node *BsTree<K, V>::find(const K &key)
    {
        node *root = _root;
        while (root)
        {
            if (key > root->_key)
            {
                root = root->_right;
            }
            else if (key < root->_key)
            {
                root = root->_left;
            }
            else
            {
                return root;
            }
        }
        return nullptr;
    }

    //
    template <class K, class V>
    bool BsTree<K, V>::erase(const K &key)
    {
        node *cur = _root, *parent = cur;
        while (cur) // 找到删除的结点
        {
            if (key > cur->_data)
            {
                parent = cur;
                cur = cur->_right;
            }
            else if (key < cur->_data)
            {
                parent = cur;
                cur = cur->_left;
            }
            else
            {
                break;
            }
        }
        if (cur == nullptr)
        {
            return false;
        }
        // 此时cur指向要被删除的结点
        if (cur->_left == nullptr) // cur左为空
        {
            if (cur == _root) // 防止cur此时是根结点
            {
                _root = cur->_right;
            }
            else
            {
                // 判断cur在parent的左边还是右边,以此让父亲继承儿子的右子树
                if (parent->_left == cur)
                {
                    parent->_left = cur->_right;
                }
                else
                {
                    parent->_right = cur->_right;
                }
            }
        }
        else if (cur->_right == nullptr) // 同理
        {
            if (cur == _root)
            {
                _root = cur->_left;
            }
            else
            {
                if (parent->_left == cur)
                {
                    parent->_left = cur->_left;
                }
                else
                {
                    parent->_right = cur->_left;
                }
            }
        }
        else
        {
            // 当左右子树均在时,找到左子树中最大数,让他当这块的根结点
            node *tmp = cur->_left, *p = cur;
            while (tmp->_right != nullptr)
            {
                p = tmp;
                tmp = tmp->_right;
            }
            // tmp指向此时左子树的最大值,即tmp无右子树
            // 现在要判断tmp在p的左边还是右边(因为有可能tmp的位置就是最大值了)
            if (p->_left == tmp)
            {
                p->_left = tmp->_left;
            }
            else
            {
                p->_right = tmp->_left;
            }
            std::swap(cur->_data, tmp->_data);
            cur = tmp;
        }
        delete cur;
        cur=nullptr;
        return true;
    }

}