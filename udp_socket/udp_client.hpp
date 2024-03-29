#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <functional>
#include <strings.h>
#include <cstring>

#include <string>
#include <iostream>

#include <pthread.h>

#include "Log.hpp"
#include "terminal.hpp"

const int buff_size = 1024;

Log lg;

enum
{
    SOCKET_ERR = 1,
    BIND_ERR = 2
};

// 客户端需要提前知道服务端的套接字地址信息
// 日常生活中,我们一般直接通过网址进入,网址就是ip地址,且它会直接和端口号绑定
// 所以,这里我们只能自己手动提供服务端的ip和端口号

// 客户端不需要手动创建套接字,os会自动为我们提供(在首次发送数据时)
struct data
{
    int sockfd_;
    struct sockaddr_in *paddr_;
    socklen_t len_;
};

class udp_client
{
public:
    udp_client(const uint16_t port = 8080, const std::string ip = "47.108.135.233")
        : ip_(ip), port_(port), sockfd_(0)
    {
    }
    void run()
    {
        data *d = init();

        std::string info;
        char buffer[buff_size];
        memset(buffer, 0, sizeof(buffer));
        while (true)
        {
            std::cout << "Please enter:";
            std::getline(std::cin, info);

            // 将消息发送给服务器
            sendto(d->sockfd_, info.c_str(), info.size(), 0, reinterpret_cast<const struct sockaddr *>(d->paddr_), d->len_);
            info.clear();

            struct sockaddr_in addr; // 仅用于填充参数,拿到自己的地址信息没啥意义
            socklen_t len = sizeof(addr);

            // 获取数据
            ssize_t n = recvfrom(d->sockfd_, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<struct sockaddr *>(&addr), &len);
            if (n < 0)
            {
                lg(WARNING, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }
            buffer[n] = 0;

            std::cout << buffer << std::endl;
            memset(buffer, 0, sizeof(buffer));
        }
    }
    void chat()
    {
        data *d = init();

        pthread_t r = 0, w = 0;
        pthread_create(&r, nullptr, input, d);
        pthread_create(&w, nullptr, output, d);

        pthread_join(r, nullptr);
        pthread_join(w, nullptr);
    }

private:
    data *init()
    {
        // 创建套接字文件
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0)
        {
            lg(FATAL, "socket create error, sockfd : %d", sockfd);
            exit(SOCKET_ERR);
        }

        // 创建sockaddr结构
        struct sockaddr_in *svr_paddr = new sockaddr_in;
        socklen_t svr_len = sizeof(*svr_paddr);
        bzero(svr_paddr, svr_len);

        inet_aton(ip_.c_str(), &(svr_paddr->sin_addr));
        svr_paddr->sin_family = AF_INET;
        svr_paddr->sin_port = htons(port_);

        return new data({sockfd, svr_paddr, svr_len});
    }
    static void *input(void *args)
    {
        data *d = reinterpret_cast<data *>(args);

        char ip[30];
        inet_ntop(AF_INET, &((d->paddr_)->sin_addr), ip, sizeof(ip) - 1);

        std::string welcome = "comming...";

        sendto(d->sockfd_, welcome.c_str(), welcome.size(), 0, reinterpret_cast<const struct sockaddr *>(d->paddr_), d->len_);

        std::string info;
        while (true)
        {
            std::cout << "Please enter:";
            std::getline(std::cin, info);

            // 将消息发送给服务器
            sendto(d->sockfd_, info.c_str(), info.size(), 0, reinterpret_cast<const struct sockaddr *>(d->paddr_), d->len_);
            info.clear();
        }
        return nullptr;
    }
    static void *output(void *args)
    {
        data *d = reinterpret_cast<data *>(args);
        // my_dup();

        char buffer[buff_size];
        memset(buffer, 0, sizeof(buffer));
        while (true)
        {
            struct sockaddr_in addr; // 仅用于填充参数,拿到自己的地址信息没啥意义
            socklen_t len = sizeof(addr);

            // 获取数据(所有用户的消息都会获取)
            ssize_t n = recvfrom(d->sockfd_, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<struct sockaddr *>(&addr), &len);
            if (n < 0)
            {
                lg(WARNING, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }
            buffer[n] = 0;

            std::cerr << buffer << std::endl;
            memset(buffer, 0, sizeof(buffer));
        }
        return nullptr;
    }

private:
    int sockfd_;
    std::string ip_;
    uint16_t port_;
};