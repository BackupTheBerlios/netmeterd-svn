//$Id$

#include <iostream>
#include "counter.h"
#include "iface.h"

using namespace std;

int main(int arg, char **argv){
  
  iface a(argv[1]);
  //a.load();
  for (int i=0;i<atoi(argv[2]);i++)
  {
    sleep(1);
    cout << a << endl;
    a.update();
  }
  a.save();
  cout << a << endl;
  return 0;
  
}
