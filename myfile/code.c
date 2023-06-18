#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<malloc.h>
#include<fcntl.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<assert.h>

#define num 1024

typedef struct myfile{
  int fd;
  char buffer[num];
  int end;
}myfile;

myfile* _fopen(const char* pathname,const char* option){
  assert(pathname);
  assert(option);
  myfile* fp=NULL;
  int fd=0;
  if(strcmp(option,"r")==0){
      fd=open(pathname,O_RDONLY,0666);
      if(fd>=0){
        fp=(myfile*)malloc(sizeof(myfile));
        memset(fp->buffer,0,sizeof(fp->buffer));
        fp->end=0;
        fp->fd=fd;
      }
  }
  if(strcmp(option,"w")==0){
      fd=open(pathname,O_WRONLY|O_TRUNC|O_CREAT,0666);
      if(fd>=0){
        fp=(myfile*)malloc(sizeof(myfile));
        memset(fp->buffer,0,sizeof(fp->buffer));
        fp->end=0;
        fp->fd=fd;
      }
  }
  if(strcmp(option,"a")==0||strcmp(option,"a+")==0){
      fd=open(pathname,O_WRONLY|O_APPEND|O_CREAT,0666);
      if(fd>=0){
        fp=(myfile*)malloc(sizeof(myfile));
        memset(fp->buffer,0,sizeof(fp->buffer));
        fp->end=0;
        fp->fd=fd;
      }
  }
  return fp;
}

void _fflush(myfile* fp){
  assert(fp);
  if(fp->end>0){
    write(fp->fd,fp->buffer,fp->end);
    syncfs(fp->fd);
    fp->end=0;
  }
}

void _fclose(myfile* fp){
  assert(fp);
  _fflush(fp);
  close(fp->fd);
  free(fp);
}

void _fputs(const char* str,myfile* fp){
  assert(fp);
  assert(str);
  strcpy(fp->buffer+fp->end,str);
  fp->end+=strlen(str);
  fprintf(stderr,"prev:%s ",fp->buffer);
 // printf("prev:%s ",fp->buffer);
  if(fp->fd==1){
    if(fp->buffer[fp->end-1]=='\n'){
      write(fp->fd,fp->buffer,fp->end);
      memset(fp->buffer,0,sizeof(fp->buffer));
      fp->end=0;
    }
    fprintf(stderr,"after:%s\n",fp->buffer);
  }
  //printf("after:%s\n",fp->buffer);
}
int main(){
  close(1);
  const char* arr1="hello ";
  const char* arr2="world\n";
  myfile* fp=_fopen("log.txt","w");
  if(fp==NULL){
    printf("_fopen error\n");
    return 1;
  }
  _fputs(arr1,fp);
  _fputs(arr2,fp);
  fork();
  _fclose(fp);
  //_fputs(arr1,fp);
  //_fputs(arr2,fp);
  //_fputs(arr1,fp);
  //_fputs(arr2,fp);
  //_fclose(fp);
  return 0;
}
