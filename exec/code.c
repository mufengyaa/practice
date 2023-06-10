#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main(){
  pid_t ret=fork();
  if(ret==0){
    printf("im child\n");
    sleep(1);
    char* const order[]={
      (char*)"ls",(char*)"-l",NULL
    };
    char* const _env[]={
      (char*)"val=2023",NULL
    };
    execl("/usr/bin/ls","ls","-l","-a",NULL);
//  execv("/usr/bin/ls",order);
//  execlp("ls","ls","-l",NULL);
   // execvp("ls",order);
  //  execle("/usr/bin/ls","ls","-l",_env);
    //execle("/home/mufeng/c++/exec/test","test",NULL,_env);
    //execle("/home/mufeng/c++/exec/test","test",NULL,_env);
  //  execlp("python","python","test.py",NULL);
  //  exit(11);
  }
  else{
    printf("im parent\n");
    int status=0;
    pid_t res=waitpid(-1,&status,0);
    if(res>0){
      printf("success,status:%d\n",WEXITSTATUS(status));
    }
    else{
      printf("fail\n");
    }
  }
 // printf("beginning\n");
 // execl("/usr/bin/ls","ls","-l","--color=auto",NULL);
 // exit(222);
 // printf("end\n");
  return 0;
}
