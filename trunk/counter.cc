//$Id$

#include "counter.h"
#include <cmath>

#include <iostream>

counter::counter(double theup, sint theupunit, double thedown, sint thedownunit)
{
  up = theup;
  upunit = theupunit;
  down = thedown;
  downunit = thedownunit;
  
  if (up >= 1024 || down >= 1024) reduce();
}

counter::counter()
{
  up = down = 0;
  upunit = downunit = 0;
}

counter::counter(const counter &source)
{
  counter(source.up,source.upunit,source.down,source.downunit);
}


void counter::reduce()
{
  while (up >= 1024 && upunit <= counter::HB)
  {
    up/=1024;
    upunit++;
  }
  while (down >= 1024 && downunit <= counter::HB)
  {
    down/=1024;
    downunit++;
  }
}


counter counter::operator+(const counter &add) 
{
  counter aux;
  //First, the up
  if (upunit == add.upunit)
  {
    aux.up = up+add.up;
    aux.upunit = upunit;
  }
  else
    if (upunit > add.upunit)
    {
      aux.up = up*pow(1024,(upunit-add.upunit))+add.up;
      aux.upunit = add.upunit;
    }
    else
    {
      aux.up = up+add.up*pow(1024,(add.upunit-upunit));
      aux.upunit = upunit;
    }
  //Now the down
  if (downunit == add.downunit)
  {
    aux.down = up+add.down;
    aux.downunit = downunit;
  }
  else
    if (downunit > add.downunit)
    {
      aux.down = down*pow(1024,(downunit-add.downunit))+add.down;
      aux.downunit = add.downunit;
    }
    else
    {
      aux.down = down+add.down*pow(1024,(add.downunit-downunit));
      aux.downunit = downunit;
    }
  
  if (aux.upunit >= 1024 || aux.downunit >= 1024)
    aux.reduce();
  return aux;
}

void counter::operator=(const counter &source)
{
 up = source.up;
 upunit = source.upunit;
 down = source.down;
 downunit = source.downunit;
  
}

void counter::operator+=(const counter &add)
{
  *this = *this+add;
}

//For debbuging porpouses
void counter::print()
{
  std::cout << "Up: " << up << "\nDown: " << down << "\nUpunit " << upunit << "\nDownunit: " << downunit << "\n";
}
