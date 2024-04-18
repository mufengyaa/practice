#include <signal.h>
#include <cstring>

#include "cal.hpp"
#include "socket.hpp"

static MY_SOCKET my_socket;
static calculate Cal;
#define buff_size 256

// 网络服务端

class server
{
public:
    server(const uint16_t port = 8080, const std::string &ip = "0.0.0.0")
        : port_(port), ip_(ip)
    {
    }
    ~server()
    {
    }
    void run()
    {
        init();
        while (true)
        {
            uint16_t client_port;
            std::string client_ip;
            int sockfd = my_socket.Accept(client_ip, client_port);
            lg(INFO, "get a new link..., sockfd: %d, client ip: %s, client port: %d", sockfd, client_ip.c_str(), client_port);

            int ret = fork();
            if (ret > 0)
            {
                my_socket.Close();
                char buffer[buff_size];
                memset(buffer, 0, sizeof(buffer));

                while (true)
                {
                    int n = read(sockfd, buffer, sizeof(buffer));
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        std::string content = encode(buffer);
                        std::cout << content.c_str() << std::endl;

                        std::string res = Cal.cal(content);
                        int n = write(sockfd, res.c_str(), res.size());
                    }
                    else if (n == 0)
                    {
                        break;
                    }
                    else // 读出错误
                    {
                        break;
                    }
                }
                exit(0);
                close(sockfd);
            }
        }
    }

private:
    void init()
    {
        signal(SIGPIPE, SIG_IGN);
        signal(SIGCHLD, SIG_IGN);
        my_socket.Socket();
        my_socket.Bind(port_);
        my_socket.Listen();
        lg(INFO, "server init done");
    }

public:
    uint16_t port_;
    std::string ip_;
};