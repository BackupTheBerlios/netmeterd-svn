#include "functions.h"

string trim(string & str) {
  while (str[str.size()-1]==' ') str=str.substr(0,str.size()-2);
  while (str[0]==' ') str=str.substr(1,str.size()-1);
  return str;
} 
