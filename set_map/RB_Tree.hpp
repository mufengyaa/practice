#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cassert>
#include <cstdlib>
#include <utility>

// 有迭代器的红黑树
namespace my_RB_Tree
{
    enum colour
    {
        black,
        red
    };

    template <class T>
    struct RBTreeNode // 结点
    {
        RBTreeNode(const T& data)
            : _left(nullptr),
            _right(nullptr),
            _parent(nullptr),
            _col(red),
            _data(data)
        {
        }
        RBTreeNode* _left;
        RBTreeNode* _right;
        RBTreeNode* _parent;
        colour _col;
        T _data;
    };

    template <class T, class Ptr, class Ref> // T是元素类型,ptr是指针类型,ref是引用类型(后两种会有const类型)
    struct RBTreeIterator                    // 迭代器
    {
        typedef RBTreeNode<T> Node;
        typedef RBTreeIterator<T, Ptr, Ref> Self;
        //为了可以能让普通迭代器初始化const迭代器,需要来一个普通迭代器对象
        typedef RBTreeIterator<T, T*, T&> iterator;
        Node* _pNode;

        RBTreeIterator(Node* pNode)
            : _pNode(pNode)
        {
        }
        RBTreeIterator(const iterator& it) // const迭代器时,它是一个初始化;普通迭代器时,它是一个拷贝
            : _pNode(it._pNode)
        {
        }

        // 让迭代器具有类似指针的行为
        Ref operator*()
        {
            return _pNode->_data;
        }
        Ptr operator->()
        {
            return &(_pNode->_data);
        }

        // 让迭代器可以移动：前置/后置++
        Self& operator++()
        {
            Increament();
            return *this;
        }
        Self operator++(int)
        {
            Self tmp(*this);
            Increament();
            return tmp;
        }
        // 让迭代器可以移动：前置/后置--
        Self& operator--()
        {
            DeIncreament();
            return *this;
        }
        Self operator--(int)
        {
            Self tmp(*this);
            DeIncreament();
            return tmp;
        }

        // 让迭代器可以比较
        bool operator!=(const Self& s) const
        {
            return _pNode != s._pNode;
        }
        bool operator==(const Self& s) const
        {
            return _pNode == s._pNode;
        }

    private:
        void Increament();
        void DeIncreament();

    };

    // 为了后序封装map和set，本代码的红黑树会有一个作为哨兵位的头结点
    template <class K, class T, class KeyOfT> // K是关键字的类型,T是元素类型(区分这两个的原因:会用该红黑树封装成set和map,而map是key_value的)
                                              // keyofT是返回关键字类型的值(否则map无法返回)
    class RBTree                              // 红黑树
    {
    public:
        typedef RBTreeNode<T> Node;
        typedef RBTreeIterator<T, T*, T&> iterator;
        typedef RBTreeIterator<T, const T*, const T&> const_iterator;

    public:
        RBTree()
        {
            _pHead = new Node(T());
            _pHead->_left = _pHead;
            _pHead->_parent = nullptr;
            _pHead->_right = _pHead;
        }

        // 在红黑树中插入值为data的节点，插入成功返回true，否则返回false
        std::pair<iterator, bool> Insert(const T& data);

        // 检测红黑树中是否存在值为data的节点，存在返回该节点的地址，否则返回nullptr
        Node* Find(const K& data);

        // 获取红黑树最左侧节点
        Node* LeftMost() const;

        // 获取红黑树最右侧节点
        Node* RightMost() const;

        iterator begin()
        {
            return iterator(LeftMost());
        }
        iterator end()
        {
            return iterator(_pHead);
        }
        const_iterator begin() const
        {
            return const_iterator(LeftMost());
        }
        const_iterator end() const
        {
            return const_iterator(_pHead);
        }

        // 检测红黑树是否为有效的红黑树，注意：其内部主要依靠_IsValidRBTRee函数检测
        bool IsValidRBTRee()
        {
            Node* root = _pHead->_parent;
            if (root->_col == red)
            {
                return false;
            }
            int count = 0;
            find_blacknode(count, _pHead->_parent);
            return _IsValidRBTRee(_pHead->_parent, count, 0);
        }

    private:
        bool _IsValidRBTRee(Node* pRoot, size_t blackCount, size_t pathBlack);
        // 左单旋
        void RotateL(Node* pParent);
        // 右单旋
        void RotateR(Node* pParent);
        // 为了操作树简单起见：获取根节点
        Node*& GetRoot()
        {
            return _pHead->_parent;
        }
        void find_blacknode(int& count, Node* root)
        {
            if (root == nullptr)
            {
                return;
            }
            if (root->_col == black)
            {
                ++count;
            }
            find_blacknode(count, root->_left);
            find_blacknode(count, root->_right);
        }

