#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<iostream>
#include<errno.h>

int main()
{
  fprintf(stdout,"hello fprintf\n");
  errno=1;
  perror("hello perror");
  const char* s1="hello write 1\n";
  const char* s2="hello write 2\n";
  write(1,s1,strlen(s1));
  write(2,s2,strlen(s2));
  std::cout<<"hello cout"<<std::endl;
  std::cerr<<"hello cerr"<<std::endl;
  return 0;
}
