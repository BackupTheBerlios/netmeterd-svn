//$Id$

#include <iostream>
#include <getopt.h>
#include "counter.h"
#include "iface.h"

void help()
{
  std::cout << "Netmeterd Version " << VERSION << std::endl
  << "Copyright: Alberto Garcia Hierro and Ayoze Roberto "
  << "Bernardo" << std::endl
  << "Usage: netmeterd [options]" << std::endl
  << "-i:    Specifies the interfaces (interface1,interface2,...)"
  << std::endl
  << "-t:    Specifies the time in secodns between counter updates"
  << std::endl
  << "-f:    Run on foreground (don't daemonize)" << std::endl;

}

int main(int argc, char *argv[])
{
  int time=1,ch;
  bool wantsdaemon=true;
  std::string interfaces;
  

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

  std::cout << time << " " << interfaces << std::endl;
  return 0;
}