    private:
        Node* _pHead = nullptr;
    };

    template <class K, class T, class KeyOfT>
    void RBTree<K, T, KeyOfT>::RotateL(Node* pParent)
    {
        Node* cur = pParent->_right, * curleft = cur->_left;

        // 连接p和cur左树,因为该位置被p占据
        pParent->_right = curleft;
        if (curleft)
        {
            curleft->_parent = pParent;
        }

        // 连接父结点
        if (pParent->_parent != _pHead)
        {
            Node* ppnode = pParent->_parent;
            if (ppnode->_left == pParent)
            {
                ppnode->_left = cur;
            }
            else
            {
                ppnode->_right = cur;
            }
            cur->_parent = ppnode;
        }
        else
        {
            _pHead->_parent = cur;
            cur->_parent = _pHead;
        }
        // 连接p和cur
        pParent->_parent = cur;
        cur->_left = pParent;
    }
    template <class K, class T, class KeyOfT>
    void RBTree<K, T, KeyOfT>::RotateR(Node* pParent)
    {
        Node* cur = pParent->_left, * curright = cur->_right;

        // 连接p和cur右树,因为该位置被p占据
        pParent->_left = curright;
        if (curright)
        {
            curright->_parent = pParent;
        }

        // 连接父结点
        if (pParent->_parent != _pHead)
        {
            Node* ppnode = pParent->_parent;
            if (ppnode->_left == pParent)
            {
                ppnode->_left = cur;
            }
            else
            {
                ppnode->_right = cur;
            }
            cur->_parent = ppnode;
        }
        else
        {
            _pHead->_parent = cur;
            cur->_parent = _pHead;
        }
        // 连接p和cur
        pParent->_parent = cur;
        cur->_right = pParent;
    }

    template <class K, class T, class KeyOfT>
    typename RBTree<K, T, KeyOfT>::Node* RBTree<K, T, KeyOfT>::LeftMost() const
    {
        Node* cur = _pHead->_parent;
        while (cur->_left)
        {
            cur = cur->_left;
        }
        return cur;
    }
    template <class K, class T, class KeyOfT>
    typename RBTree<K, T, KeyOfT>::Node* RBTree<K, T, KeyOfT>::RightMost() const
    {
        Node* cur = _pHead->_parent;
        while (cur->_right)
        {
            cur = cur->_right;
        }
        return cur;
    }

    template <class K, class T, class KeyOfT>
    typename RBTree<K, T, KeyOfT>::Node* RBTree<K, T, KeyOfT>::Find(const K& data) // 注意这里,
    {
        Node* cur = _pHead->_parent;
        KeyOfT kot;
        while (cur)
        {
            if (data > kot(cur->_data))
            {
                cur = cur->_right;
            }
            else if (data < kot(cur->_data))
            {
                cur = cur->_left;
            }
            else
            {
                return cur;
            }
        }
        return nullptr;
    }

