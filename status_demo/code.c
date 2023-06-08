#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(){
  pid_t ret=fork();
  int status=0;
  if(ret==0){
    printf("im child,pid:%d,ppid:%d\n",getpid(),getppid());
  //int* p=NULL;
  //*p=1;
  int a=5;
    while(a){
      printf("im child,pid:%d,ppid:%d\n",getpid(),getppid());
      sleep(1);
      a--;
    }
  exit(13);
  }
  else{
    int quit=0;
    while(!quit){
      pid_t resul= waitpid(-1,&status,WNOHANG);
      if(resul==0){
        printf("i need wait\n");
        printf("im working\n");
      }
      else if(resul>0){
        printf("im success,status:%d\n",WEXITSTATUS(status));
        quit=1;
      }
      else{
        printf("im fail\n");
        quit=1;
      }
      sleep(1);
    }
  
  }
  return 0;
}
