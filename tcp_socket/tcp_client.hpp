#include <iostream>
#include <string>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <signal.h>

#include "Log.hpp"
#include "helper.hpp"

class tcp_client
{
public:
    tcp_client(const uint16_t port = 8080, const std::string ip = "47.108.135.233")
        : sockfd_(-1), port_(port), ip_(ip)
    {
    }
    ~tcp_client()
    {
    }
    void run()
    {
        struct sockaddr_in *server_addr = init();
        int ret = connect(sockfd_, reinterpret_cast<struct sockaddr *>(server_addr), sizeof(*server_addr));
        if (ret < 0)
        {
            std::cout << "connect fail" << std::endl;
            exit(CONNECT_ERROR);
        }

        while (true)
        {

            std::cout << "please enter:" << std::endl;
            std::string buffer;
            std::getline(std::cin, buffer);

            int n = write(sockfd_, buffer.c_str(), buffer.size());
            if (n < 0)
            {
                break;
            }

            char info[1024];
            memset(info, 0, sizeof(info));

            n = read(sockfd_, info, sizeof(info) - 1);
            if (n > 0)
            {
                info[n] = 0;
                std::cout << info << std::endl;
            }
            else
            {
                break;
            }
        }
        close(sockfd_);
        delete server_addr;
    }

    void short_service()
    {
        signal(SIGPIPE, SIG_IGN); // 忽略这个信号,以便我们进行重连

        struct sockaddr_in *server_addr = new sockaddr_in;
        memset(server_addr, 0, sizeof(*server_addr));
        server_addr->sin_family = AF_INET;
        inet_pton(AF_INET, ip_.c_str(), &(server_addr->sin_addr));
        server_addr->sin_port = htons(port_);

        while (true) // 为了让io失败后,可以进入重连功能
        {
            bool is_reconnect = false;
            int re_num = 5;

            do
            {
                sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
                if (sockfd_ < 0)
                {
                    exit(SOCK_ERROR);
                }
                int ret = connect(sockfd_, reinterpret_cast<struct sockaddr *>(server_addr), sizeof(*server_addr));
                if (ret < 0)
                {
                    is_reconnect = true;
                    --re_num;
                    std::cout << "im reconnect ing..." << std::endl;
                    sleep(2);
                }
                else
                {
                    if (is_reconnect == true)
                    {
                        std::cout << "connect success !" << std::endl;
                    }
                    is_reconnect = false;
                }
            } while (is_reconnect && re_num);

            if (re_num == 0)
            {
                break; // 重连次数达到限制,客户端退出
            }

            std::cout << "please enter:" << std::endl;
            std::string buffer;
            std::getline(std::cin, buffer);

            int n = write(sockfd_, buffer.c_str(), buffer.size());
            if (n < 0)
            {
                continue; // 除了处理读失败,写失败也需要(向不存在的文件写入)
            }

            char info[1024];
            memset(info, 0, sizeof(info));

            n = read(sockfd_, info, sizeof(info) - 1);
            if (n > 0)
            {
                info[n] = 0;
                std::cout << info << std::endl;
            }
            else
            {
                continue;
            }
            close(sockfd_);
        }
    }

private:
    struct sockaddr_in *init()
    {
        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ < 0)
        {
            lg(FATAL, "socket create error, sockfd : %d,%s", sockfd_, strerror(errno));
            exit(SOCK_ERROR);
        }

        struct sockaddr_in *server_addr = new sockaddr_in;
        memset(server_addr, 0, sizeof(*server_addr));
        server_addr->sin_family = AF_INET;
        inet_pton(AF_INET, ip_.c_str(), &(server_addr->sin_addr));
        server_addr->sin_port = htons(port_);

        return server_addr;
    }

private:
    int sockfd_;
    uint16_t port_;
    std::string ip_;
};