#include<stdio.h>
#include<string.h>
#include<unistd.h>

#define num 100

int main(){
  char buf[num+1]={0};
  const char* symbol="|/-\\";
  int i=1;
  buf[0]='<';
  while(i<=num){
    printf("[%-100s][%d%%][%c]\r",buf,i,symbol[i%4]);
    fflush(stdout);
    if(i==num-1){
      buf[i]='>';
    }
    else{
      buf[i]='=';
    }
    i++;
    usleep(50000);
  }
  printf("\n");
  return 0;
}
