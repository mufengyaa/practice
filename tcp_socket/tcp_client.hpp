#include <iostream>
#include <string>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#include "Log.hpp"
#include "helper.hpp"

class tcp_client
{
public:
    tcp_client(const uint16_t port = 8080, const std::string ip = "47.108.135.233")
        : sockfd_(-1), port_(port), ip_(ip)
    {
    }
    ~tcp_client() {}
    void run()
    {
        // struct sockaddr_in *server_addr = init();

        sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd_ < 0)
        {
            lg(FATAL, "socket create error, sockfd : %d,%s", sockfd_, strerror(errno));
            exit(SOCK_ERROR);
        }
        lg(INFO, "socket create success, sockfd : %d", sockfd_);

        struct sockaddr_in *server_addr = new sockaddr_in;
        memset(server_addr, 0, sizeof(*server_addr));
        server_addr->sin_family = AF_INET;
        inet_pton(AF_INET, ip_.c_str(), &(server_addr->sin_addr));
        server_addr->sin_port = htons(port_);

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
            write(sockfd_, buffer.c_str(), buffer.size());

            char info[1024];
            memset(info, 0, sizeof(info));
            int n = read(sockfd_, info, sizeof(info) - 1);
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
        lg(INFO, "socket create success, sockfd : %d", sockfd_);

        struct sockaddr_in *addr = new sockaddr_in;
        memset(addr, 0, sizeof(*addr));
        addr->sin_family = AF_INET;
        inet_pton(AF_INET, ip_.c_str(), &(addr->sin_addr));
        addr->sin_port = htons(port_);

        return addr;
    }

private:
    int sockfd_;
    uint16_t port_;
    std::string ip_;
};