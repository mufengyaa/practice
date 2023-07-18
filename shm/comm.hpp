#ifndef COMM_H
#define COMM_H

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "log.hpp"
#include <assert.h>

using namespace std;

#define PATH_NAME "/home/mufeng"
#define PROJ_ID 0x2345
#define SUM_SIZE 4096

// 加入访问控制(通过管道来传递信号,接收到信号才进行读取)

#define FIFO_PATH "./fifo"
#define READ O_RDONLY
#define WRITE O_WRONLY | O_TRUNC

class Init
{
public:
    Init()
    {
        umask(0);
        int ret = mkfifo(FIFO_PATH, 0666);
        assert(ret == 0);
        (void)ret;
        log("fifo created success", notice) << endl;
    }
    ~Init()
    {
        unlink(FIFO_PATH);
        log("fifo removed success", notice) << endl;
    }
};

void wait_signal(int fd)
{
    uint32_t signal = 0;
    ssize_t size = read(fd, &signal, sizeof signal);
    assert(size == sizeof(uint32_t));
    (void)size;
    log("waiting ...", notice) << endl;
}
void send_signal(int fd)
{
    uint32_t signal = 1;
    ssize_t size = write(fd, &signal, sizeof signal);
    assert(size == sizeof(uint32_t));
    (void)size;
    log("being awakened ...", notice) << endl;
}
int open_fifo(string path, int flags)
{
    int fd = open(path.c_str(), flags);
    assert(fd >= 0);
    return fd;
}
void close_fifo(int fd)
{
    close(fd);
}

#endif
