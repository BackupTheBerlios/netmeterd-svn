//$Id$

#include <ctime>
#include <fstream>
#include <cstdlib>
#include "iface.h"

char *units[] = { "B","KB","MB"};

string trim(string str)
{
  while (str[str.size()-1]==' ') str=str.substr(0,str.size()-2);
  while (str[0]==' ') str=str.substr(1,str.size()-1);
  return str;
}

string itoa(const int &x)
{
  char buf[256];
  sprintf(buf,"%d",x);
  return static_cast <string> (buf);
}

iface::iface(const string &thename, const counter &thecount, const Date &thedate)
{
  name = thename;
  count = thecount;
  date = thedate;
}

iface::iface(const string &thename)
{
  name = thename;
  count = counter(0,0,0,0);
  time_t mytime;
  struct tm *times;
  time(&mytime);
  times = localtime(&mytime);
  date.day = times->tm_mday;
  date.month = times->tm_mon;
  date.year = times->tm_year;

}

iface::iface(const iface &source)
{
  name = source.name;
  count = source.count;
  date = source.date;
}

iface::iface() 
{
  iface("");
}

bool iface::load(const method &access)
{
  string buf,path;
  int st,end;
  double up,down;
  sint upunit,downunit;
  bool found=false;
  
  if (access.type == FS)
    path = access.data+itoa(date.month)+'-'+itoa(date.year);
  else return false;
  
  ifstream fd (path.c_str());
  if (!fd) return true;
  
  while (fd >> buf && !found)
    if (buf == '['+itoa(date.day)+']') // we have found the day
      while (fd >> buf && !found)
        if (buf.find(name,0) != string::npos) //Yeah, we've got the iface too
        {
          found = true;
          st = buf.find('=',0)+1;
          end = buf.find(',',0);
          up = atof(buf.substr(st,end-st-1).c_str());
          st = end+1;
          end = buf.find(',',st);
          upunit = atoi(buf.substr(st,end-st-1).c_str());
          st = end+1;
          end = buf.find(',',st);
          down = atof(buf.substr(st,end-st-1).c_str());
          st = end+1;
          end = buf.find(',',st);
          downunit = atoi(buf.substr(st,end-st-1).c_str());
          counter tmp(up,upunit,down,downunit);
          count = tmp;
        } 
    return true;
}

void iface::update()
{

  double bytesup,bytesdown;


#ifdef LINUX26
// Implemetation using /sys/class/net
  
  string aux,path;
  path = "/sys/class/net/"+name+"/statistics/tx_bytes";
  ifstream fd (path.c_str());
  if (fd)
  {
    fd >> aux;
    fd.close();
    bytesup = atof(aux.c_str());
    path = "/sys/class/net/"+name+"/statistics/rx_bytes";
    fd.open(path.c_str(),ios::in);
    fd >> aux;
    fd.close();
    bytesdown = atof(aux.c_str());
  }
  else { bytesup=bytesdown=0;}
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
    count.reduce();
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
    return true;
  }

  return false;
}

ostream &operator<<(ostream &out,const iface &oface)
{
 out << "Iface: " << oface.name << endl << "TX: " << oface.count.getUp() << " " << units[oface.count.getUpUnit()] << endl << "RX: " << oface.count.getDown() << " " << units[oface.count.getDownUnit()];
 return out;
}


