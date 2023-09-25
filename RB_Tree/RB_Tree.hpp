#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cassert>
#include <cstdlib>

namespace my_RB_Tree
{

    enum colour
    {
        black,
        red
    };
    template <class T>
    struct RBTreeNode
    {
        RBTreeNode(const T &data)
            : _left(nullptr),
              _right(nullptr),
              _parent(nullptr),
              _col(red),
              _data(data)
        {
        }
        RBTreeNode *_left;
        RBTreeNode *_right;
        RBTreeNode *_parent;
        colour _col;
        T _data;
    };

    // 请模拟实现红黑树的插入--注意：为了后序封装map和set，本文在实现时给红黑树多增加了一个头结点
    template <class T>
    class RBTree
    {
        typedef RBTreeNode<T> Node;

    public:
        RBTree()
        {
            _pHead = new Node(T());
            _pHead->_left = _pHead;
            _pHead->_parent = nullptr;
            _pHead->_right = _pHead;
        }

        // 在红黑树中插入值为data的节点，插入成功返回true，否则返回false
        // 注意：为了简单起见，本次实现红黑树不存储重复性元素
        bool Insert(const T &data);

        // 检测红黑树中是否存在值为data的节点，存在返回该节点的地址，否则返回nullptr
        Node *Find(const T &data);

        // 获取红黑树最左侧节点
        Node *LeftMost();

        // 获取红黑树最右侧节点
        Node *RightMost();

        // 检测红黑树是否为有效的红黑树，注意：其内部主要依靠_IsValidRBTRee函数检测
        bool IsValidRBTRee()
        {
            Node *root = _pHead->_parent;
            if (root->_col == red)
            {
                return false;
            }
            int count = 0;
            find_blacknode(count, _pHead->_parent);
            return _IsValidRBTRee(_pHead->_parent, count, 0);
        }

    private:
        bool _IsValidRBTRee(Node *pRoot, size_t blackCount, size_t pathBlack);
        // 左单旋
        void RotateL(Node *pParent);
        // 右单旋
        void RotateR(Node *pParent);
        // 为了操作树简单起见：获取根节点
        Node *&GetRoot()
        {
            return _pHead->_parent;
        }
        void find_blacknode(int &count, Node *root)
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
        Node *_pHead = nullptr;
    };

    template <class T>
    void RBTree<T>::RotateL(Node *pParent)
    {
        Node *root = pParent->_parent;
        Node *cur = root->_right, *curleft = cur->_left;

        // 连接p和cur左树,因为该位置被p占据
        pParent->_right = curleft;
        if (curleft)
        {
            curleft->_parent = pParent;
        }

        // 连接父结点
        if (pParent->_parent)
        {
            Node *ppnode = pParent->_parent;
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
            root = cur;
        }
        // 连接p和cur
        pParent->_parent = cur;
        cur->_left = pParent;
    }
    template <class T>
    void RBTree<T>::RotateR(Node *pParent)
    {
        Node *root = pParent->_parent;
        Node *cur = root->_left, *curright = cur->_right;

        // 连接p和cur右树,因为该位置被p占据
        pParent->_left = curright;
        if (curright)
        {
            curright->_parent = pParent;
        }

        // 连接父结点
        if (pParent->_parent)
        {
            Node *ppnode = pParent->_parent;
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
            root = cur;
        }
        // 连接p和cur
        pParent->_parent = cur;
        cur->_right = pParent;
    }

    template <class T>
    typename RBTree<T>::Node *RBTree<T>::LeftMost()
    {
        Node *cur = _pHead->_parent;
        while (cur->_left)
        {
            cur = cur->_left;
        }
        return cur;
    }
    template <class T>
    typename RBTree<T>::Node *RBTree<T>::RightMost()
    {
        Node *cur = _pHead->_parent;
        while (cur->_right)
        {
            cur = cur->_right;
        }
        return cur;
    }

    template <class T>
    typename RBTree<T>::Node *RBTree<T>::Find(const T &data)
    {
        Node *cur = _pHead->_parent;
        while (cur)
        {
            if (data > cur->_data)
            {
                cur = cur->_right;
            }
            else if (data < cur->_data)
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

    template <class T>
    bool RBTree<T>::Insert(const T &data)
    {
        if (_pHead->_parent == nullptr)
        {
            _pHead->_parent = new Node(data);
            _pHead->_parent->_col = black;
        }
        else
        {
            Node *cur = _pHead->_parent, *parent = cur;
            while (cur)
            {
                if (data > cur->_data)
                {
                    parent = cur;
                    cur = cur->_right;
                }
                else if (data < cur->_data)
                {
                    parent = cur;
                    cur = cur->_left;
                }
                else
                {
                    return false;
                }
            }
            cur = new Node(data);
            Node *grandfather = nullptr;
            while (parent && parent->_col == red)
            {
                grandfather = parent->_parent; // 因为父结点是红色,所以肯定有爷爷结点(注意红黑树规则:根结点必须是黑色)

                if (grandfather->_left == parent) // 确定父亲位置
                {
                    Node *uncle = grandfather->_right; // 也就能确定叔叔位置

                    if (uncle && uncle->_col == red)
                    {
                        parent->_col = uncle->_col = black;
                        grandfather->_col = black;
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
                            // 拐角在右边,也就是先右旋,再左旋
                            RotateR(parent);
                            RotateL(grandfather);
                            // cur成为根结点

                            // 改颜色
                            cur->_col = black;
                            parent->_col = grandfather->_col = red;
                        }
                        break;
                    }
                }
                else // cur在parent的右树
                {
                    Node *uncle = grandfather->_left;

                    if (uncle && uncle->_col == red)
                    {
                        parent->_col = uncle->_col = black;
                        grandfather->_col = black;
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
                            // 拐角在左边,也就是先左旋,再右旋
                            RotateL(parent);
                            RotateR(grandfather);
                            // 改颜色
                            cur->_col = black;
                            parent->_col = grandfather->_col = red;
                        }
                        break;
                    }
                }
                cur = grandfather;
                parent = cur->_parent;
            }
            (_pHead->_parent)->_col = black;
        }
        return true;
    }
    template <class T>
    bool RBTree<T>::_IsValidRBTRee(Node *cur, size_t blackCount, size_t pathBlack)
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
            Node *ppnode = cur->_parent;
            if (cur->_col == red && ppnode->_col == red)
            {
                return false;
            }
        }
        if (cur->_col == black)
        {
            ++pathBlack;
        }
        return _IsValidRBTRee(cur->_left,blackCount,pathBlack) && _IsValidRBTRee(cur->_right,blackCount,pathBlack);
    }

}