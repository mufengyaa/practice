#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <strings.h>
#include <cstring>

#include <string>
#include <functional>
#include <map>
#include <iostream>

#include "Log.hpp"

extern std::string get_time();

Log lg;

const int buff_size = 1024;
using func_t = std::function<std::string(const std::string &)>;

enum
{
    SOCKET_ERR = 1,
    BIND_ERR = 2
};

// 启动服务器时,传入ip地址和端口号
// 手动启动

class udp_server
{
public:
    udp_server(const uint16_t port = 8080, const std::string ip = "0.0.0.0")
        : ip_(ip), port_(port), sockfd_(0)
    {
    }
    void run(func_t func)
    {
        init();
        // 开始收发数据
        char buffer[buff_size];
        std::string message;
        while (true)
        {
            memset(buffer, 0, sizeof(buffer));

            struct sockaddr_in src_addr;
            socklen_t src_len = sizeof(src_addr);

            // 获取数据
            ssize_t n = recvfrom(sockfd_, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<struct sockaddr *>(&src_addr), &src_len);
            if (n < 0)
            {
                lg(WARNING, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }
            buffer[n] = 0;

            std::string id = generate_id(inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port));
            message = id + "sever recvfrom success";
            lg(INFO, message.c_str());

            // 处理数据
            std::string echo_info = func(buffer);

            // 响应给发送端
            sendto(sockfd_, echo_info.c_str(), echo_info.size(), 0, reinterpret_cast<const struct sockaddr *>(&src_addr), src_len);

            message = id + "sever sendto success";
            lg(INFO, message.c_str());
        }
    }
    void chat()
    {
        init();
        // 开始收发数据
        char buffer[buff_size];
        memset(buffer, 0, sizeof(buffer));
        std::string message;

        while (true)
        {
            struct sockaddr_in src_addr;
            socklen_t src_len = sizeof(src_addr);

            // 获取数据
            ssize_t n = recvfrom(sockfd_, buffer, sizeof(buffer) - 1, 0, reinterpret_cast<struct sockaddr *>(&src_addr), &src_len);
            char ip[30];
            // std::cout << inet_ntop(AF_INET, &(src_addr.sin_addr), ip, sizeof(ip) - 1)<<std::endl;

            if (n < 0)
            {
                lg(WARNING, "recvfrom error, errno: %d, err string: %s", errno, strerror(errno));
                continue;
            }
            buffer[n] = 0;
            // std::cout << buffer << std::endl;
            usr_[src_addr.sin_addr.s_addr] = src_addr; // 注册用户表
            // for (auto it : usr_)
            // {
            //     std::cout << inet_ntop(AF_INET, &((it.second).sin_addr), ip, sizeof(ip) - 1) << std::endl;
            // }

            std::string id = generate_id(inet_ntoa(src_addr.sin_addr), ntohs(src_addr.sin_port));
            message = id + "sever recvfrom success";
            lg(INFO, message.c_str());

            // 处理数据
            std::string time_stamp = get_time();
            std::string echo_info = id + time_stamp + buffer;
            memset(buffer, 0, sizeof(buffer));

            // 响应给所有用户端
            send_all(echo_info);

            message = id + "sever sendto success";
            lg(INFO, message.c_str());
        }
    }
    ~udp_server()
    {
        if (sockfd_ > 0)
        {
            close(sockfd_);
        }
    }
    static std::string get_id()
    {
        udp_server obj;
        return obj.generate_id(obj.ip_, obj.port_);
    }

private:
    std::string generate_id(const std::string ip, const uint16_t port)
    {
        return "[" + ip + ":" + std::to_string(port) + "]";
    }
    void init()
    {
        // 创建套接字文件
        sockfd_ = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd_ < 0)
        {
            lg(FATAL, "socket create error, sockfd : %d,%s", sockfd_, strerror(errno));
            exit(SOCKET_ERR);
        }

        // 创建sockaddr结构
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        bzero(&addr, len);

        addr.sin_addr.s_addr = inet_addr(ip_.c_str());
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port_);

        // 绑定套接字信息
        int res = bind(sockfd_, reinterpret_cast<const struct sockaddr *>(&addr), len);
        if (res < 0)
        {
            lg(FATAL, "bind error, sockfd : %d,%s", sockfd_, strerror(errno));
            exit(BIND_ERR);
        }
        lg(INFO, "bind success, sockfd : %d", sockfd_);
    }
    void send_all(const std::string &echo_info)
    {
        char ip[30];
        for (auto it : usr_)
        {
            // std::cout << inet_ntop(AF_INET, &((it.second)->sin_addr), ip, sizeof(ip) - 1)<<std::endl;
            sendto(sockfd_, echo_info.c_str(), echo_info.size(), 0, reinterpret_cast<const struct sockaddr *>(&(it.second)), sizeof(it.second));
        }
    }

private:
    int sockfd_;
    std::string ip_;
    uint16_t port_;
    std::map<in_addr_t, struct sockaddr_in> usr_; //不能是指针,这样下次循环时,指针就换成新的客户端了
};
