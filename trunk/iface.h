//$Id$

#ifndef _IFACE_H
#define _IFACE_H

#include <string>
#include "counter.h"


using namespace std;

//Some usefull functions
string trim (string);
string itoa(const int &);
string Unit(const sint &);
sint Unit(const string &);

struct Date
{
  int day, month, year; //self explanatory
};

struct Method
{
  sint type; //file or DB
  string data; //path or user:pass@host/db
};

enum methods {FS,BD};

using namespace std;

class iface
{  
  friend ostream &operator<<(ostream &, const iface &);
  friend istream &operator>>(istream &, iface &);
  
  public:
    iface(const string &, const counter &, const Date &);
    iface(const iface &);
    iface(const string &);
    iface();
    //~iface();
    bool load();
    //Modifies: Load the iface initial values using the supplied method
    //Returns: True on success or false on error
    bool save();
    //Modifies: Saves the data using the supplied method
    //Returns: True on success or false on error
    int ifind(const string &);
    //Modifies: Nothing
    //Returns: the position of teh interface in the file or -1 is not found
    void update();
    //Modifies: Updates the counter from /proc/net/dev or /sys/class/net/dev
    //Returns: Nothing
    bool renew();
    //Checks if day has changed and then, resets count and date
  private:
    string name;
    counter count;
    double lastUp;
    double lastDown;
    Date date;
    static string logdir;
};   
#endif  
  
