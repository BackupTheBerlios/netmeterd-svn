//$Id$

#ifndef _IFACE_H
#define _IFACE_H

#include <string>
#include "counter.h"



//Some usefull functions
std::string trim (std::string);
std::string itoa(const int &);
std::string Unit(const sint &);
sint Unit(const std::string &);

struct Date
{
  int day, month, year; //self explanatory
};

class iface
{  
  friend std::ostream &operator<<(std::ostream &, const iface &);
  friend std::istream &operator>>(std::istream &, iface &);
  
  public:
    iface(const std::string &, const counter &, const Date &);
    iface(const iface &);
    iface(const std::string &);
    iface();
    std::string getName() { return this->name;};
    std::string getLogdir() { return this->logdir;};
    void setLogdir(const std::string &logdir) { this->logdir=logdir;};
    bool load();
    //Modifies: Load the iface initial values using the supplied method
    //Returns: True on success or false on error
    bool save();
    //Modifies: Saves the data using the supplied method
    //Returns: True on success or false on error
    int ifind(const std::string &);
    //Modifies: Nothing
    //Returns: the position of teh interface in the file or -1 is not found
    void update();
    //Modifies: Updates the counter from /proc/net/dev or /sys/class/net/dev
    //Returns: Nothing
    bool renew();
    //Checks if day has changed and then, resets count and date
  private:
    std::string name;
    counter count;
    double lastUp;
    double lastDown;
    Date date;
    static std::string logdir;
};   
#endif  
  
