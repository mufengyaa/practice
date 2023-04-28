#define _CRT_SECURE_NO_WARNINGS

#include<iostream>

using namespace std;

//using std::cout;
//using std::endl;

////全局域
//int a = 1;
////命名空间域
//namespace one {
//	int a = 2;
//}
//int main() {
//	//局部域
//	int a = 3;
//	std::cout << a << std::endl;
//	std::cout << ::a << std::endl;
//	std::cout << one::a << std::endl;
//	return 0;
//}

//void func(int a, int b = 2, int c = 3) {
//	cout << a << ' ' << b << ' ' << c << endl;
//}
//int main() {
//	func(1);
//	return 0;
//}

//int add(int a, int b) {
//	return a + b;
//}
//double add(double a, double b) {
//	return a + b;
//}
//int main() {
//	cout << add(1, 2) << endl;
//	cout << add(1.1, 0.4) << endl;
//	return 0;
//}

//int& func(int x) {
//	int n = x;
//	n++;
//	return n;
//}
//int main() {
//	int& ret = func(1);
//	cout << ret << endl;
//	func(5);
//	cout << ret << endl;
//
//	//int a = 1, b = 2;
//	//swap(a, b);
//	//cout << a << ' ' << b << endl;
//	//int a = 1;
//	//int& b = a;
//	//int& c = b;
//	//b++;
//	//cout << a << endl;
//	//cout << b << endl;
//	//cout << c << endl;
//
//	//int c = 11;
//	//b = c;
//	//cout << b << endl;
//	//cout << a << endl;
//
//	//cout << &a << endl;
//	//cout << &b << endl;
//	//cout << b << endl;
//
//	//b++;
//	//cout << b << endl;
//	//cout << a << endl;
//}

//int main() {
//	//const int a = 0;
//	//int& b = a;
//	//int b = a;
//	//int x = 1;
//	//int& y = x;
//	//const int& z = x;
//	//y++;
//	//cout << z << endl;
//	double a = 1.1;
//	int b = a;
//	cout << b << endl;
//
//	const int& d = a;
//	cout << d << endl;
//}

//int func() {
//	static int x = 1;
//	return x;
//}
//int& func1() {
//	static int x = 0;
//	x++;
//	return x;
//}
//int main() {
//	int a = func1();
//	cout << a << endl;  //a只接收了func1返回的值
//	func1();
//	cout << a << endl;
//
//	int& b = func1();  //先前已经调用了两次,因此x已经为2,此时再次调用,就为3
//	cout << b << endl;
//	func1(); //b共用了x的空间,x改变b就会改变
//	cout << b << endl;
//
//	const int& c = func1(); //c的权限缩小了,但x的改变仍会改变c
//	cout << c << endl;
//	func1();
//	cout << c << endl;
//
//	return 0;
//}

//int main() {
//	//int a = 1;
//	//auto b = 1 + 1.1;
//	//cout << typeid(a).name() << endl;
//	//cout << typeid(b).name() << endl;
//
//	int x = 1;
//	auto a = &x;
//	auto* b = x;
//	auto* b = &x;
//	return 0;
//}

//auto a = 1, b = 2;
//auto c = 1, d = 1.1;

class A {
public:
	void print() {
		cout << "1" << endl;
	}
//private:
	char a_;
};
int main() {
	//A* p = nullptr;
	//(*p).a_ = 1;
	cout << sizeof(A) << endl;
	//cout << p-> a_ << endl;
	return 0;
}