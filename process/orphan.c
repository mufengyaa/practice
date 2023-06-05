#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
  size_t ret=fork();
  if(ret==0){
    sleep(30);
  }else{
    sleep(3);
    exit(0);
  }
  return 0;
}

