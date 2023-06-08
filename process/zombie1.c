#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(){
  pid_t ret=fork();
  if(ret<0){
    perror("fork");
    exit(1);
  }
  else if(ret==0){
    printf("im child,pid:%d,ppid:%d\n",getpid(),getppid());
    sleep(5);
    exit(2023);
  }
  else{
    printf("im parent,pid:%d,ppid:%d\n",getpid(),getppid());
    //sleep(7);
    //pid_t res=wait(NULL);
    int status=0;
    pid_t res=waitpid(ret,&status,0);
    if(res>0){
      printf("success,status:%d\n",status);
    }
   // while(1){
   //   printf("im parent,pid:%d,ppid:%d\n",getpid(),getppid());
   //   sleep(1);
   // }
  }
}

