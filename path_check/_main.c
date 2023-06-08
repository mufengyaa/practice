#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(int argc,char* argv[],char* env[]){
  for(int i=0;env[i];i++){
    printf("%s\n",env[i]);
  }
  printf("%s\n",getenv("_main"));
  return 0;
}
