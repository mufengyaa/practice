#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
  printf("im here,id:%d\n",getpid());
  int ret=fork();
  if(ret==0){
    printf("im child,id:%d,parent_id:%d\n",getpid(),getppid());
  }
  else{
    printf("im parent,id:%d,parent_id:%d\n",getpid(),getppid());
  }
  return 0;
}
