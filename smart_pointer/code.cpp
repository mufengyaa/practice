#include "auto_ptr.hpp"
#include "unique_ptr.hpp"
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"

class A
{
public:
    A(int a = 0)
        : _a(a)
    {
        cout << "A(int a = 0)" << endl;
    }

    ~A()
    {
        cout << this;
        cout << " ~A()" << endl;
    }
    // private:

    int _a;
};
void test1()
{
    my_auto_ptr::auto_ptr<int> p(new int(1));
    my_auto_ptr::auto_ptr<A> p1 = new A(2);
    cout << *p << endl;
    cout << p1->_a << endl;
}
void test2()
{
    my_unique_ptr::unique_ptr<A> p1 = new A(2), p2 = new A(1);
    cout << p1->_a << endl;
    // p1(p2);
}
// void test3()
// {
//     my_shared_ptr::shared_ptr<A> p1 = new A(2), p2(p1);
//     cout << p2->_a++ << endl;
//     cout << p1->_a << endl;
//     my_weak_ptr::weak_ptr<A> p3, p4(p2);
//     p3 = p1;
//     cout << p3->_a << endl;
//     cout << p4->_a << endl;
// }
void test4()
{
    my_shared_ptr::shared_ptr<A> p1 = new A(2);
    my_shared_ptr::shared_ptr<A> p2(new A[3], [](A *p)
                                    { delete[] p; });
    my_shared_ptr::shared_ptr<FILE> p3(fopen("code.cpp", "r"), [](FILE *ptr)
                                       {cout << "fclose:" << ptr << endl;fclose(ptr); });
}
int main()
{
    test4();
    return 0;
}