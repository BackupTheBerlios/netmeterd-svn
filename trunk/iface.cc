//$Id$

#include <ctime>
#include <fstream>
#include <cstdlib>
#include "iface.h"
#include <vector>

#include <iostream>

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
  lastUp = lastDown = 0;

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

bool iface::load(const Method &access)
{
  string buf,path;
  int pos;
  
  if (access.type == FS)
    path = access.data+itoa(date.month)+'-'+itoa(date.year);
  path = "ifaces.txt"; //DEBUG
  //else return false;
  
  if ((pos = ifind(path)) < 0) return true;
  ifstream fd (path.c_str());
  fd.seekg(pos);
  fd >> *this;
  fd.close();
  return true;
}

bool iface::save(const Method &access)
{
  string path;
  int pos;
  fstream fd;
  
  if (access.type == FS)
    path = access.data+itoa(date.month)+'-'+itoa(date.year);
    path = "ifaces.txt"; //DEBUG
    pos = ifind(path);
    if (pos >= 0)
    {
      fd.open(path.c_str(),ios::in|ios::out);
      fd.seekp(pos);
      fd << endl << *this;
    }
    else if (pos == -3)
          {
            fd.open(path.c_str(),ios::out);
            fd << '[' << date.day << ']'
               << endl << *this << endl;
          }
    fd.close();
 
  return true;
}
int iface::ifind(const string &path)
{
  string buf;
  int pos;
  bool dayfound=false; 
  ifstream fd(path.c_str());
  fd.seekg(ios::beg);
  if (!fd) return -3; //File not found
  while ( fd >> buf)
  {
    dayfound=true;
    if (buf == string("["+itoa(date.day)+"]")) // we have found the day
      while (!fd.eof())
      {
        pos = fd.tellg();
        fd >> buf;
        if (buf.find(name,0) != string::npos) //Yeah, we've got the iface too
        {
          fd.close();
          return pos;
        }
      } 
  }  //We haven't find the iface :-(
  
  fd.close();
  if (dayfound) return -1;
  return -2;
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
    cout << aux << ":";
    path = "/sys/class/net/"+name+"/statistics/rx_bytes";
    fd.open(path.c_str(),ios::in);
    fd >> aux;
    cout << aux << endl;
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
        for(unsigned int pos=0;pos<temp.size();pos++) {
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

  if ( bytesup && bytesdown && lastUp && lastDown)
  {
    counter tmp(bytesup-lastUp,0,bytesdown-lastDown,0);
    count += tmp;
    if (count.getUp() >= 1024 || count.getDown() >= 1024);
      //count.reduce();
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
 out << oface.name << '=' << oface.count.getUp() << ","
 << oface.count.getUpUnit() << "," << oface.count.getDown() 
 << "," << oface.count.getDownUnit();
 return out;
}

istream &operator>>(istream &in, iface &inface)
{
  string buf;
  in >> buf;
  int st=0,end;
  end = buf.find('=',0)-1;
  inface.name = buf.substr(0,end);
  st = buf.find('=',0)+1;
  end = buf.find(',',0);
  inface.count.setUp(atof(buf.substr(st,end-st).c_str()));
  st = end+1;
  end = buf.find(',',st);
  inface.count.setUpUnit(atoi(buf.substr(st,end-st).c_str()));
  st = end+1;
  end = buf.find(',',st);
  inface.count.setDown(atof(buf.substr(st,end-st).c_str()));
  st = end+1;
  end = buf.find(',',st);
  inface.count.setDownUnit(atoi(buf.substr(st,end-st).c_str()));
  return in;
}
                          

