//$Id$

#include <ctime>
#include <fstream>
#include <cstdlib>
#include "iface.h"



iface::iface(const string &thename, const counter &thecount, const Date &thedate)
{
  name = thename;
  count = thecount;
  date = thedate;
}

iface::iface(const iface &source)
{
  name = source.name;
  count = source.count;
  date = source.date;
}

iface::iface() 
{
  name = "";
  count = counter(0,0,0,0);
  time_t mytime;
  struct tm *times;
  time(&mytime);
  times = localtime(&mytime);
  date.day = times->tm_mday;
  date.month = times->tm_mon;
  date.year = times->tm_year;
  delete [] times;
}

/*bool iface::load(const method &access)
{
  if (access.type == iface::FILESYSTEM)
    string path = access.data;
  else return false;
  
  ifstream f(path+)day,(string));


*/
void iface::update()
{

  double bytesup,bytesdown;


#ifdef LINUX26
// Implemetation using /sys/class/net
  
  string aux,path;
  path = "/sys/class/net/"+name+"/statistics/rx_bytes";
  ifstream f (path.c_str());
  f >> aux;
  f.close();
  bytesup = atof(aux.c_str());
  path = "/sys/class/net/"+name+"/statistics/tx_bytes";
  f.open(path.c_str(),ios::in);
  f >> aux;;
  f.close();
  bytesdown = atof(aux.c_str());
  
#else
// Implementation using /proc/net/dev

  ifstream f ("/proc/net/dev");
  string buf, temp;
  

#endif

  counter tmp(bytesup-lastUp,0,bytesdown-lastDown,0);
  count += tmp;
  lastUp = bytesup;
  lastDown = bytesdown;
  if (shouldRenew())
  {
    //save();
    //reset();
  }

}
bool iface::shouldRenew()
{
  time_t mytime;
  struct tm *times;
  time(&mytime);
  times = localtime(&mytime);
  if (date.day != times->tm_mday || date.month != times->tm_mon || date.year != times->tm_year)
  {  
    delete [] times;
    return true;
  }

  delete [] times;
  return false;
}
