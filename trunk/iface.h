//$Id$

#ifndef _IFACE_H
#define _IFACE_H

#include <string>
#include "counter.h"

using namespace std;

struct Date
{
  int day, month, year; //self explanatory
};

struct method
{
  sint type; //file or DB
  string data; //path or user:pass@host/db
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
    bool load(const method &);
    //Modifies: Load the iface initial values using the supplied method
    //Returns: True on success or false on error
    bool save(const method &);
    //Modifies: Saves the data using the supplied method
    //Returns: True on success or false on error
    void update();
    //Modifies: Updates the counter from /proc/net/dev or /sys/class/net/dev
    //Returns: Nothing
    bool shouldRenew();
    //Checks if day has changed and the counter need to be renewed
    void reset();
    //Puts the counter to 0
  private:
    string name;
    counter count;
    double lastUp;
    double lastDown;
    Date date;
};   
#endif  
  
