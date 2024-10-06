#pragma once

#include <sys/epoll.h>
#include <cstring>
#include <errno.h>
#include "Log.hpp"
#include"nocopy.hpp"

static const int def_epoll_size = 128;


// 我们要保证这个对象唯一,因为没有必要存在多个[自定义操作epoll模型的方法],可以直接禁止拷贝/赋值,也可以继承一个无法拷贝/赋值的类
class MY_EPOLL : public no_copy
{
public:
    MY_EPOLL(int timeout)
    {
        sock_ = epoll_create(def_epoll_size);
        if (sock_ < 0)
        {
            lg(ERROR, "epoll_create error,%s\n", strerror(errno));
        }
        else
        {
            lg(DEBUG, "epoll_create success,fd: %d\n", sock_);
        }
        timeout_ = timeout;
    }
    ~MY_EPOLL()
    {
        if (sock_ >= 0)
        { // 保证在fd有效的情况下,关闭
            close(sock_);
        }
    }
    int wait(struct epoll_event *events, int max)
    {
        return epoll_wait(sock_, events, max, timeout_);
    }
    int ctl(int op, int fd, uint32_t event) // 这里我们设置成直接传入事件名,然后在内部构建epoll_event结构
    {
        int n = 0;
        if (op == EPOLL_CTL_DEL)
        {
            n = epoll_ctl(sock_, op, fd, nullptr); // 删除后,无论什么事件都无所谓了
            if (n == -1)
            {
                lg(ERROR, "EPOLL_CTL_DEL error\n");
            }
        }
        else
        {
            struct epoll_event t;
            t.events = event;
            t.data.fd = fd; // 方便用户层拿到时,可以知道是哪个fd就绪
            n = epoll_ctl(sock_, op, fd, &t);
            if (n == -1)
            {
                lg(ERROR, "EPOLL_CTL_ADD/MOD error,%s\n", strerror(errno));
            }
        }
        return n;
    }

private:
    int timeout_;
    int sock_;
};