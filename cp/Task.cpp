#include "Task.hpp"

Task::Task(int x, int y, function_t func)
    : x_(x), y_(y), func_(func)
{
}
int Task::get_x()
{
    return x_;
}
int Task::get_y()
{
    return y_;
}