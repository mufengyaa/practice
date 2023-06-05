#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
  size_t ret=fork();
  if(ret==0){
    sleep(4);
    exit(0);
  }else{
    sleep(30);
  }
  return 0;
}
