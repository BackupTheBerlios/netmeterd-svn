//$Id$

#ifndef _IFACE_H
#define _IFACE_H

#include <string>
#include "counter.h"


using namespace std;

//Some usefull functions
string trim (string);
string itoa(const int &);

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
    bool load(const Method &);
    //Modifies: Load the iface initial values using the supplied method
    //Returns: True on success or false on error
    bool save(const Method &);
    //Modifies: Saves the data using the supplied method
    //Returns: True on success or false on error
    int ifind(const string &);
    //Modifies: Nothing
    //Returns: the position of teh interface in the file or -1 is not found
    bool write(const string &, const  int &);
    //Modifies: Writes the counter values to a file ina determinated position
    //Returns:True on success, false on error
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
  
