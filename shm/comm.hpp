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

#endif
