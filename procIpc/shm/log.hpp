#pragma once

#include <iostream>
#include <string>
#include <time.h>

using namespace std;

#define debug 0
#define notice 1
#define warning 2
#define error 3

const string msg[]{
    "debug", "notice", "warning", "error"};

ostream &log(string message, int level)
{
    cout << "|" << (unsigned)time(nullptr) << "|" << message << "|" << msg[level] << "|";
    return cout;
}