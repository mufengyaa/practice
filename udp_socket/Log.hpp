#pragma once

#include <iostream>
#include <time.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define INFO 0
#define DEBUG 1
#define WARNING 2
#define ERROR 3
#define FATAL 4 // 致命的错误

#define SIZE 1024

class Log
{
public:
    Log()
    {
    }
    void operator()(int level, const char *format, ...)
    {
        time_t t = time(nullptr);
        struct tm *ctime = localtime(&t);

        char leftbuffer[SIZE];
        snprintf(leftbuffer, sizeof(leftbuffer), "[%s][%d-%d-%d %d:%d:%d]", levelToString(level).c_str(),
                 ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
                 ctime->tm_hour, ctime->tm_min, ctime->tm_sec);

        va_list s;
        va_start(s, format);
        char rightbuffer[SIZE];
        vsnprintf(rightbuffer, sizeof(rightbuffer), format, s);
        va_end(s);

        // 格式：默认部分+自定义部分
        char logtxt[SIZE * 2];
        snprintf(logtxt, sizeof(logtxt), "%s %s", leftbuffer, rightbuffer);

        printf("%s\n", logtxt);
    }
    ~Log()
    {
    }

private:
    std::string levelToString(int level)
    {
        switch (level)
        {
        case INFO:
            return "INFO";
        case DEBUG:
            return "DEBUG";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        default:
            return "NONE";
        }
    }
};