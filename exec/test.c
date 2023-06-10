#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
   printf("hello,im test\n");
   printf("env:%s\n",getenv("val"));
   return 0;
}
