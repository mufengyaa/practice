#pragma once

#include <iostream>
#include <string>
#include <functional>

using namespace std;
//typedef function<int(int, int)> function_t;
using function_t = function<int(int, int)>;

class Task
{
public:
    Task() {} // 方便只是为了接收传参而定义一个对象
    Task(int x, int y, function_t func)
        : x_(x), y_(y), func_(func)
    {
    }
    int get_x()
    {
        return x_;
    }
    int get_y()
    {
        return y_;
    }

private:
    int x_;
    int y_;
    function_t func_;
};