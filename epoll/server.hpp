#include "Log.hpp"
#include "socket.hpp"
#include "myepoll.hpp"
#include <memory>

static const int def_port = 8080;
static const int def_timeout = 1000;
static const int def_size = 64;

class epoll_server
{
public:
    epoll_server()
        : p_listen_socket_(new MY_SOCKET), p_epoll_(new MY_EPOLL(def_timeout)) {}
    ~epoll_server()
    {
    }
    void start()
    {
        init();
        // 添加监听套接字
        int fd = p_listen_socket_->get_fd();
        p_epoll_->ctl(EPOLL_CTL_ADD, fd, EPOLLIN);

        while (true)
        {
            struct epoll_event events[def_size];
            int ret = p_epoll_->wait(events, def_size);
            if (ret > 0) // 有事件就绪
            {
                handle(ret, events);
            }
            else if (ret == 0) // 超时
            {
                continue;
            }
            else
            {
                perror("epoll_wait");
                break;
            }
        }
    }

private:
    void init()
    {
        p_listen_socket_->Socket();
        p_listen_socket_->Bind(def_port);
        p_listen_socket_->Listen();
    }
    void handle(int n, struct epoll_event *events)
    {
        int listen_sock_ = p_listen_socket_->get_fd();
        for (int i = 0; i < n; ++i) // 遍历有效事件
        {
            int fd = events[i].data.fd;     // 这里我们就可以知道是哪个文件上的事件就绪了
            if (events[i].events & EPOLLIN) // 有读事件就绪
            {
                if (fd == listen_sock_) // 获取新连接
                {
                    accepter(fd);
                }
                else // 读事件
                {
                    receiver(fd);
                }
            }
        }
    }
    void receiver(int fd)
    {
        char in_buff[1024];
        int n = read(fd, in_buff, sizeof(in_buff) - 1);
        if (n > 0)
        {
            in_buff[n - 1] = 0;
            std::cout << "get message: " << in_buff << std::endl;
        }
        else if (n == 0) // 客户端关闭连接
        {
            lg(INFO, "%d quit", fd);
            p_epoll_->ctl(EPOLL_CTL_DEL, fd, 0);
            close(fd);
        }
        else
        {
            lg(ERROR, "fd: %d ,read error");
            p_epoll_->ctl(EPOLL_CTL_DEL, fd, 0);
            close(fd);
        }
    }
    void accepter(int fd)
    {
        std::string clientip;
        uint16_t clientport;
        int sock = p_listen_socket_->Accept(clientip, clientport);
        if (sock == -1)
        {
            return;
        }
        else // 把新文件上的读事件加入红黑树
        {
            p_epoll_->ctl(EPOLL_CTL_ADD, sock, EPOLLIN);
            lg(INFO, "get a new link, client info@ %s:%d", clientip.c_str(), clientport);
        }
    }

private:
    std::shared_ptr<MY_SOCKET> p_listen_socket_;
    std::shared_ptr<MY_EPOLL> p_epoll_;
};