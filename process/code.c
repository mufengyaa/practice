#include<stdio.h>
#include<unistd.h>

void test1(){
  while(1){
    printf("%d\n",getpid());
    sleep(1);
  }
}
int main(){
  test1();
  return 0;
}
