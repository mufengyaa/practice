#include<stdio.h>
#include<unistd.h>

int main(int argc,char* argv[],char* env[]){
  for(int i=0;env[i];i++){
    printf("%s\n",env[i]);
  }
  return 0;
}
