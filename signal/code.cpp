#include <iostream>
#include <unistd.h>
#include <signal.h>
#include<stdlib.h>
#include<vector>
#include<sys/wait.h>
#include<sys/types.h>
#include<functional>
using namespace std;

int count_num=0;

void func(int signum)
{
  cout << "im " << getpid() << "i got a signal : " << signum << endl;
 // cout<<count_num<<endl;
  //exit(1);
}
void test1()
{
  signal(2, func);
  while (1)
  {
    cout << "im here : " << getpid() << endl;
    sleep(1);
  }
}
void test2(){
  signal(SIGFPE,func);
  int a=1;
  a/=0;
  while(1){
   sleep(1);
  }
}
void test3(){
  alarm(1);
  int count=0;
  while(1){
    cout<<count<<endl;
    ++count;
  }
}
void test4(){
  alarm(1);
  signal(SIGALRM,func);
  while(1){
    ++count_num;
  }
}


vector<function<void()>> callbacks;
void log(){
  cout<<"loging"<<endl;
}
void show_user(){
  if(fork()==0){
    execl("/usr/bin/who","who",nullptr);
    exit(1);
  }
  wait(nullptr);
}
void func_test(int signum){
  signum=0;
  for(auto& it:callbacks){
    it();
  }
  alarm(1);
}
void test5(){
  callbacks.push_back(log);
  callbacks.push_back(show_user);
  alarm(1);
  signal(SIGALRM,func_test);
  while(1){
    ;
  }
}
int main()
{
  test2();
  return 0;
}
