//$Id$

#include <ctime>
#include <fstream>
#include <cstdlib>
#include "iface.h"
#include "functions.h"



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
  if (access.type == FS)
    string path = access.data+itoa(day)+;
  else return false;
  
  ifstream f(path.c_str());

}
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
  f >> aux;
  f.close();
  bytesdown = atof(aux.c_str());
  
#else

// Implementation using /proc/net/dev

  int line=0,j=1;
  string temp,dev;
  double number=0;
  bool was_space=true,found=false;
  
  fstream file ("/proc/net/dev", ios::in );
  while (getline(file,temp)) { //read data from file
    if (line>=2) { //First 2 lines are headers

      if (trim(temp.substr(0,temp.find(':',0))) == name) { // is the correct device?
        found=true;
        temp=trim(temp.substr(temp.find(':',0)+1,temp.size()-temp.find(':',0)-2)); //only get from ':'
        for(int pos=0;pos<temp.size();pos++) {
          if (temp[pos]!=' ') {
            number=number*10 + (temp[pos] - '0');
            was_space=false;
          }
          else {
            if (!was_space) { //A number has ended
              if (j==1) { bytesdown=number; } //save received in counter
              if (j==9) { bytesup=number; } //the same for sent
              number=0;  
              j++;
            }
            was_space=true;
          }
        }
      }
      line++; j=1;
    }
  }
  if (!found) { lastDown=0; lastUp=0; }
#endif
  if ( bytesup && bytesdown)
  {
    counter tmp(bytesup-lastUp,0,bytesdown-lastDown,0);
    count += tmp;
  }
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


  


