#include"add.h"

int add(int a,int b){
  int sum=0;
  for(int i=a;i<=b;i++){
    sum+=i;
  }
  return sum;
}
