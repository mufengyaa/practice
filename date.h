#pragma once

#include<stdlib.h>
#include<iostream>
using namespace std;

class Date {
public:
	void print();
	int GetMonthDay(int year, int month);	// ��ȡĳ��ĳ�µ�����
	Date(int year = 1900, int month = 1, int day = 1);// ȫȱʡ�Ĺ��캯��
	// d2(d1)
	Date(const Date& d);// �������캯��
	// d2 = d3 ->  d2.operator=(&d2, d3)
	Date& operator=(const Date& d);	// ��ֵ���������
	//~Date();// ��������
	Date& operator+=(int day);// ����+=����
	Date operator+(int day);// ����+����
	Date operator-(int day);// ����-����
	Date& operator-=(int day);// ����-=����
	Date& operator++();   // ǰ��++
	Date operator++(int);	// ����++
	Date operator--(int);// ����--
	Date& operator--();// ǰ��--
	bool operator>(const Date& d);// >���������
	bool operator==(const Date& d);// ==���������
	bool operator >= (const Date& d);// >=���������
	bool operator < (const Date& d);// <���������
	bool operator <= (const Date& d);// <=���������
	bool operator != (const Date& d);// !=���������
	int operator-(const Date& d);// ����-���� ��������
	int GetYearDay(int year);

private:
	int _year;
	int _month;
	int _day;
};