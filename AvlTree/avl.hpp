#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cassert>
#include <cstdlib>

namespace my_AvlTree
{
    template <class T>
    struct AvlTreeNode
    {
        AvlTreeNode(const T &data = T())
            : _left(nullptr), _right(nullptr), _parent(nullptr), _data(data), _bf(0)
        {
        }

        AvlTreeNode<T> *_left;
        AvlTreeNode<T> *_right;
        AvlTreeNode<T> *_parent;
        T _data;
        int _bf; // 节点的平衡因子
    };

    // AVL: 二叉搜索树 + 平衡因子的限制
    template <class T>
    class AvlTree
    {
        typedef AvlTreeNode<T> Node;

    public:
        AvlTree()
            : _root(nullptr)
        {
        }

        // 在AVL树中插入值为data的节点
        bool Insert(const T &data);

        // AVL树的验证
        bool IsAvlTree()
        {
            return _IsAvlTree(_root);
        }

        void levelOrder();
        size_t Height()
        {
            return _Height(_root);
        }

    private:
        // 根据AVL树的概念验证pRoot是否为有效的AVL树
        bool _IsAvlTree(Node *root);
        size_t _Height(Node *root);
        // 右单旋
        void RotateR(Node *parent);
        // 左单旋
        void RotateL(Node *parent);
        // 右左双旋
        void RotateRL(Node *parent);
        // 左右双旋
        void RotateLR(Node *parent);

    private:
        Node *_root;
    };

