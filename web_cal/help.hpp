#pragma once

#include<map>
#include<string>

#define buff_size 256

enum
{
    ERROR_DIVIDE_BY_ZERO = 1, // 除0/模0错误
    ERROR_INVALID_EXPRESSION, // 无效表达式错误(符号识别不了)
    ERROR_MEMORY_OVERFLOW,    // 内存溢出错误
    ERROR_SYNTAX_ERROR,       // 语法错误(比如没有=)
};

void init_err(std::map<int,std::string>& m){
    m[ERROR_DIVIDE_BY_ZERO]="除0/模0错误";
    m[ERROR_INVALID_EXPRESSION]="无效表达式";
    m[ERROR_MEMORY_OVERFLOW]="内存溢出";
    m[ERROR_SYNTAX_ERROR]="语法错误";
}