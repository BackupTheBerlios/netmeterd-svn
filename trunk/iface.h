#ifndef _IFACE_H
#define _IFACE_H

#include <string>

struct Date
{
  int day, month, year;
};

using namespace std;

class iface
{  
  public:
    iface(const string &, const counter &, const Date &);
    //iface(const string &, const uint &, const sint &, const uint &, const sint &, const int &, const int &, const int &);
    iface(const iface &);
    iface();
    //~iface();
  private:
    char *name;
    counter count;
    Date date;
}   
#endif  
  
