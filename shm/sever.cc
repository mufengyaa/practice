#include "comm.hpp"

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
    log("process link success", debug) << endl;
    // sleep(3);

    // 通信
    while (true)
    {
        if (strcmp(addres, "quit") == 0)
        {
            break;
        }
        cout << addres << endl;
        sleep(2);
    }

    int ret = shmdt(addres);
    assert(ret != -1);
    (void)ret;
    log("process unlink success", debug) << endl;
    // sleep(3);

    ret = shmctl(shmid, IPC_RMID, nullptr);
    assert(ret != -1);
    (void)ret;
    log("shm unlink success", debug) << endl;

    return 0;
}