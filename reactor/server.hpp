#pragma once

#include <memory>
#include <errno.h>
#include <string>
#include <functional>
#include <fcntl.h>
#include <unordered_map>

#include "Log.hpp"
#include "socket.hpp"
#include "myepoll.hpp"

class connection;
class epoll_server;

using func_t = std::function<void(std::shared_ptr<connection>)>;

class connection // 每个文件都对应一个连接,拥有自己的输入输出缓冲区
{
    int fd_;
    std::string in_buffer_;
    std::string out_buffer_;

public:
    func_t read_cb_;
    func_t write_cb_;
    func_t except_cb_;

    // 方便日志打印
    std::string ip_;
    uint16_t port_;

    // 使用 weak_ptr 防止循环引用
    std::weak_ptr<epoll_server> p_svr_;

public:
    connection(int sockfd, func_t read_cb, func_t write_cb, func_t except_cb)
        : fd_(sockfd), read_cb_(read_cb), write_cb_(write_cb), except_cb_(except_cb) {}
    ~connection() {}
    void set_p_svr(std::weak_ptr<epoll_server> ptr)
    {
        p_svr_ = ptr;
    }
    int get_fd() { return fd_; }
    void in_append(const std::string &str) { in_buffer_ += str; }
    void out_append(const std::string &str) { out_buffer_ += str; }
    std::string &inbuffer() { return in_buffer_; }
    std::string &outbuffer() { return out_buffer_; }
};

class epoll_server : public std::enable_shared_from_this<epoll_server>, public no_copy
{
    static const int def_timeout = 1000;
    static const int def_num = 64;
    static const int def_buffsize = 128;
    static const uint32_t EVENT_IN = EPOLLIN | EPOLLET; // 设置ET模式
    static const uint32_t EVENT_OUT = EPOLLOUT | EPOLLET;

    int port_;
    func_t handle_;
    std::shared_ptr<MY_SOCKET> p_listen_sock_;
    std::shared_ptr<MY_EPOLL> p_epoll_;
    std::unordered_map<int, std::shared_ptr<connection>> connections_; // 建立fd->连接对象的映射关系
    struct epoll_event events_[def_num];

public:
    epoll_server(int port, func_t handle)
        : port_(port), handle_(handle), p_listen_sock_(new MY_SOCKET()), p_epoll_(new MY_EPOLL(def_timeout)) {}
    ~epoll_server() {}

    void add_sock(int fd, uint32_t event, func_t read_cb, func_t write_cb, func_t except_cb, const std::string &ip = "0.0.0.0", uint16_t port = 0)
    {
        std::shared_ptr<connection> new_connection(new connection(fd, read_cb, write_cb, except_cb));
        new_connection->set_p_svr(shared_from_this()); // shared_from_this(): 返回当前对象的shared_ptr,要确保epoll_server已经以shared_ptr的形式存在(主函数中以shared_ptr形式实例化对象)
        new_connection->ip_ = ip;
        new_connection->port_ = port;
        connections_.insert(std::make_pair(fd, new_connection));

        p_epoll_->ctl(EPOLL_CTL_ADD, fd, event);
    }

    void loop()
    {
        init();
        while (true)
        {
            int n = p_epoll_->wait(events_, def_num); // 等待并获取就绪事件
            for (int i = 0; i < n; ++i)
            {
                Dispatcher(events_[i]); // 每次处理一个就绪事件
            }
        }
    }

