#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
  printf("val:%s\n",getenv("val"));
  return 0;
}
