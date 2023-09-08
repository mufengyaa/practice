#include "BsTree.hpp"

using namespace std;

void test1()
{
    key::BsTree<int> t;
    t.insert(1);
    t.insert(6);
    t.insert(2);
    t.insert(10);
    t.insert(4);
    t.insert(7);
    t.insert(5);
    t.inOrder();

    t.erase(7);
    t.inOrder();
}
void test2()
{
    key::BsTree<int> t;
    int arr[] = {1, 6, 2, 5, 9, 2, 46, 3};
    for (auto c : arr)
    {
        t.insert(c);
    }
    t.inOrder();

    for (auto c : arr)
    {
        t.erase(c);
        t.inOrder();
    }
}
void test3()
{
    key::BsTree<int> t;
    int arr[] = {1, 6, 2, 5, 9, 2, 46, 3};
    for (auto c : arr)
    {
        t.insert_R(c);
    }
    t.inOrder();
    // for (auto c : arr)
    // {
    //     t.erase(c);
    //     t.inOrder();
    // }

    key::BsTree<int> tmp1(t);
    tmp1.inOrder();

    key::BsTree<int> tmp2;
    tmp2 = t;
    tmp2.inOrder();
}
void test4()
{
    key::BsTree<int> t;
    int arr[] = {1, 6, 2, 5, 9, 2, 46, 3};
    for (auto c : arr)
    {
        t.insert_R(c);
    }
    t.inOrder();

    for (auto c : arr)
    {
        cout << t.find(c) << " ";
    }
    cout << endl;
    for (auto c : arr)
    {
        cout << t.find_R(c) << " ";
    }
    cout << endl;

    for (auto c : arr)
    {
        t.erase_R(c);
        t.inOrder();
    }
}
void test5()
{
    key_value::BsTree<string, string> dict;
    dict.insert("insert", "插入");
    dict.insert("sort", "排序");
    dict.insert("right", "右边");
    dict.insert("date", "日期");

    string str;
    while (cin >> str)
    {
        auto ret = dict.find(str);
        if (ret)
        {
            cout << ret->_value << endl;
        }
        else
        {
            cout << "无此单词" << endl;
        }
    }
}
void test6()
{
    // 统计水果出现的次数
    string arr[] = {"西瓜", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉"};
    key_value::BsTree<string, int> countTree;
    for (auto &str : arr)
    {
        auto ret = countTree.find(str);
        if (ret == nullptr)
        {
            countTree.insert(str, 1);
        }
        else
        {
            ret->_value++;
        }
    }
    countTree.inOrder();
}
int main()
{
    test6();
    return 0;
}