#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const std::string null_path = "/dev/null";

void daemon(const std::string &path = "")
{
    // 忽略信号
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    // 创建子进程
    if (fork() > 0)
    {
        exit(0);
    }
    // 子进程成为守护进程
    setsid();

    // 修改工作目录
    if (!path.empty())
    {
        chdir(path.c_str());
    }

    // 重定向
    int fd = open(null_path.c_str(), O_RDWR);
    if (fd > 0)
    {
        dup2(fd, 0);
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);
    }
}