#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<stdlib.h>

int main()
{
  printf("im here,id:%d\n", getpid());
  int ret = fork();
  if (ret == 0)
  {
    printf("im child,id:%d,parent_id:%d\n", getpid(), getppid());
    sleep(1);
    exit(0);
  }
  else
  {
    printf("im parent,id:%d,parent_id:%d\n", getpid(), getppid());
    wait(NULL);
    printf("wait success\n");
  }

  return 0;
}
