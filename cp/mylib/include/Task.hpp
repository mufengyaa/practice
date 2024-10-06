#pragma once

#include <iostream>
#include <string>
#include <functional>

using namespace std;

// typedef function<int(int, int)> function_t;
using function_t = function<int(int, int)>;

class Task
{
public:
    Task() {} // 方便只是为了接收传参而定义一个对象
    Task(int x, int y, function_t func);
    int get_x();
    int get_y();

private:
    int x_;
    int y_;
    function_t func_;
};