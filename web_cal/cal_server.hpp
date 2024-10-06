#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <functional>
#include <memory>

#include "cal.hpp"
#include "socket.hpp"

static MY_SOCKET my_socket;
// static calculate Cal;

bool exit_ = false;
using cal_t = std::function<std::string(std::string &arr)>;

// 网络服务端

class server : public std::enable_shared_from_this<server>
{
public:
    server(const uint16_t port, const std::string &ip, cal_t callback)
        : port_(port), ip_(ip), callback_(callback)
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
            lg(DEBUG, "accepting ...");
            int sockfd = my_socket.Accept(client_ip, client_port);
            if (sockfd == -1)
            {
                continue;
            }
            lg(INFO, "get a new link..., sockfd: %d, client ip: %s, client port: %d", sockfd, client_ip.c_str(), client_port);

            int ret = fork();
            if (ret == 0)
            {
                my_socket.Close();
                char buffer[buff_size];
                std::string in_buffer;

                while (!exit_)
                {
                    memset(buffer, 0, sizeof(buffer));
                    int n = read(sockfd, buffer, sizeof(buffer)); //"size"\n"a op b"\n
                    if (n > 0)
                    {
                        buffer[n] = 0;
                        lg(INFO, "get request : %s", buffer);
                        in_buffer += buffer; // 连续读取

                        while (true) // 处理多份数据
                        {
                            std::string content = callback_(in_buffer); //->"size"\n"result code"\n
                            // std::cout << "content: " << content << std::endl;
                            if (content.empty())
                            {
                                break;
                            }

                            write(sockfd, content.c_str(), content.size());
                        }
                    }
                    else if (n == 0)
                    {
                        lg(INFO, "%s quit", client_ip.c_str());
                        break;
                    }
                    else // 读出错误
                    {
                        break;
                    }
                }
                // lg(INFO, "fork quit");
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
        daemon(1, 0);

        my_socket.Socket();
        my_socket.Bind(port_);
        my_socket.Listen();
        lg(INFO, "server init done");
    }

public:
    uint16_t port_;
    std::string ip_;
    cal_t callback_;
};