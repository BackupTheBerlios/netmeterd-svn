//$Id$
#include <ctime>
#include "iface.h"

iface::iface(const string &thename, const counter &thecount, const Date &thedate)
{
  name = thename;
  counter = thecount;
  date = thedate;
}

iface::iface(const iface &source)
{
  name = source.name;
  counter = source.counter;
  date = source.date;
}

iface::iface(): counter(0,0,0,0){
{
  name = "";
  time_t mytime;
  struct tm *times;
  time(&mytime);
  times = localtime(&mytime);
  date.day = times->tm_day;
  date.month = times->tm_mon;
  date.year = times->tm_year;
  delete [] times;
}


