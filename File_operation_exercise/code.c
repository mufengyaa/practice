#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>


int main(int argc,char* argv[]){
  printf("hello\n");
  fprintf(stdout,"world\n");
  write(1,"!\n",2);
  fork();
  return 0;
  //if(argc!=2){
  //  return 2;
  //}
  //int fd=open("log.txt",O_WRONLY|O_TRUNC|O_CREAT,0666);
  //int fd=open("log.txt",O_WRONLY|O_APPEND|O_CREAT,0666);
 // int fd=open("log.txt",O_RDONLY);
 // if(fd<0){
 //   return 1;
 // };
 // dup2(fd,stdin->_fileno);
 // char arr[64];
 // //fgets(arr,64,stdin);
 // scanf("%s",arr);
 // printf("%s",arr);
 // return 0;

  ////simulation
  //close(1);
  ////int fd=open("log.txt",O_WRONLY|O_TRUNC|O_CREAT,0666);
  //int fd=open("log.txt",O_WRONLY|O_APPEND|O_CREAT,0666);
  ////fprintf(stdout,"fd:%d\n",fd);
  //printf("fd:%d\n",fd);
  //return 0;


 // FILE* fp=fopen("log.txt","w");
  //if(fp==NULL){
  //  perror("fopen");
  //}
  
  //int fd= open("log.txt",O_WRONLY|O_CREAT);
  
  // int fd= open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
  //int fd= open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
  //int fd= open("log.txt",O_RDONLY);
  //if(fd<0){
  // perror("open");
  // return 1;
  //}
  //else{
  //  printf("fd:%d\n",fd);
  //}
 
 // char arr[64];
 // memset(arr,'\0',sizeof arr);
 // read(fd,arr,sizeof(arr));
 // printf("log.txt:%s\n",arr);


 // while(1){
 //   sleep(1);
 // }
  //int fd=open("log.txt",O_WRONLY);
  //printf("fd:%d\n",fd);
  //const char* arr="ww";
  //ssize_t size=write(fd,arr,strlen(arr));
  //printf("size:%zd\n",size);
  //close(fd);
  //return 0;
}

