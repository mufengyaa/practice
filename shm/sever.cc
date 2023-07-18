#include "comm.hpp"

Init init; // 创建管道文件

int main()
{
    key_t key = ftok(PATH_NAME, PROJ_ID);
    assert(key != -1);
    (void)key;
    log("key created success", debug) << ", key : " << key << endl;

    int shmid = shmget(key, SUM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    assert(shmid != -1);
    (void)shmid;
    log("shm created success", debug) << ", shmid : " << shmid << endl;
    // sleep(3);

    char *addres = (char *)shmat(shmid, nullptr, 0);
    assert(addres != nullptr);
    (void)addres;
    log("process linked success", debug) << endl;
    // sleep(3);

    // 通信
    // while (true)
    // {
    //     if (strcmp(addres, "quit") == 0)
    //     {
    //         break;
    //     }
    //     cout << addres << endl;
    //     sleep(2);
    // }
    // 添加访问控制
    int fd = open_fifo(FIFO_PATH, READ);
    while (true)
    {
        wait_signal(fd); // 等待唤醒
        if (strcmp(addres, "quit") == 0)
        {
            break;
        }
        cout << addres << endl;
    }
    close_fifo(fd); // 通信结束

    int ret = shmdt(addres);
    assert(ret != -1);
    (void)ret;
    log("process unlinked success", debug) << endl;
    // sleep(3);

    ret = shmctl(shmid, IPC_RMID, nullptr);
    assert(ret != -1);
    (void)ret;
    log("shm unlinked success", debug) << endl;

    return 0;
}