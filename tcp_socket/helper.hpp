#pragma once

#include <string>
#include <cstring>

enum
{
    SOCK_ERROR = 1,
    BIND_ERROR,
    LISTEN_ERROR,
    CONNECT_ERROR
};

const int buff_size = 1024;

std::string get_time()
{
    time_t t = time(nullptr);
    struct tm *ctime = localtime(&t);

    char time_stamp[1024];
    snprintf(time_stamp, sizeof(time_stamp), "[%d-%d-%d %d:%d:%d]:",
             ctime->tm_year + 1900, ctime->tm_mon + 1, ctime->tm_mday,
             ctime->tm_hour, ctime->tm_min, ctime->tm_sec);
    return time_stamp;
}

std::string generate_id(const std::string ip, const uint16_t port)
{
    return "[" + ip + ":" + std::to_string(port) + "]";
}

std::string process_info(const std::string &info, const std::string ip, const uint16_t port)
{
    std::string time_stamp = get_time();
    std::string id = generate_id(ip, port);

    std::string res = id + time_stamp + info;
    return res;
}