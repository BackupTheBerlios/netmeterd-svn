#include "ifdata.h"
#include <math.h>

ifdata::ifdata(double prevup, double prevdown, int prevunit)
{
  while ( up > 1024 && down > 1024 && unit <= ifdata::GIGABYTES)
  {
    up /= 1024;
    down /= 1024;
    unit++;
  }
  
  up = prevup;
  down = prevdown;
  unit = prevunit;
}

ifdata::ifdata(const ifdata & source)
{
  up = source.up;
  down = source.down;
  unit = source.unit;
}

void ifdata::setup(const double & prevup)
{
  up = prevup;
}

void ifdata::setdown(const double & prevdown)
{
  down = prevdown;
}

void ifdata::setunit(const int & prevunit)
{
  unit = prevunit;
}

double ifdata::getup()
{
  return up;
}

double ifdata::getdown()
{
  return down;
}

int ifdata::getunit()
{
  return unit;
}

const ifdata ifdata::operator+(const ifdata & add)
{
  ifdata aux;
  
  if (unit == add.unit)
  {
    aux.up = up + add.up;
    aux.down = down + add.down;
    aux.unit = unit;
  }
  else 
    if ( unit < add.unit)
    {
      aux.up = up + add.up * pow(1024, add.unit - unit);
      aux.down = down + add.down * pow(1024, add.unit - unit);
      aux.unit = add.unit;
    }
    else
      if ( add.unit < unit)
      {
        aux.up = up + add.up * pow(1024, unit - add.unit);
        aux.down = down + add.down * pow(1024, unit - add.unit);
        aux.unit = unit;
      }
  return aux;
}
        
const ifdata ifdata::operator=(const ifdata & source)
{
  return ifdata(source);
}

