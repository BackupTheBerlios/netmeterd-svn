//$Id$

#include <ctime>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>

#include "iface.hpp"

#include <iostream>

#ifndef LOGDIR
  #define LOGDIR "/var/log/netmeterd/"
#endif


//Static members
std::string iface::logdir=LOGDIR;


std::string trim(std::string str)
{
  while (str[str.size()-1]==' ') str=str.substr(0,str.size()-2);
  while (str[0]==' ') str=str.substr(1,str.size()-1);
  return str;
}

std::string itoa(const int &x)
{
  std::stringstream buf;
  buf << x;
  return buf.str();
}

std::string Unit(const sint &x)
{
  std::vector <std::string> units(7);
  units[0]="B";units[1]="KB";units[2]="MB";units[3]="GB";
  units[4]="TB";units[5]="PB";units[6]="HB";
  if (x < sint(units.size()))
    return units[x];
  return "E";
}

sint Unit(const std::string &str)
{
  std::vector <std::string> units(7);
  units[0]="B";units[1]="KB";units[2]="MB";units[3]="GB";
  units[4]="TB";units[5]="PB";units[6]="HB";

  for (sint i=0;i<sint(units.size());i++)
    if (units[i] == str)
      return i;
  
  return -1;
}


iface::iface(const std::string &name, \
            const counter &count, const Date &date)
{
  this->name = name;
  this->count = count;
  this->date = date;
}

iface::iface(const std::string &name)
{
  this->name = name;
  //count = counter(0,0,0,0);
  time_t mytime;
  struct tm *times;
  time(&mytime);
  times = localtime(&mytime);
  this->date.day = times->tm_mday;
  this->date.month = times->tm_mon+1;
  this->date.year = times->tm_year+1900;
  this->lastUp = this->lastDown = 0;

}

iface::iface(const iface &source)
{
  this->name = source.name;
  this->count = source.count;
  this->date = source.date;
}

iface::iface() 
{
  iface("");
}

bool iface::load()
{
  std::string buf,path;
  int pos;
  
  path = this->logdir+"/log-"+itoa(this->date.month)+'-'+ \
          itoa(this->date.year);
  
  if ((pos = this->ifind(path)) < 0) return true;
  std::ifstream fd (path.c_str());
  fd.seekg(pos);
  fd >> *this;
  fd.close();
  return true;
}

bool iface::save()
{
  std::string path,buf;
  std::fstream fd;
  int pos, endpos;

  path = this->logdir+"/log-"+itoa(this->date.month)+ \
        '-'+itoa(this->date.year);
  pos = this->ifind(path);
  if (pos >= 0)
  {
    fd.open(path.c_str(),std::ios::in|std::ios::out);
    if(!fd) return false;
    fd.seekp(pos);
    std::getline(fd,buf);
    endpos = fd.tellp();
    srand(static_cast<unsigned>(time(0)));
    std::string tmppath = this->logdir+"/tempfile-"+itoa(rand());
    std::ofstream tmp(tmppath.c_str());
    //This not represents a risk, since logdir should be writable
    //only by the user who owns the process
    fd.seekp(std::ios::beg);
    while (int(tmp.tellp())!= pos)
    {
      std::getline(fd,buf);
      if (buf!="\n")
        tmp << buf << std::endl;
    }
    tmp << *this << std::endl;
    fd.seekp(endpos);
    for (int i=0;!fd.eof();i++)
    {
      //Avoid the last \n
      if (i>0) tmp << std::endl;
      std::getline(fd,buf);
      tmp << buf;
    }
    tmp.close();
    remove(path.c_str());
    rename(tmppath.c_str(),path.c_str());
  }
  else
  {
    if (pos == -1)
      fd.open(path.c_str(),std::ios::out|std::ios::app);
    else
      fd.open(path.c_str(),std::ios::out);
    
    if(!fd) return false;
    
    fd << *this << std::endl;
  }
  
  fd.close();
  
  return true;
}
int iface::ifind(const std::string &path)
{
  std::string buf;
  int pos;
  std::ifstream fd(path.c_str());
  fd.seekg(std::ios::beg);
  if (!fd) return -2; //File not found
  while (!fd.eof())
  {
    pos = fd.tellg();
    std::getline(fd,buf);
    if ( buf.find(itoa(this->date.day),0) != std::string::npos  \
        && buf.find(this->name,0) != std::string::npos) //We've got it
      return pos;
  }
 //We haven't find the iface :-(  
  fd.close();
  return -1;
}

void iface::update()
{

  double bytesup,bytesdown;


#ifdef LINUX26
// Implemetation using /sys/class/net
  
  std::string buf,path;
  path = "/sys/class/net/"+this->name+"/statistics/tx_bytes";
  std::ifstream fd (path.c_str());
  if (fd)
  {
    fd >> buf;
    fd.close();
    bytesup = atof(buf.c_str());
    path = "/sys/class/net/"+this->name+"/statistics/rx_bytes";
    fd.open(path.c_str(),std::ios::in);
    fd >> buf;
    fd.close();
    bytesdown = atof(buf.c_str());
  }
  else { lastUp=lastDown=0;}
#else

// Implementation using /proc/net/dev

  int line=0,j=1;
  std::string temp,dev;
  double number=0;
  bool was_space=true,found=false;
  
  std::ifstream file ("/proc/net/dev", std::ios::in );
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

  if (lastUp && lastDown)
  {
    counter tmp(bytesup-lastUp,0,bytesdown-lastDown,0);
    this->count+=tmp;
    if (this->count.getUp() >= 1024 || this->count.getDown() >= 1024);
      this->count.reduce();
  }
  
  lastUp = bytesup;
  lastDown = bytesdown;
  
  renew();
}

bool iface::renew()
{
  time_t mytime;
  struct tm *times;
  time(&mytime);
  times = localtime(&mytime);
  if (this->date.day != times->tm_mday || this->date.month != times->tm_mon+1 \
      || this->date.year != times->tm_year+1900)
  {  
    this->save();
    this->date.day = times->tm_mday;
    this->date.month = times->tm_mon+1;
    this->date.year = times->tm_year+1900;
    this->count.reset();
    return true;
  }

  return false;
}

std::ostream &operator<<(std::ostream &out,const iface &oface)
{
 out << oface.date.day << " " << oface.name << " "
 << oface.count.getUp() << " " << Unit(oface.count.getUpUnit())
 << " " << oface.count.getDown() << " "
 << Unit(oface.count.getDownUnit());
 return out;
}

std::istream &operator>>(std::istream &in, iface &inface)
{
  std::string buf0,buf1,buf2,buf3,buf4;
  in >> buf0 >>  inface.name >> buf1 
  >> buf2 >> buf3 >> buf4;
  inface.count.setUp(atof(buf1.c_str()));
  inface.count.setUpUnit(Unit(buf2));
  inface.count.setDown(atof(buf3.c_str()));
  inface.count.setDownUnit(Unit(buf4));
  return in;
}
                          

