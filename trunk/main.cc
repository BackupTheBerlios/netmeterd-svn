//$Id$

#include <iostream>
#include "counter.h"
#include "iface.h"

using namespace std;

int main(int arg, char **argv){
  
  iface a(argv[1]);
  while (true)
  {
    sleep(1);
    cout << a << endl;
    a.update();
  }
  return 0;
  
}
