#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>

#define num 1024
#define size 32
#define SEP " "

char cmd[num];
char* options[size];

int main(){
  extern char** environ;
  while(1){
    printf("root@localhost myshell#");
    fflush(stdout);
    memset(cmd,'\0',sizeof cmd);
    if(fgets(cmd,sizeof cmd,stdin)==NULL){
      continue;
    }//ctrl+z to quit
    cmd[strlen(cmd)-1]='\0';
    options[0]=strtok(cmd,SEP);
    int i=1;
      if(strcmp(options[0],"ls")==0){
        options[i++]="--color=auto";
      }
    while(1){
      options[i++]=strtok(NULL,SEP);
      if(options[i-1]==NULL){
        break;
      }
    }
    if(strcmp(options[0],"export")==0){
      char* my_env=(char*)malloc(sizeof(char)*sizeof(options[1])); 
      strcpy(my_env,options[1]);
      putenv(my_env);
      continue;
    }
   // for(i=0;options[i];i++){
   //   printf("%s\n",options[i]);
   // }
   
    if(strcmp(options[0],"cd")==0){
      if(options[1]!=NULL){
        chdir(options[1]);  //change work_path
      }
      continue;
    }
    pid_t id=fork();
    if(id==0){
      printf("im child\n");
      execvpe(options[0],options,environ);
      exit(1);
    }
    else{
      int status=0;
      pid_t ret=waitpid(-1,&status,0);
      if(ret>0){  //success
        printf("%s:%d\n",options[0],WEXITSTATUS(status));
      }
      else{   //fail
        printf("fail\n");
      }
    }
  }
  return 0;
}
