#include <iostream>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

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

using namespace std;
int main()
{
    cout << "enter:";
    fflush(stdout);
    set_no_block(0);
    char buff[100];
    while (true)
    {
        int ret = read(0, buff, sizeof(buff) - 1);
        if (ret > 0)
        {
            buff[ret - 1] = 0;
            cout << "echo: " << buff << endl;
        }
        else if (ret == 0)
        {
            cout << "end" << endl;
        }
        else
        {
            // printf("error,errno : %d,strerror: %s\n", errno, strerror(errno));
            if (errno == EWOULDBLOCK)
            {
                cout << "no data,try again" << endl;
            }
            else
            {
                cout << "read error" << endl;
            }
            sleep(1);
        }
    }

    return 0;
}