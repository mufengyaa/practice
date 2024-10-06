#include "comm.hpp"

int main()
{
    key_t key = ftok(PATH_NAME, PROJ_ID);
    if (key < 0)
    {
        log("key created fail", error) << ", client key : " << key << endl;
    }

    int shmid = shmget(key, SUM_SIZE, 0);
    if (shmid < 0)
    {
        log("shmid created fail", error) << ", client shmid : " << shmid << endl;
    }

    char *addres = (char *)shmat(shmid, nullptr, 0);
    if (addres == nullptr)
    {
        log("process linked fail", error) << endl;
    }
    // sleep(3);

    // 通信
    while (true)
    {
        ssize_t size = read(0, addres, SUM_SIZE - 1);
        if (size > 0)
        {
            addres[size - 1] = 0;
            if (strcmp(addres, "quit") == 0)
            {
                break;
            }
        }
    }
    // 添加访问控制
    // int fd = open_fifo(FIFO_PATH, WRITE);
    // while (true)
    // {
    //     ssize_t size = read(0, addres, SUM_SIZE - 1);
    //     if (size > 0)
    //     {
    //         addres[size - 1] = 0;
    //         send_signal(fd);
    //         if (strcmp(addres, "quit") == 0)
    //         {
    //             break;
    //         }
    //     }
    // }

    int ret = shmdt(addres);
    if (ret < 0)
    {
        log("process unlinked fail", error) << endl;
    }
    // sleep(3);

    return 0;
}