//$Id$

#include "counter.h"

int main(){
  
  counter a(1,2,1,2);
  counter b;
  b=a;
  a+=a;
  a.print();
  b.print();
  return 0;
}
