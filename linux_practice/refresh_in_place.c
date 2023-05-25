#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main(){
  int count=5;
  while(count--){
    printf("%d\r",count);
    fflush(stdout);
    sleep(1);
  }
  return 0;
}
