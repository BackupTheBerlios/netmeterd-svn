//$Id$

#include <iostream>
#include "counter.h"
#include "iface.h"

using namespace std;

int main(int arg, char **argv){
  
  Method access;
  access.type = FS;
  access.data = "";
  iface a(argv[1]);
  a.load(access);
  for (int i=0;i<atoi(argv[2]);i++)
  {
    sleep(1);
    cout << a << endl;
    a.update();
  }
  a.save(access);
  return 0;
  
}