    template <class K, class T, class KeyOfT>
    std::pair<typename RBTree<K, T, KeyOfT>::iterator, bool> RBTree<K, T, KeyOfT>::Insert(const T& data) // 为了和map适配,要返回pair类型
                                                                                                         //(first是插入元素所在的迭代器,second是bool值,判断是否成功插入)
    {
        KeyOfT kot;
        Node* newnode = nullptr;
        if (_pHead->_parent == nullptr)
        {
            newnode = new Node(data);
            newnode->_col = black;
            _pHead->_parent = newnode;
            newnode->_parent = _pHead;
            return std::make_pair(iterator(newnode), true);
        }
        else
        {
            Node* cur = _pHead->_parent, * parent = cur;
            while (cur)
            {
                if (kot(data) > kot(cur->_data))
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else if (kot(data) < kot(cur->_data))
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else
                {
                    return std::make_pair((iterator)cur, false);
                }
            }

            newnode = new Node(data);
            cur = newnode;
            cur->_parent = parent;
            if (kot(parent->_data) > kot(cur->_data))
            {
                parent->_left = cur;
            }
            else
            {
                parent->_right = cur;
            }

            Node* grandfather = nullptr;
            while (parent != _pHead && parent->_col == red)
            {
                grandfather = parent->_parent; // 因为父结点是红色,所以肯定有爷爷结点(注意红黑树规则:根结点必须是黑色)

                if (grandfather->_left == parent) // 确定父亲位置
                {
                    Node* uncle = grandfather->_right; // 也就能确定叔叔位置

                    if (uncle && uncle->_col == red)
                    {
                        parent->_col = uncle->_col = black;
                        grandfather->_col = red;
                    }
                    else // 如果uncle不存在/为黑,就需要旋转+变色了
                    {
                        // 需要先判断旋转类型(也就是判断 -- parent和cur的相对位置)
                        if (parent->_left == cur)
                        {
                            // 一条偏右的直线,需要右旋
                            RotateR(grandfather);
                            // 旋转完后parent成为根结点

                            // 更改完结点指向后,就可以改颜色了(都是根结点为黑,另外两个为红)
                            parent->_col = black;
                            cur->_col = grandfather->_col = red; // 和cur一层
                        }
                        else
                        {
                            // 拐角在左边,也就是先左旋,再右旋
                            RotateL(parent);
                            RotateR(grandfather);
                            // cur成为根结点

                            // 改颜色
                            cur->_col = black;
                            parent->_col = grandfather->_col = red;
                        }
                        break;
                    }
                }
                else // parent在grandfather的右树
                {
                    Node* uncle = grandfather->_left;

                    if (uncle && uncle->_col == red)
                    {
                        parent->_col = uncle->_col = black;
                        grandfather->_col = red;
                    }
                    else // 如果uncle不存在/为黑,就需要旋转+变色了
                    {
                        // 需要先判断旋转类型(也就是判断 -- parent和cur的相对位置)
                        if (parent->_right == cur)
                        {
                            // 一条偏左的直线,需要左旋
                            RotateL(grandfather);
                            parent->_col = black;
                            cur->_col = grandfather->_col = red; // 和cur一层
                        }
                        else
                        {
                            // 拐角在右边,也就是先右旋,再左旋
                            RotateR(parent);
                            RotateL(grandfather);
                            // 改颜色
                            cur->_col = black;
                            parent->_col = grandfather->_col = red;
                        }
                        break;
                    }
                }
                cur = grandfather; // 注意,这里会改cur的指向,但返回值需要返回插入位置的迭代器,所以需要另外保存
                parent = cur->_parent;
            }
            (_pHead->_parent)->_col = black; // 根结点必须为黑(防止它在上面的循环中被修改)
        }

        _pHead->_left = LeftMost();
        _pHead->_right = RightMost();
        //std::cout << (_pHead->_left)->_data << " " << (_pHead->_right)->_data << std::endl;
        return std::make_pair(iterator(newnode), true);
    }

    template <class K, class T, class KeyOfT>
    bool RBTree<K, T, KeyOfT>::_IsValidRBTRee(Node* cur, size_t blackCount, size_t pathBlack)
    {
        if (cur == nullptr)
        {
            // 到空结点后,就说明一条路径已经走通了,可以用得到的黑色结点数与基准数对比,不一样就说明红黑树错误
            if (pathBlack != blackCount)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        if (cur->_parent)
        {
            Node* ppnode = cur->_parent;
            if (cur->_col == red && ppnode->_col == red)
            {
                return false;
            }
        }
        if (cur->_col == black)
        {
            ++pathBlack;
        }
        return _IsValidRBTRee(cur->_left, blackCount, pathBlack) && _IsValidRBTRee(cur->_right, blackCount, pathBlack);
    }

    template <class T, class Ptr, class Ref>
    void RBTreeIterator<T, Ptr, Ref>::Increament()
    {
        Node* cur = _pNode, * parent = _pNode->_parent;
        if (cur->_right)
        {
            // 找到右子树的最小结点
            Node* curright = cur->_right;
            while (curright->_left)
            {
                curright = curright->_left;
            }
            _pNode = curright;
        }
        else
        {
            while (parent->_parent != cur && parent->_right == cur) // 找到cur是parent的左结点的位置,这样parent的位置就是下一个位置
            {
                cur = parent;
                parent = parent->_parent;
            }
            _pNode = parent;
        }
    }
    template <class T, class Ptr, class Ref>
    void RBTreeIterator<T, Ptr, Ref>::DeIncreament()
    {
        Node* cur = _pNode, * parent = _pNode->_parent;
        if (cur->_left)
        {
            // 找到左子树的最大结点
            Node* curleft = cur->_left;
            while (curleft->_right)
            {
                curleft = curleft->_right;
            }
            _pNode = curleft;
        }
        else
        {
            while (parent->_parent != cur && parent->_left == cur) // 找到cur是parent的左结点的位置,这样parent的位置就是下一个位置
            {
                cur = parent;
                parent = parent->_parent;
            }
            _pNode = parent;
        }
    }
}