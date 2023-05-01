#define _CRT_SECURE_NO_WARNINGS

#include"date.h"

int Date::GetMonthDay(int year, int month) {
	static int m[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
	if (month == 2) {
		if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
			return 29;
		}
	}
	return m[month];
}
int Date::GetYearDay(int year) {
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
		return 366;
	}
	return 365;
}
Date::Date(int year, int month, int day) {		
	this->_year = year;
	this->_month = month;
	this->_day = day;
}
//Date::~Date() {
//	this->_year = 0;
//	this->_month = 0;
//	this->_day = 0;
//}
Date::Date(const Date& d) {
	this->_year = d._year;
	this->_month = d._month;
	this->_day = d._day;
}
Date& Date::operator=(const Date& d) {
	if (this != &d) {
		this->_year = d._year;
		this->_month = d._month;
		this->_day = d._day;
	}
	return *this;
}
Date& Date::operator+=(int day) {
	this->_day += day;
	int mday = GetMonthDay(this->_year, this->_month);
	while (this->_day > mday) {
		if (this->_day > mday) {
			this->_day -= mday;
			this->_month++;
		}
		if (this->_month == 13) {
			this->_month = 1;
			this->_year++;
		}
		mday = GetMonthDay(this->_year, this->_month);
	}
	return *this;
}
Date Date::operator+(int day) {
	Date tmp = *this;
	tmp += day;
	return tmp;
}
Date& Date::operator-=(int day) {
	this->_day -= day;
	int mday = GetMonthDay(this->_year, this->_month);
	while (this->_day <= 0 ) {
		if (this->_day <= 0) {
			this->_month--;
			mday = GetMonthDay(this->_year, this->_month);
			this->_day += mday;
		}
		if (this->_month == 0) {
			this->_month = 12;
			this->_year--;
		}		
		mday = GetMonthDay(this->_year, this->_month);
	}
	return *this;
}
Date Date::operator-(int day) {
	Date tmp = *this;
	tmp -= day;
	return tmp;
}
Date& Date::operator++() {
	(*this) += 1;
	return *this;
}
Date Date::operator++(int) {
	Date tmp = *this;
	(*this) += 1;
	return tmp;
}
Date& Date::operator--() {
	(*this) -= 1;
	return *this;
}
Date Date::operator--(int) {
	Date tmp = *this;
	(*this) -= 1;
	return tmp;
}
bool Date::operator>(const Date& d) {
	if (this->_year > d._year) {
		return true;
	}
	if (this->_year == d._year && this->_month > d._month) {
		return true;
	}
	if (this->_year == d._year && this->_month == d._month && this->_day > d._day) {
		return true;
	}
	return false;
}
bool Date::operator==(const Date& d) {
	if (this->_year == d._year && this->_month == d._month && this->_day == d._day) {
		return true;
	}
	return false;
}
bool Date:: operator >= (const Date& d) {
	return *this == d || *this > d;
}
bool  Date::operator < (const Date& d) {
	return !(*this >= d);
}
bool  Date::operator <= (const Date& d) {
	return !(*this > d);
}
bool  Date::operator != (const Date& d) {
	return !(*this == d);
}
int Date::operator-(const Date& d) {
	int day = this->_day - d._day;
	int lessmonth = this->_month < d._month ? this->_month : d._month;
	int gratermonth= this->_month > d._month ? this->_month : d._month;
	for (int i = lessmonth + 1; i <= gratermonth; i++) {
		day += GetMonthDay(d._year, i);
	}
	int lessyear = this->_year < d._year ? this->_year : d._year;
	int grateryear = this->_year > d._year ? this->_year : d._year;
	for (int i = lessyear + 1; i <= grateryear; i++) {
		day += GetYearDay(i);
	}
	return day;
}
void Date::print() {
	cout << _year << "-" << _month << "-" << _day << endl;
}


void date() {
	Date d1;
	d1.print();
	d1 += 100;
	d1.print();
	d1-=11;
	d1.print();
	Date d2 = ++d1;
	d2.print();
	d1.print();
	d2 = d1++;
	d2.print();
	d1.print();
	cout << d2 - d1 << endl;
	d2 = --d1;
	d2.print();
	d1.print();
	d2 = d1--;
	d2.print();
	d1.print();
}
void test() {
	Date d1;
	cout << sizeof(d1) << endl;
}
int main() {
	date();
	//test();
	Date d2;
	return 0;
}