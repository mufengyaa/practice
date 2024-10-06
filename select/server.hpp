#include "Log.hpp"
#include "socket.hpp"
#include <sys/select.h>

const int def_port = 8080;
const int def_max_num = (sizeof(fd_set) * 8); // fd_set最多能容纳的fd数量
const int def_data = -1;

class select_server
{
public:
    select_server() {}
    ~select_server() {}
    void start()
    {
        listen_socket_.Socket();
        listen_socket_.Bind(def_port);
        listen_socket_.Listen();

        // 初始化辅助数组
        for (int i = 0; i < def_max_num; ++i)
        {
            fds_[i] = -1;
        }

        int listen_fd = listen_socket_.get_fd();
        fds_[0] = listen_fd; // 固定第一项为监听套接字fd

        while (true)
        {
            int max_fd = -1;
            fd_set readfds;
            FD_ZERO(&readfds);
            for (int i = 0; i < def_max_num; ++i)
            {
                if (fds_[i] != -1)
                {
                    FD_SET(fds_[i], &readfds);
                }
                if (fds_[i] > max_fd)
                {
                    max_fd = fds_[i];
                }
            }
            // FD_SET(listen_fd, &readfds);
            struct timeval timeout = {1, 0}; // 1秒0微秒

            // int ret = select(listen_fd+1, &readfds, nullptr, nullptr, &timeout);
            int ret = select(max_fd + 1, &readfds, nullptr, nullptr, &timeout);
            if (ret > 0) // 有事件就绪
            {
                handle(&readfds);
            }
            else if (ret == 0) // 超时
            {
                continue;
            }
            else
            {
                perror("select");
                break;
            }
        }
        listen_socket_.Close();
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
            fds_[i] = -1; // 重置数组内的值
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
        else // 把新fd加入位图
        {
            int pos = 1;
            for (; pos < def_max_num; ++pos)
            {
                if (fds_[pos] == def_data) // 找到空位,但不能直接添加
                {
                    break;
                }
            }
            if (pos != def_max_num)
            {
                fds_[pos] = sock;
            }
            else // 满了
            {
                lg(WARNING, "server is full,close %d now", sock);
                close(sock);
            }
        }
    }
    void handle(fd_set *p_readfds)
    {
        for (int i = 0; i < def_max_num; ++i) // 遍历数组
        {
            int fd = fds_[i];
            if (fd != def_data) // 有效fd
            {
                if (FD_ISSET(fd, p_readfds)) // 有事件就绪
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
    int fds_[def_max_num]; // 辅助数组
};