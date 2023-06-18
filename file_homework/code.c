#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main(){
  FILE* fp=fopen("bite","wb+");
  const char* arr="linux so easy!\n";
  fwrite(arr,strlen(arr),1,fp);
  fflush(fp);
  fseek(fp,0,SEEK_SET);
  char ret[1024];
  memset(ret,0,sizeof(ret));
  fread(ret,strlen(arr),1,fp);
  printf("%s",ret);
  fclose(fp);
  return 0;
  //int fd=open("bite",O_RDWR|O_CREAT|O_TRUNC,0666);
  //const char* arr="i like linux!\n";
  //write(fd,arr,strlen(arr));
  //lseek(fd,0,SEEK_SET);
  //char ret[1024];
  //memset(ret,0,sizeof(ret));
  //read(fd,ret,sizeof(ret));
  //printf("%s",ret);
  //close(fd);
  return 0;
}

