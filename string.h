#pragma once

#define MALLOC(type,num) (type*)malloc(sizeof(type)*(num))
#define REALLOC(obj,type,num) (type*)realloc(obj,sizeof(type)*(num))

#include<iostream>
#include<stdlib.h>
#include<assert.h>

struct string {
	//���캯��
	string();  //���ַ���
	string(const string& p);  //����
	string(const char* p);  //�ַ���
	string(const char* p, int len);  //ָ���ֽ������ַ���
	string(const char* p, int pos, int len);  //ָ��λ��ָ���ֽ������ַ���
	string(int count, char a);  //ָ���ַ�,����
	//assign����
	string& assign(const char* p);//����ַ�����������Ϊ "ABC"
	string& assign(const char* p, int len);//����ַ�����������Ϊ"AB"�����������ַ�
	string& assign(const char* p, int pos, int len);//����ַ���������Ϊ "ABC" �еĴ� λ��1 ��ʼ������ 1�� �ַ�
	string& assign(int count, char a);//����ַ�����Ȼ���ַ�������Ϊ 5�� 'A'
	//insert����
	string& insert(int i, int count,char a);//��str�±�Ϊ2��λ����� 3�� �ַ�'A'
	string& insert(int i, const char* p);//��str�±�Ϊ2��λ����� �ַ��� "ABC"
	string& insert(int i, const char* p, int count);//��str�±�Ϊ2��λ����� �ַ��� "ABC" �� 1�� �ַ�
	string& insert(int i, const char* p, int pos, int count);//��str�±�Ϊ2��λ����� �ַ��� "ABC" �д�λ�� 1 ��ʼ�� 1 ���ַ�
	//����
	string substr(int i);//��ȡ�Ӵ�����ȡ�� str �� �±�Ϊ2 ��ĩβ���� str1
	string substr(int pos, int len);//��ȡ�Ӵ�����ȡ�� str �� �±�Ϊ2 ��ʼ����ȡ�����ֽڣ��� str1
	const char* data();//��string��תΪ�ַ������飬���ظ�s1
	char* copy(char* p, int count, int pos);//�� str ��� pos λ�ÿ�ʼ������ count�� �ַ�, �浽 s ��

	string operator+(const string& str);
	string& operator+=(const string& str);
	string& operator+=(const char* p);
	string& operator=(const string& str);
	bool operator<(const string& str);
	bool operator<=(const string& str);
	bool operator==(const string& str);
	bool operator>(const string& str);
	bool operator>=(const string& str);
	//��ӡ
	void printarr();
	//����
	~string();

	char* _arr;
	int _size;
	int _capacity;
};