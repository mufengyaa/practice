#include "Log.hpp"
#include "socket.hpp"
#include <poll.h>

static const int def_port = 8080;
static const int def_max_num = 1024;
static const int def_data = -1;
static const int no_data = 0;

class poll_server
{
public:
    poll_server()
    {
        for (int i = 0; i < def_max_num; ++i)
        {
            fds_[i].fd = def_data;
            fds_[i].events = no_data;
            fds_[i].revents = no_data;
        }
    }
    ~poll_server()
    {
        listen_socket_.Close();
    }
    void start()
    {
        listen_socket_.Socket();
        listen_socket_.Bind(def_port);
        listen_socket_.Listen();
        int timeout = 1;

        // 固定数组第一项是监听套接字
        struct pollfd tl = {listen_socket_.get_fd(), POLLIN, no_data};
        fds_[0] = tl;

        while (true)
        {
            int ret = poll(fds_, def_max_num, timeout);
            if (ret > 0) // 有事件就绪
            {
                handle();
            }
            else if (ret == 0) // 超时
            {
                continue;
            }
            else
            {
                perror("poll");
                break;
            }
        }
    }

private:
    void receiver(int fd, int i)
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
            close(fd);
            lg(DEBUG, "%d quit", fd);
            fds_[i].fd = -1; // 重置该位置
            fds_[i].events = no_data;
            fds_[i].revents = no_data;
        }
        else
        {
            lg(ERROR, "fd: %d ,read error");
        }
    }
    void accepter()
    {
        std::string clientip;
        uint16_t clientport;
        int sock = listen_socket_.Accept(clientip, clientport);
        if (sock == -1)
        {
            return;
        }
        else // 把新fd加入数组
        {
            struct pollfd t;
            int pos = 1000; //1s
            for (; pos < def_max_num; ++pos)
            {
                if (fds_[pos].fd == def_data) // 找到空位,但不能直接添加
                {
                    break;
                }
            }
            if (pos != def_max_num)
            {
                t.fd = sock;
            }
            else // 满了
            {
                //这里可以扩容
                lg(WARNING, "server is full,close %d now", sock);
                close(sock);
            }
            t.events = POLLIN;
            t.revents = no_data;
            fds_[pos] = t;
        }
    }
    void handle()
    {
        for (int i = 0; i < def_max_num; ++i) // 遍历数组
        {
            int fd = fds_[i].fd;
            if (fd != def_data) // 有效fd
            {
                if (fds_[i].revents & POLLIN) // 有事件就绪
                {
                    if (fd == listen_socket_.get_fd()) // 获取新连接
                    {
                        accepter();
                    }
                    else // 读事件
                    {
                        receiver(fd, i);
                    }
                }
            }
        }
    }

private:
    MY_SOCKET listen_socket_;
    struct pollfd fds_[def_max_num];
};