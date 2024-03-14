#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

std::string terminal = "/dev/pts/2";

void my_dup()
{
    int fd = open(terminal.c_str(), O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }
    dup2(fd, 2);
    close(fd);
}