#pragma once

#include <iostream>
#include <string>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#include "Log.hpp"

enum
{
    SOCK_ERROR = 2,
    BIND_ERROR,
    LISTEN_ERROR
};

int def_log = 10;

// 封装出socket接口
class MY_SOCKET
{
public:
    MY_SOCKET()
        : sockfd_(0)
    {
    }
    ~MY_SOCKET()
    {
        Close();
    }
    void Socket()
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ < 0)
        {
            lg(FATAL, "socket create error, sockfd : %d,%s", sockfd_, strerror(errno));
            exit(SOCK_ERROR);
        }
        lg(INFO, "socket create success, sockfd : %d", sockfd_);
    }
    void Bind(int port)
    {
        struct sockaddr_in *addr = new sockaddr_in;
        memset(addr, 0, sizeof(*addr));
        addr->sin_family = AF_INET;
        addr->sin_addr.s_addr = INADDR_ANY; // 接收任何主机发来的数据包
        addr->sin_port = htons(port);

        int t = bind(sockfd_, reinterpret_cast<struct sockaddr *>(addr), sizeof(*addr));
        if (t < 0)
        {
            lg(FATAL, "bind error, sockfd : %d,%s", sockfd_, strerror(errno));
            exit(BIND_ERROR);
        }
        lg(INFO, "bind success, sockfd : %d", sockfd_);

        int opt = 1;
        setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    }
    void Listen()
    {
        if (listen(sockfd_, def_log) < 0)
        {
            lg(FATAL, "listen error, sockfd : %d,%s", sockfd_, strerror(errno));
            exit(LISTEN_ERROR);
        }
        lg(INFO, "listen success, sockfd : %d", sockfd_);
    }
    int Accept(std::string &clientip, uint16_t &clientport)
    {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        memset(&client_addr, 0, client_len);

        int sockfd = accept(sockfd_, reinterpret_cast<struct sockaddr *>(&client_addr), &client_len);
        if (sockfd < 0)
        {
            lg(WARNING, "accept error, %s: %d", strerror(errno), errno);
            return -1;
        }
        char client_ip[32];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, sizeof(client_ip));
        clientip = client_ip;
        clientport = ntohs(client_addr.sin_port);

        return sockfd;
    }
    bool Connect(const std::string &ip, const uint16_t &port)
    {
        struct sockaddr_in *server_addr = new sockaddr_in;
        memset(server_addr, 0, sizeof(*server_addr));

        server_addr->sin_family = AF_INET;
        inet_pton(AF_INET, ip.c_str(), &(server_addr->sin_addr));
        server_addr->sin_port = htons(port);

        int ret = connect(sockfd_, reinterpret_cast<struct sockaddr *>(server_addr), sizeof(*server_addr));
        if (ret < 0)
        {
            printf("connect fail : %s:%d\n", ip.c_str(), port);
            return false;
        }
        // printf("connect success\n");
        return true;
    }
    void Close() // tcp协议在初始化好后,就不需要这个套接字了(子进程)
    {
        close(sockfd_);
    }
    int get_fd()
    {
        return sockfd_;
    }

public:
    int sockfd_;
};