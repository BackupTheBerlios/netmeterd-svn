//$Id$


#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <getopt.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "counter.hpp"
#include "iface.hpp"

#ifndef LOGDIR
#define LOGDIR "/var/log/netmeterd"
#endif

std::vector <iface> ifaces;

void help()
{
  std::cout << "Netmeterd Version " << VERSION << std::endl
  << "Copyright: Alberto Garcia Hierro and Ayoze Roberto "
  << "Bernardo" << std::endl
  << "Usage: netmeterd [options]" << std::endl
  << "-i:   Interfaces to monitorize (interface1,interface2,...)"
  << std::endl
  << "-t:   Seconds between counter updates (default 1)"
  << std::endl
  << "-T:   Minutes between saves to disk (default 5)" << std::endl
  << "-l:   Specifies an alternate log dir " << std::endl
  << "       (default " << LOGDIR << ")" << std::endl
  << "-f:   Run on foreground (don't daemonize)" << std::endl;

}

std::vector <std::string> getNames (const std::string &interfaces)
{
  std::vector <std::string> ret;
  std::string buf="";
  
  for (unsigned int i=0;i<interfaces.size();i++)
    if (interfaces.at(i) != ',')
    {
      buf += interfaces.at(i);
    }
    else
    {
      ret.push_back(buf);
      buf="";
    }
  //We need to push the last name too
  ret.push_back(buf);
  return ret;
}

void quit(int sig){
  std::cout << "Signal " << sig << " received" << std::endl;
  std::cout << "Saving status of ifaces" << std::endl;
  for (unsigned int i=0;i<ifaces.size();i++)
    ifaces[i].save();
  std::cout << "Exiting..." << std::endl;
  exit(1);
}



int main(int argc, char *argv[])
{
  int updatetime=1,savetime=5,ch;
  time_t prevsave=time(0);
  bool wantsdaemon=true;
  extern char *optarg;
  struct stat statBuf;
  std::string logdir="",interfaces="";
  std::vector <std::string> names; 
  struct sigaction action;

  //Parse command line
  while ((ch = getopt (argc, argv, "i:t:T:l:fh")) != -1)
    switch (ch)
    {
      case 'i':
        interfaces = std::string(optarg);
        break;
      case 'f':
        wantsdaemon=false;
        break;
      case 't':
        updatetime = atoi(optarg);
        break;
      case 'T':
        savetime = atoi(optarg);
        break;
      case 'l':
        logdir = std::string(optarg);
        break;
      case 'h':
        help();
        return 0;
        break;
      case '?':
      default:
        std::cout << "Try -h for more information." << std::endl;
        return 1;
      }

  if (interfaces=="")
  {
    std::cout << "Error: you must specify one interface at least."
    << std::endl;
    return 1;
  }
  
  names = getNames(interfaces);
  
  //Create the ifaces
  for (unsigned int i=0;i<names.size();i++)
    ifaces.push_back(iface(names[i]));

  //Set the logdir
  if(logdir!="")
    ifaces[0].setLogdir(logdir);
    
  
  //Check write perms
  if (stat(ifaces[0].getLogdir().c_str(), &statBuf) < 0)
  {
    std::cout << "Error: Cannot stat "
    << ifaces[0].getLogdir().c_str() << std::endl
    << "Exiting..." << std::endl;
    return 1;
  }

  //Ugly conditional. Returns false if logdir 
  //is not a writable directoriy
  if (!S_ISDIR(statBuf.st_mode) || \
    !(statBuf.st_mode & S_IWOTH || \
    ( statBuf.st_mode & S_IWGRP && statBuf.st_gid == getgid()) || \
    ( statBuf.st_mode & S_IWUSR && statBuf.st_uid == getuid()))) 
  {
    std::cout << "Error: " << ifaces[0].getLogdir().c_str()
    << " should be a writable directory" << std::endl
    << "Exiting..." << std::endl;
    return 1;
  }
            
  
  //Allright, so here we go :)
  
  //Load previous values, if any
  for (unsigned int i=0;i<names.size();i++)
    ifaces[i].load();

  
  //Signal handlers
  action.sa_handler = quit;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGINT, &action, NULL);
  
  //Should I go background?
  if(wantsdaemon)
    daemon(0,0);
   
  //The main loop, a.k.a. the funny thing
  while (true)
  {  
    //Update the ifaces
    for (unsigned int i=0;i<ifaces.size();i++)
    {
      ifaces[i].update();
      //Some verbosity for foreground runs
      std::cout << ifaces[i] << std::endl;
    }
    //If save time has passed, save the ifaces
    if (time(0)-prevsave >= savetime*60)
    {
      for (unsigned int i=0;i<ifaces.size();i++)
      { 
        ifaces[i].save();
        //Some verbosity OMT
        std::cout << "Saving state of " << ifaces[i].getName() << std::endl;
      }
      
      prevsave=time(0);
    }
    //Wait for the next update
    sleep(updatetime);
  }
  return 0;
}
