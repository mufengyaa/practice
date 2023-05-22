#define _CRT_SECURE_NO_WARNINGS

//void Test()
//{
//	// 动态申请一个int类型的空间
//	int* ptr4 = new int;
//
//	// 动态申请一个int类型的空间并初始化为10
//	int* ptr5 = new int(10);
//
//	// 动态申请10个int类型的空间
//	int* ptr6 = new int[3];
//
//	int* ptr1 = new int[3]{ 1 };
//	delete ptr4;
//	delete ptr5;
//	delete[] ptr6;
//}
//int main() {
//	Test();
//	return 0;
//}

#include<iostream>
#include<vector>
#include<string>
using namespace std;
//class A {
//public:
//	int _a = 0;
//	~A() {
//		cout << "~A()" << endl;
//	}
//};
//int main() {
//	A* p = new A[10];
//	for (int i = 0; i < 10; ++i) {
//		p[i]._a = i + 1;
//	}
//	delete p;
//	return 0;
//}

//int main() {
//	const int a = 1;
//	auto& b = a;
//	b = 2;
//	return 0;
//}

//int main() {
//	vector<string> str;
//	string tmp;
//	int i = 0;
//	while (cin >> tmp) {
//		for (auto& a : tmp) {
//			a = toupper(a);
//		}
//		str.push_back(tmp);
//		cout << str[i++] << endl;
//	}
//	return 0;
//}

int main() {
	int arr[14 - 7];
	vector<int> a;
	for (int i = 0; i < 10; i++) {
		a.push_back(i + 1);
	}
	for (auto it = a.begin(); it != a.end(); ++it) {
		(*it) *= 2;
		cout << (*it) << " ";
	}
	return 0;
}