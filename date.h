#pragma once

#include<stdlib.h>
#include<iostream>
using namespace std;

class Date {
public:
	void print();
	int GetMonthDay(int year, int month);	// 获取某年某月的天数
	Date(int year = 1900, int month = 1, int day = 1);// 全缺省的构造函数
	// d2(d1)
	Date(const Date& d);// 拷贝构造函数
	// d2 = d3 ->  d2.operator=(&d2, d3)
	Date& operator=(const Date& d);	// 赋值运算符重载
	//~Date();// 析构函数
	Date& operator+=(int day);// 日期+=天数
	Date operator+(int day);// 日期+天数
	Date operator-(int day);// 日期-天数
	Date& operator-=(int day);// 日期-=天数
	Date& operator++();   // 前置++
	Date operator++(int);	// 后置++
	Date operator--(int);// 后置--
	Date& operator--();// 前置--
	bool operator>(const Date& d);// >运算符重载
	bool operator==(const Date& d);// ==运算符重载
	bool operator >= (const Date& d);// >=运算符重载
	bool operator < (const Date& d);// <运算符重载
	bool operator <= (const Date& d);// <=运算符重载
	bool operator != (const Date& d);// !=运算符重载
	int operator-(const Date& d);// 日期-日期 返回天数
	int GetYearDay(int year);

private:
	int _year;
	int _month;
	int _day;
};