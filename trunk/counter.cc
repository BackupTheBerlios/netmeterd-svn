//$Id$

#include "counter.h"
#include <cmath>

//char *units[]= { "Bytes","Kilobytes","Megabytes","Gigabytes", "Terabytes","Petabytes","Hexabytes"};

counter::counter(double count, short int unit)
{
  Count = count;
  Unit = unit;
  if (Count >= 1024) reduce();
}
    
counter::counter(const counter &source)
{
  counter(source.count(),source.unit());
}

double counter::count() const
{
  return Count;
}

short int counter::unit() const
{
  return Unit;
}

void counter::setCount(const double &count)
{
  Count = count;
}

void counter::setUnit(const short int &unit)
{
  Unit = unit;
}

void counter::reduce()
{
  while (Count >= 1024)
  {
    Count/=1024;
    Unit++;
  }
}


counter counter::operator+(const counter &add) 
{
  counter aux;
  if (count() == add.count())
  {
    aux.Count = count()+add.count();
    aux.Unit = unit();
  }
  else
    if (count () > add.count())
    {
      aux.Count= count()*pow(1024,(count()-add.count()))+add.count();
      aux.Unit = add.unit();
    }
    else
    {
      aux.Count =count()+add.count()*pow(1024,(add.count()-count()));
      aux.Unit = unit();
    }
  
  if (aux.count() >= 1024)
    aux.reduce();
  return aux;
}

counter counter::operator=(const counter &source)
{
  return counter(source);
}

std::ostream & operator<<(std::ostream &out, const counter &rhs)
{
  out << rhs.count() << " unidad: " << rhs.unit();
  return out;
}

