//$Id$

#include "counter.h"
#include <cmath>

#include <iostream>

counter::counter(double up, sint upunit, \
                double down, sint downunit)
{
  this->up = up;
  this->upunit = upunit;
  this->down = down;
  this->downunit = downunit;
  
  if (up >= 1024 || down >= 1024) reduce();
}

counter::counter()
{
  this->up = this->down = 0;
  this->upunit = this->downunit = 0;
}

counter::counter(const counter &source)
{
  counter(source.up,source.upunit,source.down,source.downunit);
}


void counter::reduce()
{
  while (this->up >= 1024 && \
        this->upunit <= counter::HB)
  {
    this->up/=1024;
    this->upunit++;
  }
  while (this->down >= 1024 && \
        this->downunit <= counter::HB)
  {
    this->down/=1024;
    this->downunit++;
  }
}


counter counter::operator+(const counter &add) 
{
  counter aux;
  //First, the up
  if (this->upunit == add.upunit)
  {
    aux.up = this->up+add.up;
    aux.upunit = this->upunit;
  }
  else
    if (this->upunit > add.upunit)
    {
      aux.up = this->up*pow(1024,(this->upunit-add.upunit))+add.up;
      aux.upunit = add.upunit;
    }
    else
    if (add.upunit > this->upunit)
    {
      aux.up = this->up+add.up*pow(1024,(add.upunit-this->upunit));
      aux.upunit = this->upunit;
    }
  //Now the down
  if (this->downunit == add.downunit)
  {
    aux.down = this->down+add.down;
    aux.downunit = this->downunit;
  }
  else
    if (this->downunit > add.downunit)
    {
      aux.down = this->down*pow(1024,(this->downunit-add.downunit))+add.down;
      aux.downunit = add.downunit;
    }
    else
      if (add.downunit > this->downunit)
      {
        aux.down = this->down+add.down*pow(1024,(add.downunit-this->downunit));
        aux.downunit = this->downunit;
      }
  
  if (aux.upunit >= 1024 || aux.downunit >= 1024)
    aux.reduce();
  return aux;
}

void counter::operator=(const counter &source)
{
 
 this->up = source.up;
 this->upunit = source.upunit;
 this->down = source.down;
 this->downunit = source.downunit;
  
}

void counter::operator+=(const counter &add)
{
  *this = *this+add;
}