    template <class T>
    void AvlTree<T>::levelOrder()
    {
        Node *root = _root;
        std::vector<std::vector<T>> arr;
        std::queue<Node *> q;
        if (root != nullptr)
        {
            q.push(root);
        }
        while (!q.empty())
        {
            std::vector<T> tmp;  // 存储一层的结点
            int size = q.size(); // 此时队列内的元素就是上一层的结点个数
            for (int i = 0; i < size; ++i)
            {
                tmp.push_back(q.front()->_data);
                if (q.front()->_left)
                { // 有子树就放进队列中
                    q.push(q.front()->_left);
                }
                if (q.front()->_right)
                {
                    q.push(q.front()->_right);
                }
                q.pop(); // 出掉这个父结点
            }
            arr.push_back(tmp);
        }
        for (auto &i : arr)
        {
            for (auto &j : i)
            {
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }

    template <class T>
    bool AvlTree<T>::Insert(const T &data)
    {
        if (_root == nullptr)
        {
            _root = new Node(data);
        }
        else
        {
            Node *cur = _root, *parent = cur, *newnode = nullptr;
            // 找到插入位置
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
            // 插入+改父亲bf
            newnode = new Node(data);
            if (data < parent->_data)
            {
                parent->_left = newnode;
                --parent->_bf;
                newnode->_parent = parent;
            }
            else
            {
                parent->_right = newnode;
                ++parent->_bf;
                newnode->_parent = parent;
            }

            // std::cout << "parent:" << parent->_data << " "
            //           << "newnode:" << newnode->_data << std::endl;

            // 维护bf
            cur = parent; //注意,这里不能直接定义cur的父结点,因为不能保证cur不为空(如果此时只有俩结点,就为空了!!!)
            while (cur != _root)
            {
                Node *pnode = cur->_parent;
                // 更新bf
                if (pnode->_left == cur)
                {
                    --pnode->_bf;
                }
                else
                {
                    ++pnode->_bf;
                }
                // 判断是否继续往上更新
                if (cur->_bf == 0)
                {
                    break;
                }
                else
                {
                    if (pnode->_bf == 2 || pnode->_bf == -2)
                    {
                        // pnode就是不合法的结点,然后判断它该怎么调整
                        if (pnode->_bf == -2 && cur->_bf == -1)
                        {
                            // 右单旋
                            RotateR(pnode);
                        }
                        if (pnode->_bf == 2 && cur->_bf == 1)
                        {
                            // 左单旋
                            RotateL(pnode);
                        }
                        if (pnode->_bf == -2 && cur->_bf == 1)
                        {
                            // 左右双旋
                            RotateLR(pnode);
                        }
                        if (pnode->_bf == 2 && cur->_bf == -1)
                        {
                            // 右左双旋
                            RotateRL(pnode);
                        }
                        break;
                    }
                    else
                    {
                        cur = pnode;
                        pnode = pnode->_parent;
                    }
                }
            }
        }
        return true;
    }

    template <class T>
    bool AvlTree<T>::_IsAvlTree(Node *root)
    {
        if (root == nullptr)
        {
            return true;
        }
        int left_height = _Height(root->_left);
        int right_height = _Height(root->_right);
        if (right_height - left_height != root->_bf)
        {
            std::cout << right_height << std::endl;
            std::cout << left_height << std::endl;
            std::cout << root->_bf << std::endl;
            std::cout << "平衡因子异常" << std::endl;
            return false;
        }
        return abs(right_height - left_height) < 2 && _IsAvlTree(root->_left) && _IsAvlTree(root->_right);
    }

    template <class T>
    size_t AvlTree<T>::_Height(Node *root)
    {
        if (root == nullptr)
        {
            return 0;
        }
        int leftsize = _Height(root->_left) + 1;
        int rightsize = _Height(root->_right) + 1;
        return leftsize > rightsize ? leftsize : rightsize;
    }

    template <class T>
    void AvlTree<T>::RotateL(Node *parent)
    {
        Node *cur = parent->_right, *curleft = cur->_left, *ppnode = parent->_parent;

        // 连接cur上需要修改的子树(比如左旋就是让parent当cur的左子树,那么cur左树就得和parent右边相连)
        if (curleft) // 防止左树为空
        {
            curleft->_parent = parent;
        }
        parent->_right = curleft;

        // 连接cur和parent
        cur->_left = parent;
        // 修改parent父结点的指向
        if (ppnode == nullptr) // 如果此时parent是根结点,那么它没有父结点,且需要更新根结点
        {
            _root = cur;
        }
        else
        {
            if (ppnode->_left == parent)
            {
                ppnode->_left = cur;
            }
            else
            {
                ppnode->_right = cur;
            }
        }

        // 改父结点指向
        cur->_parent = ppnode;
        parent->_parent = cur;
        // 维护bf
        cur->_bf = parent->_bf = 0;
    }

    template <class T>
    void AvlTree<T>::RotateR(Node *parent)
    {
        Node *cur = parent->_left, *curright = cur->_right, *ppnode = parent->_parent;
        // 连接cur上需要修改的子树(右旋就是让parent当cur的右子树,那么cur右树就得和parent左边相连)
        if (curright) // 防止右树为空
        {
            curright->_parent = parent;
        }
        parent->_left = curright;

        // 连接cur和parent
        cur->_right = parent;
        // 修改parent父结点的指向
        if (ppnode == nullptr) // 如果此时parent是根结点,那么它没有父结点,且需要更新根结点
        {
            _root = cur;
        }
        else
        {
            if (ppnode->_left == parent)
            {
                ppnode->_left = cur;
            }
            else
            {
                ppnode->_right = cur;
            }
        }

        // 改父结点指向
        cur->_parent = ppnode;
        parent->_parent = cur;
        // 维护bf
        cur->_bf = parent->_bf = 0;
    }

    template <class T>
    void AvlTree<T>::RotateLR(Node *parent)
    {
        Node *cur = parent->_left, *curright = cur->_right;
        int bf_comp = curright->_bf; // 用于判断插入结点的左右位置

        RotateL(parent->_left); // 让cur成为curright的左子树
        RotateR(parent);        // 让parent成为curright的右子树

        // curright是旋转后子树的根结点
        // 最终让curright的左子树给了cur的右子树,curright的右子树给了parent的左子树

        // if (_Height(curright) == 1)
        // {
        //     curright->_bf = 0;
        //     cur->_bf = 0;
        //     parent->_bf = 0;
        // }
        if (bf_comp == 0)
        {
            curright->_bf = 0;
            cur->_bf = 0;
            parent->_bf = 0;
        }
        else
        {
            if (bf_comp == -1) // 在curright的左边
            {
                // 说明cur的右子树多了1,使其与原先的左子树高度相等
                // 而parent的左子树是h-1,右子树是原先的h
                curright->_bf = 0;
                cur->_bf = 0;
                parent->_bf = 1;
            }
            else if (bf_comp == 1) // 在curright的右边
            {
                // 说明parent的左子树多了1,使其与原先的右子树高度相等
                // 而cur的右子树是h-1,左子树是原先的h
                curright->_bf = 0;
                cur->_bf = -1;
                parent->_bf = 0;
            }
            else
            {
                assert(false);
            }
        }
    }

    template <class T>
    void AvlTree<T>::RotateRL(Node *parent) // 插入结点在curleft的左右子树上其中一个位置
    {
        Node *cur = parent->_right, *curleft = cur->_left;

        int bf_comp = curleft->_bf; // 用于判断插入结点的左右位置
        RotateR(parent->_right);    // 让cur成为curleft的右子树
        RotateL(parent);            // 让parent成为curleft的左子树

        // curleft是旋转后子树的根结点
        // 最终让原curleft的右子树给了cur的左子树,curleft的左子树给了parent的右子树

        // if (_Height(curleft) == 1)
        // {
        //     curleft->_bf = 0;
        //     cur->_bf = 0;
        //     parent->_bf = 0;
        // }
        if (bf_comp == 0)
        {
            curleft->_bf = 0;
            cur->_bf = 0;
            parent->_bf = 0;
        }
        else
        {
            if (bf_comp == -1) // 在curleft的左边
            {
                // 说明parent右子树多了1,使其与原先的左子树高度相等
                // 而cur的左子树是h-1,右子树是原先的h
                curleft->_bf = 0;
                cur->_bf = 1;
                parent->_bf = 0;
            }
            if (bf_comp == 1) // 在curleft的右边
            {
                // 说明cur的左子树多了1,使其与原先的右子树高度相等
                // 而parent的右子树是h-1,左子树是原先的h
                curleft->_bf = 0;
                cur->_bf = 0;
                parent->_bf = -1;
            }
        }
    }
}