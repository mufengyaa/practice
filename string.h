#pragma once

#define MALLOC(type,num) (type*)malloc(sizeof(type)*(num))
#define REALLOC(obj,type,num) (type*)realloc(obj,sizeof(type)*(num))

#include<iostream>
#include<stdlib.h>
#include<assert.h>

struct string {
	//构造函数
	string();  //空字符串
	string(const string& p);  //拷贝
	string(const char* p);  //字符串
	string(const char* p, int len);  //指定字节数的字符串
	string(const char* p, int pos, int len);  //指定位置指定字节数的字符串
	string(int count, char a);  //指定字符,个数
	//assign函数
	string& assign(const char* p);//清空字符串，并设置为 "ABC"
	string& assign(const char* p, int len);//清空字符串，并设置为"AB"，保留两个字符
	string& assign(const char* p, int pos, int len);//清空字符串，设置为 "ABC" 中的从 位置1 开始，保留 1个 字符
	string& assign(int count, char a);//清空字符串，然后字符串设置为 5个 'A'
	//insert函数
	string& insert(int i, int count,char a);//在str下标为2的位置添加 3个 字符'A'
	string& insert(int i, const char* p);//在str下标为2的位置添加 字符串 "ABC"
	string& insert(int i, const char* p, int count);//在str下标为2的位置添加 字符串 "ABC" 中 1个 字符
	string& insert(int i, const char* p, int pos, int count);//在str下标为2的位置添加 字符串 "ABC" 中从位置 1 开始的 1 个字符
	//拷贝
	string substr(int i);//提取子串，提取出 str 的 下标为2 到末尾，给 str1
	string substr(int pos, int len);//提取子串，提取出 str 的 下标为2 开始，提取三个字节，给 str1
	const char* data();//将string类转为字符串数组，返回给s1
	char* copy(char* p, int count, int pos);//将 str 里的 pos 位置开始，拷贝 count个 字符, 存到 s 里

	string operator+(const string& str);
	string& operator+=(const string& str);
	string& operator+=(const char* p);
	string& operator=(const string& str);
	bool operator<(const string& str);
	bool operator<=(const string& str);
	bool operator==(const string& str);
	bool operator>(const string& str);
	bool operator>=(const string& str);
	//打印
	void printarr();
	//析构
	~string();

	char* _arr;
	int _size;
	int _capacity;
};