#ifndef COMM_H
#define COMM_H

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <fcntl.h>
#include<sys/wait.h>
#include<unistd.h>

using namespace std;

string pipe_path = "./fifo.ipc";
#define mode 0666
#define num 1024
#define process_size 3

#endif
