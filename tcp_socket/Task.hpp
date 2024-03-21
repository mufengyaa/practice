#pragma once

#include <iostream>
#include <string>
#include <stdio.h>

#include "helper.hpp"
#include "init.hpp"
// 这里的任务是,服务端在收到客户端的连接后的后续工作

words_init w;

class Task
{
public:
    Task() {} // 方便只是为了接收传参而定义一个对象
    Task(int fd, const char ip[32], const uint16_t port)
        : sockfd_(fd), ip_(ip), port_(port)
    {
    }

    void operator()()
    {
        function();
    }

private:
    void function()
    {
        char buffer[buff_size];
        memset(buffer, 0, sizeof(buffer));

        while (true)
        {
            int n = read(sockfd_, buffer, sizeof(buffer) - 1);
            if (n < 0)
            {
                lg(ERROR, "%s:%d read error, %s", ip_.c_str(), port_, strerror(errno));
                break;
            }
            else if (n == 0)
            {
                lg(INFO, "%s:%d quit", ip_.c_str(), port_);
                break;
            }
            else
            {
                buffer[n] = 0;
                // std::string res = process_info(buffer, ip_, port_);
                std::string res = w.translation(buffer);
                write(sockfd_, res.c_str(), res.size());
            }
        }
    }

private:
    int sockfd_;
    uint16_t port_;
    std::string ip_;
};