    void receiver(std::shared_ptr<connection> conn)
    {
        int fd = conn->get_fd();
        while (true) // 读取至底层无数据
        {
            char buffer[def_buffsize];
            memset(buffer, 0, sizeof(buffer));
            int n = read(fd, buffer, sizeof(buffer) - 1);
            if (n > 0) // 还没读完
            {
                conn->in_append(buffer);
            }
            else if (n == 0) // 对方关闭连接
            {
                lg(INFO, "sockfd: %d, client info %s:%d quit", fd, conn->ip_.c_str(), conn->port_);
                conn->except_cb_(conn); // 关注异常事件
                return;
            }
            else // 出错/读完
            {
                if (errno == EAGAIN) // 读完全部数据
                {
                    break;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else // 真的出错
                {
                    lg(WARNING, "sockfd: %d, client info %s:%d error", fd, conn->ip_.c_str(), conn->port_);
                    conn->except_cb_(conn); // 关注异常事件
                    return;
                }
            }
        }
        // 读完了数据,就该处理了,但不一定包含了一份完整报文
        handle_(conn);
    }

    void excepter(std::shared_ptr<connection> conn)
    {
        int fd = conn->get_fd();

        lg(WARNING, "Excepter hander sockfd: %d, client info %s:%d excepter handler", fd, conn->ip_.c_str(), conn->port_);
        p_epoll_->ctl(EPOLL_CTL_DEL, fd, 0);

        close(fd);
        lg(DEBUG, "close %d done\n", fd);

        connections_.erase(fd);
        lg(DEBUG, "remove %d from _connections done\n", fd);
    }
    void sender(std::shared_ptr<connection> conn)
    {

        auto &buffer = conn->outbuffer();
        int fd = conn->get_fd();

        while (true)
        {
            ssize_t n = write(fd, buffer.c_str(), buffer.size()); // 将输出缓冲区的内容写入内核
            if (n > 0)                                            // 写入一定数据
            {
                buffer.erase(0, n);
                if (buffer.empty()) // 数据写完了
                {
                    break;
                }
            }
            else if (n == 0) // 没有数据可写
            {
                return;
            }
            else
            {
                if (errno == EAGAIN)
                {
                    break;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    lg(WARNING, "sockfd: %d, client info %s:%d send error...", conn->get_fd(), conn->ip_.c_str(), conn->port_);
                    conn->except_cb_(conn);
                    return;
                }
            }
        }
        // 判断接下来是否需要关注写事件
        if (buffer.empty())
        {
            enable_event(fd, true, false);
        }
        else
        {
            enable_event(fd, true, true);
        }
    }

private:
    void init()
    {
        p_listen_sock_->Socket();
        set_no_block(p_listen_sock_->get_fd()); // 设置为非阻塞式
        p_listen_sock_->Bind(port_);
        p_listen_sock_->Listen();

        // 添加监听套接字
        add_sock(p_listen_sock_->get_fd(), EVENT_IN, std::bind(&epoll_server::accept, this, std::placeholders::_1), nullptr, nullptr);
        lg(DEBUG, "listen_socket add success\n");
    }
    void accept(std::shared_ptr<connection> conn) // 处理连接事件
    {
        while (true)
        {
            std::string clientip;
            uint16_t clientport;
            int sock = p_listen_sock_->Accept(clientip, clientport);
            if (sock > 0)
            {
                lg(DEBUG, "get a new client, get info-> [%s:%d], sockfd : %d", clientip.c_str(), clientport, sock);
                set_no_block(sock); // 设置为非阻塞式

                // 将新套接字添加进connections和epoll模型
                add_sock(sock, EVENT_IN,
                         std::bind(&epoll_server::receiver, this, std::placeholders::_1),
                         std::bind(&epoll_server::excepter, this, std::placeholders::_1),
                         std::bind(&epoll_server::excepter, this, std::placeholders::_1),
                         clientip, clientport);
            }
            else
            {
                if (errno == EAGAIN) // 无数据
                {
                    break;
                }
                else if (errno == EINTR) // 被信号中断
                {
                    continue;
                }
                else
                {
                    lg(ERROR, "accept error\n");
                    break;
                }
            }
        }
    }

    void Dispatcher(struct epoll_event &sock)
    {
        int fd = sock.data.fd; // 需要判断是否是我们关注的文件
        if (!is_safe(fd))
        {
            lg(DEBUG, "fd: %d is not safe\n", fd);
            return;
        }
        auto conn = connections_[fd];
        if (!conn)
            return;

        int event = sock.events;
        if ((event & EPOLLHUP) || (event & EPOLLERR)) // 异常事件转换为读写事件
        {
            event |= EPOLLIN;
            event |= EPOLLOUT;
        }

        if ((event & EPOLLIN) && conn->read_cb_) // 如果读回调存在
        {
            conn->read_cb_(conn); // 调用读回调
        }
        if ((event & EPOLLOUT) && conn->write_cb_) // 如果写回调存在
        {
            conn->write_cb_(conn); // 调用写回调
        }
    }

    bool is_safe(int fd)
    {
        return connections_.find(fd) != connections_.end(); // 是否在connections结构中存在
    }

    void set_no_block(int fd)
    {
        int ret = fcntl(fd, F_GETFL);
        if (ret < 0)
        {
            perror("fcntl");
            return;
        }
        fcntl(fd, F_SETFL, ret | O_NONBLOCK);
    }
    void enable_event(int fd, bool f_read, bool f_write)
    {
        if (fd < 0)
        {
            lg(ERROR, "Invalid file descriptor: %d", fd);
            return;
        }
        uint32_t event = 0;
        event |= ((f_read ? EPOLLIN : 0) | (f_write ? EPOLLOUT : 0) | EPOLLET);

        p_epoll_->ctl(EPOLL_CTL_MOD, fd, event);
    }
};

// #pragma once

// #include <memory>
// #include <errno.h>
// #include <string>
// #include <functional>
// #include <fcntl.h>
// #include <unordered_map>

// #include "Log.hpp"
// #include "socket.hpp"
// #include "myepoll.hpp"

// class connection;
// class epoll_server;

// using func_t = std::function<void(std::weak_ptr<connection>)>;

// class connection // 每个文件都对应一个连接,拥有自己的输入输出缓冲区
// {
//     int fd_;
//     std::string in_buffer_;
//     std::string out_buffer_;

// public:
//     func_t read_cb_;
//     func_t write_cb_;
//     func_t except_cb_;

//     // 方便日志打印
//     std::string ip_;
//     uint16_t port_;

//     // 使用 weak_ptr 防止循环引用
//     std::weak_ptr<epoll_server> p_svr_;

// public:
//     connection(int sockfd, func_t read_cb, func_t write_cb, func_t except_cb)
//         : fd_(sockfd), read_cb_(read_cb), write_cb_(write_cb), except_cb_(except_cb) {}
//     ~connection() {}
//     void set_p_svr(std::weak_ptr<epoll_server> ptr)
//     {
//         p_svr_ = ptr;
//     }
//     int get_fd() { return fd_; }
//     void in_append(const std::string &str) { in_buffer_ += str; }
//     void out_append(const std::string &str) { out_buffer_ += str; }
//     std::string &inbuffer() { return in_buffer_; }
//     std::string &outbuffer() { return out_buffer_; }
// };

// class epoll_server : public std::enable_shared_from_this<epoll_server>, public no_copy
// {
//     static const int def_timeout = 1000;
//     static const int def_num = 64;
//     static const int def_buffsize = 128;
//     static const uint32_t EVENT_IN = EPOLLIN | EPOLLET; // 设置ET模式
//     static const uint32_t EVENT_OUT = EPOLLOUT | EPOLLET;

//     int port_;
//     func_t handle_;
//     std::shared_ptr<MY_SOCKET> p_listen_sock_;
//     std::shared_ptr<MY_EPOLL> p_epoll_;
//     std::unordered_map<int, std::shared_ptr<connection>> connections_; // 建立fd->连接对象的映射关系
//     struct epoll_event events_[def_num];

// public:
//     epoll_server(int port, func_t handle)
//         : port_(port), handle_(handle), p_listen_sock_(new MY_SOCKET()), p_epoll_(new MY_EPOLL(def_timeout)) {}
//     ~epoll_server() {}

//     void add_sock(int fd, uint32_t event, func_t read_cb, func_t write_cb, func_t except_cb, const std::string &ip = "0.0.0.0", uint16_t port = 0)
//     {
//         std::shared_ptr<connection> new_connection(new connection(fd, read_cb, write_cb, except_cb));
//         new_connection->set_p_svr(shared_from_this()); // shared_from_this(): 返回当前对象的shared_ptr,要确保epoll_server已经以shared_ptr的形式存在(主函数中以shared_ptr形式实例化对象)
//         new_connection->ip_ = ip;
//         new_connection->port_ = port;
//         connections_.insert(std::make_pair(fd, new_connection));

//         p_epoll_->ctl(EPOLL_CTL_ADD, fd, event);
//     }

//     void loop()
//     {
//         init();
//         while (true)
//         {
//             //printf("loop\n");
//             int n = p_epoll_->wait(events_, def_num); // 等待并获取就绪事件
//             //printf("n:%d\n", n);
//             for (int i = 0; i < n; ++i)
//             {
//                 Dispatcher(events_[i]); // 每次处理一个就绪事件
//                 //printf("Dispatcher done\n");
//             }
//         }
//     }

//     void receiver(std::weak_ptr<connection> conne)
//     {
//         //f("receiver\n");
//         if (conne.expired())
//             return;
//         auto conn = conne.lock();
//         int fd = conn->get_fd();
//         while (true) // 读取至底层无数据
//         {
//             char buffer[def_buffsize];
//             memset(buffer, 0, sizeof(buffer));
//             int n = read(fd, buffer, sizeof(buffer) - 1);
//             if (n > 0) // 还没读完
//             {
//                 conn->in_append(buffer);
//                 //lg(DEBUG, "get data :%s\n", buffer);
//             }
//             else if (n == 0) // 对方关闭连接
//             {
//                 lg(INFO, "sockfd: %d, client info %s:%d quit", fd, conn->ip_.c_str(), conn->port_);
//                 conn->except_cb_(conn); // 关注异常事件
//                 return;
//             }
//             else // 出错/读完
//             {
//                 if (errno == EAGAIN) // 读完全部数据
//                 {
//                     break;
//                 }
//                 else if (errno == EINTR)
//                 {
//                     continue;
//                 }
//                 else // 真的出错
//                 {
//                     lg(WARNING, "sockfd: %d, client info %s:%d error", fd, conn->ip_.c_str(), conn->port_);
//                     conn->except_cb_(conn); // 关注异常事件
//                     return;
//                 }
//             }
//         }
//         // 读完了数据,就该处理了,但不一定包含了一份完整报文
//         handle_(conn);
//     }

//     void excepter(std::weak_ptr<connection> conne)
//     {
//         if (conne.expired())
//             return;
//         auto conn = conne.lock();

//         int fd = conn->get_fd();

//         lg(WARNING, "Excepter hander sockfd: %d, client info %s:%d excepter handler", fd, conn->ip_.c_str(), conn->port_);
//         p_epoll_->ctl(EPOLL_CTL_DEL, fd, 0);

//         close(fd);
//         lg(DEBUG, "close %d done\n", fd);

//         connections_.erase(fd);
//         lg(DEBUG, "remove %d from _connections done\n", fd);
//     }
//     void sender(std::weak_ptr<connection> conne)
//     {
//         if (conne.expired())
//             return;
//         auto conn = conne.lock();

//         auto &buffer = conn->outbuffer();
//         int fd = conn->get_fd();

//         while (true)
//         {
//             ssize_t n = write(fd, buffer.c_str(), buffer.size()); // 将输出缓冲区的内容写入内核
//             if (n > 0)                                            // 写入一定数据
//             {
//                 buffer.erase(0, n);
//                 if (buffer.empty()) // 数据写完了
//                 {
//                     break;
//                 }
//             }
//             else if (n == 0) // 没有数据可写
//             {
//                 return;
//             }
//             else
//             {
//                 if (errno == EAGAIN)
//                 {
//                     break;
//                 }
//                 else if (errno == EINTR)
//                 {
//                     continue;
//                 }
//                 else
//                 {
//                     lg(WARNING, "sockfd: %d, client info %s:%d send error...", conn->get_fd(), conn->ip_.c_str(), conn->port_);
//                     conn->except_cb_(conn);
//                     return;
//                 }
//             }
//         }
//         // 判断接下来是否需要关注写事件
//         if (buffer.empty())
//         {
//             enable_event(fd, true, false);
//         }
//         else
//         {
//             enable_event(fd, true, true);
//         }
//     }

// private:
//     void init()
//     {
//         p_listen_sock_->Socket();
//         set_no_block(p_listen_sock_->get_fd()); // 设置为非阻塞式
//         p_listen_sock_->Bind(port_);
//         p_listen_sock_->Listen();

//         // 添加监听套接字
//         add_sock(p_listen_sock_->get_fd(), EVENT_IN, std::bind(&epoll_server::accept, this, std::placeholders::_1), nullptr, nullptr);
//         lg(DEBUG, "listen_socket add success\n");
//     }
//     void accept(std::weak_ptr<connection> conne) // 处理连接事件
//     {
//         if (conne.expired())
//             return;
//         auto conn = conne.lock();
//         while (true)
//         {
//             std::string clientip;
//             uint16_t clientport;
//             int sock = p_listen_sock_->Accept(clientip, clientport);
//             if (sock > 0)
//             {
//                 lg(DEBUG, "get a new client, get info-> [%s:%d], sockfd : %d", clientip.c_str(), clientport, sock);
//                 set_no_block(sock); // 设置为非阻塞式

//                 // 将新套接字添加进connections和epoll模型
//                 add_sock(sock, EVENT_IN,
//                          std::bind(&epoll_server::receiver, this, std::placeholders::_1),
//                          std::bind(&epoll_server::excepter, this, std::placeholders::_1),
//                          std::bind(&epoll_server::excepter, this, std::placeholders::_1),
//                          clientip, clientport);
//             }
//             else
//             {
//                 if (errno == EAGAIN) // 无数据
//                 {
//                     break;
//                 }
//                 else if (errno == EINTR) // 被信号中断
//                 {
//                     continue;
//                 }
//                 else
//                 {
//                     lg(ERROR, "accept error\n");
//                     break;
//                 }
//             }
//         }
//     }

//     void Dispatcher(struct epoll_event &sock)
//     {
//         int fd = sock.data.fd; // 需要判断是否是我们关注的文件
//         if (!is_safe(fd))
//         {
//             lg(DEBUG, "fd: %d is not safe\n", fd);
//             return;
//         }
//         auto conn = connections_[fd];
//         if (!conn)
//             return;

//         int event = sock.events;
//         if ((event & EPOLLHUP) || (event & EPOLLERR)) // 异常事件转换为读写事件
//         {
//             event |= EPOLLIN;
//             event |= EPOLLOUT;
//         }

//         if ((event & EPOLLIN) && conn->read_cb_) // 如果读回调存在
//         {
//             //printf("fd: %d,read event\n", fd);
//             conn->read_cb_(conn); // 调用读回调
//         }
//         if ((event & EPOLLOUT) && conn->write_cb_) // 如果写回调存在
//         {
//             //printf("fd: %d,write event\n", fd);
//             conn->write_cb_(conn); // 调用写回调
//         }
//     }

//     bool is_safe(int fd)
//     {
//         return connections_.find(fd) != connections_.end(); // 是否在connections结构中存在
//     }

//     void set_no_block(int fd)
//     {
//         int ret = fcntl(fd, F_GETFL);
//         if (ret < 0)
//         {
//             perror("fcntl");
//             return;
//         }
//         fcntl(fd, F_SETFL, ret | O_NONBLOCK);
//     }
//     void enable_event(int fd, bool f_read, bool f_write)
//     {
//         if (fd < 0)
//         {
//             lg(ERROR, "Invalid file descriptor: %d", fd);
//             return;
//         }
//         uint32_t event = 0;
//         event |= ((f_read ? EPOLLIN : 0) | (f_write ? EPOLLOUT : 0) | EPOLLET);

//         p_epoll_->ctl(EPOLL_CTL_MOD, fd, event);
//     }
// };
