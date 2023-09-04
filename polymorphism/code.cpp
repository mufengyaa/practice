#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

// class A
// {
// public:
//     ~A()
//     {
//         ;
//     }
//     void work()
//     {
//         int a = 0;
//     }
//     virtual void func()
//     {
//         cout << "func" << endl;
//     }
//     int _a = 1;
// };
// class B : public A
// {
// public:
//     ~B()
//     {
//         delete[] _b;
//     }
//     virtual void func()
//     {
//         cout << "func" << endl;
//     }
//     int *_b = new int[10];
// };

// int main()
// {
//     // A* p1=new A;
//     // A* p2=new B;

//     // delete p1;
//     // delete (B*)p2;

//     B b;
//     printf("%p\n", *((int *)&b));

//     int a = 0;
//     printf("%p\n",&a);
//     cout << "栈:" << &a << endl; // 栈

//     int *p = new int;
//     cout << "堆:" << p << endl; // 堆

//     static int c = 2;
//     cout << "静态区:" << &c << endl;

//     const char *str = "hello";
//     printf("常量区:%p\n", str);

//     return 0;
// }

#include<iostream>

using namespace std;

//class A
//{
//public:
//	int _a;
//};
//
//class B : virtual public A
//{
//public:
//	int _b;
//};
//class C : virtual public A
//{
//public:
//	int _c;
//};
//
//class D : public B, public C
//{
//public:
//	int _d;
//};
//
//int main()
//{
//	D d;
//	d.B::_a = 1;
//	d.C::_a = 2;
//	d._b = 3;
//	d._c = 4;
//	d._d = 5;
//
//	return 0;
//}

//int main()
//{
//	D d;
//	d._a = 1;
//
//	B b;
//	b._a = 2;
//	b._b = 3;
//
//	B* ptr = &b;
//	ptr->_a++;
//
//	ptr = &d;
//	ptr->_a++;
//
//	return 0;
//}

class A
{
public:
    virtual void func()
    {
        cout << "A::func" << endl;
    }
    virtual void work() {
        int a = 0;
    }
    int _a = 1;
};
//class B : public A
//{
//public:
//    virtual void func()
//    {
//        cout << "func" << endl;
//    }
//    virtual void work1() {
//        int a = 0;
//    }
//    int _b = 2;
//};
class B 
{
public:
    virtual void func()
    {
        cout << "B::func" << endl;
    }
    void work1() {
        int a = 0;
    }
    int _b = 2;
};
class C : public A,public B
{
public:
    //virtual void func()
    //{
    //    cout << "C::func" << endl;
    //}
    virtual void work1() {
        int a = 0;
    }
    int _b = 2;
};

typedef void(*function); //将void返回类型,无参数的函数类型重命名为function
void print_vf(function vb[]) {
    cout << "虚表指针:" << vb << endl;
    for (int i = 0; vb[i] != nullptr; ++i) {
        cout << i + 1 << ":" << vb[i] << endl;
    }
}

int main()
{
    //A* pa = new A;
    //B* pb = new B;
    //pa = new C;
    //pa->func();
    //pb = new C;
    //pb->func();
    C c;
    print_vf((function*)*((int*)&c)); //拿到A的虚表地址,它在c的开头

    char* pb = ((char*)&c) + sizeof(A); //将指针指向b的位置(要先转成char*,[指针移动]会移动[指向的类型大小]的大小
    int* p1 = (int*)pb; //拿到这个位置的前4个字节,也就是虚表地址
    function* p = (function*)(*p1);
    print_vf(p);

    //print_vf((function*)(*(int*)&b)); 
    //先将b对象地址强转成int*,拿到前4个字节的空间
    //然后解引用,拿到前4个字节的内容(也就是虚表指针)
    //再将其转成 存储function类型的数组(虚表本身就是函数指针数组)
    return 0;
}

class A
{
public:
    virtual void func(int val = 1) {
        std::cout << "A->" << val << std::endl;
    }
    virtual void test() { 
        func(); 
    }
};

class B : public A
{
public:
    void func(int val = 0) { 
        std::cout << "B->" << val << std::endl;
    }
};

int main(int argc, char* argv[])
{
    B* p = new B;
    p->test();
    return 0;
}