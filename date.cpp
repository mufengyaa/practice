#define _CRT_SECURE_NO_WARNINGS

#include"date.h"

int Date::GetMonthDay(int year, int month) const{
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
Date::Date(int year, int month, int day){		
	if (month > 0 && month < 13 && day > 0 && day <= (*this).GetMonthDay(_year, month)) {
		_year = year;
		_month = month;
		_day = day;
	}
	else {
		cout << "输入非法日期" << endl;
		assert(false);
	}
}
Date::~Date() {
	_year = 0;
	_month = 0;
	_day = 0;
}
Date::Date(const Date& d) {
	_year = d._year;
	_month = d._month;
	_day = d._day;
}
Date& Date::operator=(const Date& d) {
	if (this != &d) {
		_year = d._year;
		_month = d._month;
		_day = d._day;
	}
	return *this;
}
Date& Date::operator+=(int day) {
	if (day >= 0) {
		_day += day;
		int mday = GetMonthDay(_year, _month);
		while (_day > mday) {
			if (_day > mday) {
				_day -= mday;
				_month++;
			}
			if (_month == 13) {
				_month = 1;
				_year++;
			}
			mday = GetMonthDay(_year, _month);
		}
	}
	else {
		*this -= (-day);
	}
	return *this;
}
Date Date::operator+(int day) const {
	Date tmp = *this;
	if (day >= 0) {
		tmp += day;
	}
	else {
		tmp -= (-day);
	}
	return tmp;
}
Date& Date::operator-=(int day) {
	if (day >= 0) {
		_day -= day;
		int mday = GetMonthDay(_year, _month);
		while (_day <= 0) {
			if (_day <= 0) {
				_month--;
				mday = GetMonthDay(_year, _month);
				_day += mday;
			}
			if (_month == 0) {
				_month = 12;
				_year--;
			}
			mday = GetMonthDay(_year, _month);
		}
	}
	else {
		*this += (-day);
	}
	return *this;
}
Date Date::operator-(int day) const {
	Date tmp = *this;
	if (day >= 0) {
		tmp -= day;
	}
	else {
		tmp += (-day);
	}
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
bool Date::operator>(const Date& d) const {
	if (_year > d._year) {
		return true;
	}
	if (_year == d._year && _month > d._month) {
		return true;
	}
	if (_year == d._year && _month == d._month && _day > d._day) {
		return true;
	}
	return false;
}
bool Date::operator==(const Date& d) const {
	if (_year == d._year && _month == d._month && _day == d._day) {
		return true;
	}
	return false;
}
bool Date:: operator >= (const Date& d) const {
	return *this == d || *this > d;
}
bool  Date::operator < (const Date& d) const {
	return !(*this >= d);
}
bool  Date::operator <= (const Date& d) const {
	return !(*this > d);
}
bool  Date::operator != (const Date& d) const {
	return !(*this == d);
}
int Date::operator-(const Date& d) const {
	Date max = *this;
	Date min = d;
	int count = 0, flag = 1;
	if (*this < d) {
		max = d;
		min = *this;
		flag *= -1;
	}
	while (min != max) {
		++min;
		count++;
	}
	return count * flag;
}
ostream& operator<<(ostream& out, const Date& d) {
	out << d._year << "年" << d._month << "月" << d._day << "日" <<endl;
	return out;
}
istream& operator>>(istream& in, Date& d) {
	int month, day;
	in >> d._year >> month >> day;
	if (month > 0 && month < 13 && day>0 && day <= d.GetMonthDay(d._year ,month)) {
		d._month = month;
		d._day = day;
	}
	else {
		cout << "输入非法日期" << endl;
		assert(false);
	}
	return in;
}

void date() {
	Date d1(2023, 5, 5);
	cout << d1;
	d1 += 100;
	cout << d1;
	Date d2 = ++d1;
	cin >> d1;
	cout << d1;
	d2 = d1++;
	Date d3(2023, 5, 5);
	Date d4(2000, 1, 1);
	cout << d3 - d4 << endl;
	cout << d4 - d3 << endl;

	d2 = --d1;
	d2 = d1--;
}
void test() {
	Date d1;
	cout << sizeof(d1) << endl;
}
int main() {
	date();
	//test();
	return 0;
}