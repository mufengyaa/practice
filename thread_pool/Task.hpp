#pragma once
// 生成二元整数运算任务(加减乘除),有错误码提示
// 1为/0操作,2为%0操作,3为非法错误

#include <iostream>
#include <string>

using namespace std;

string symbol = "+-*/%";
int sym_size = symbol.size();

class Task
{
public:
    Task() {} // 方便只是为了接收传参而定义一个对象
    Task(int x, int y, char c)
        : x_(x), y_(y), code_(0), op_(c), res_(0)
    {
    }
    int get_result()
    {
        return res_;
    }
    int get_code()
    {
        return code_;
    }
    string get_task()
    {
        string task = to_string(x_) + op_ + to_string(y_) + " = ?";
        return task;
    }
    void operator()()
    {
        switch (op_)
        {
        case '+':
            res_ = x_ + y_;
            break;
        case '-':
            res_ = x_ - y_;
            break;
        case '*':
            res_ = x_ * y_;
            break;
        case '/':
            if (y_ == 0)
            {
                code_ = 1;
                break;
            }
            res_ = x_ / y_;
            break;
        case '%':
            if (y_ == 0)
            {
                code_ = 2;
                break;
            }
            res_ = x_ % y_;
            break;
        default:
            code_ = 3;
            break;
        }
    }
private:
    int x_;
    int y_;
    int res_;
    int code_;
    char op_;
};