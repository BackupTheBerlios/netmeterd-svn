//$Id$

#include <vector>
#include <string>
#include <iostream>
#include <getopt.h>
#include <signal.h>

#include "counter.h"
#include "iface.h"

void help()
{
  std::cout << "Netmeterd Version " << VERSION << std::endl
  << "Copyright: Alberto Garcia Hierro and Ayoze Roberto "
  << "Bernardo" << std::endl
  << "Usage: netmeterd [options]" << std::endl
  << "-i:   Specifies the interfaces (interface1,interface2,...)"
  << std::endl
  << "-t:   Specifies the time in secodns between counter updates"
  << std::endl
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

void quit(){
  std::cout << "Morire con dignidad" << std::endl;
}



int main(int argc, char *argv[])
{
  int time=1,ch;
  bool wantsdaemon=true;
  std::string interfaces="";
  std::vector <iface> ifaces;
  std::vector <std::string> names; 
  struct sigaction signal_action;

  //Parse command line
  while ((ch = getopt (argc, argv, "i:t:fh")) != -1)
    switch (ch)
    {
      case 'i':
        interfaces = std::string(optarg);
        break;
      case 'f':
        wantsdaemon=false;
        break;
      case 't':
        time = atoi(optarg);
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
  
  //TODO: Check write perms
  
  //Signal handlers
  sigaddset(&signal_action.sa_mask, SIGTERM);
  signal_action.sa_handler = quit;
  sigaction(SIGTERM, &signal_action, 0);
          

  
  if(wantsdaemon)
    daemon(0,0);
   
  while (true);

  return 0;
}
