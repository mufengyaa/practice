#include <iostream>
#include <string>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>

#include <pthread.h>
#include <sys/wait.h>

#include "Log.hpp"
#include "helper.hpp"

const int backlog = 5;
const int buff_size = 1024;

class tcp_server
{
public:
    tcp_server(const uint16_t port = 8080, const std::string ip = "0.0.0.0")
        : ip_(ip), port_(port), listen_sockfd_(-1)
    {
    }
    void run()
    {
        init();

        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        memset(&client_addr, 0, client_len);
        lg(INFO, "init success");
        while (true)
        {
            int sockfd = accept(listen_sockfd_, reinterpret_cast<struct sockaddr *>(&client_addr), &client_len);
            if (sockfd < 0)
            {
                continue;
            }
            char client_ip[32];
            inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, sizeof(client_ip));
            int client_port = ntohs(client_addr.sin_port);

            lg(INFO, "get a new link..., sockfd: %d, client ip: %s, client port: %d", sockfd, client_ip, client_port);

            // 单进程版
            // echo(sockfd, client_ip, client_port);
            // close(sockfd);

            // 多进程版
            int ret = fork();
            if (ret == 0)
            {
                close(listen_sockfd_);
                int t = fork();
                if (t == 0)
                {
                    echo(sockfd, client_ip, client_port);
                }
                exit(0);
            }
            close(sockfd);
            int r = waitpid(ret, nullptr, 0);
            (void)r;
        }
    }
    ~tcp_server() {}

private:
    void init()
    {
        listen_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_sockfd_ < 0)
        {
            lg(FATAL, "socket create error, sockfd : %d,%s", listen_sockfd_, strerror(errno));
            exit(SOCK_ERROR);
        }
        lg(INFO, "socket create success, sockfd : %d", listen_sockfd_);

        struct sockaddr_in *addr = new sockaddr_in;
        memset(addr, 0, sizeof(*addr));
        addr->sin_family = AF_INET;
        inet_pton(AF_INET, ip_.c_str(), &(addr->sin_addr));
        addr->sin_port = htons(port_);

        int t = bind(listen_sockfd_, reinterpret_cast<struct sockaddr *>(addr), sizeof(*addr));
        if (t < 0)
        {
            lg(FATAL, "bind error, sockfd : %d,%s", listen_sockfd_, strerror(errno));
            exit(BIND_ERROR);
        }
        lg(INFO, "bind success, sockfd : %d", listen_sockfd_);

        if (listen(listen_sockfd_, backlog) < 0)
        {
            lg(FATAL, "listen error, sockfd : %d,%s", listen_sockfd_, strerror(errno));
            exit(LISTEN_ERROR);
        }
        lg(INFO, "listen success, sockfd : %d", listen_sockfd_);
        delete addr;
    }
    void echo(int fd, const char *ip, const uint16_t port)
    {
        char buffer[buff_size];
        memset(buffer, 0, sizeof(buffer));

        while (true)
        {
            int n = read(fd, buffer, sizeof(buffer) - 1);
            if (n < 0)
            {
                lg(ERROR, "%s:%d read error, %s", ip, port, strerror(errno));
                break;
            }
            else if (n == 0)
            {
                lg(INFO, "%s:%d quit", ip, port);
                break;
            }
            else
            {
                buffer[n] = 0;
                std::string res = process_info(buffer, ip, port);
                write(fd, res.c_str(), res.size());
            }
        }
    }

    std::string process_info(const std::string &info, const std::string ip, const uint16_t port)
    {
        std::string time_stamp = get_time();
        std::string id = generate_id(ip, port);

        std::string res = id + time_stamp + info;
        return res;
    }

private:
    int listen_sockfd_;
    uint16_t port_;
    std::string ip_;
};