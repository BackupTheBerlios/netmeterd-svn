//$Id$

#include "ifdata.h"


ifdata::ifdata(counter sup=0, counter sdown=0)
{
  Up = sup;
  Down = sdown;
}

ifdata::ifdata(const ifdata &source)
{
  ifdata(source.up(), source.down());
}

ifdata::ifdata(double upcount, short int upunit, double downcount, short int downunit)
{
  Up = counter(upcount, upunit);
  Down = counter(downcount, downunit);
}

const ifdata ifdata::operator+(const ifdata &add)
{
  return ifdata(up()+add.up(),down()+add.down());
}
        
const ifdata ifdata::operator=(const ifdata &source)
{
  return ifdata(source);
}

std::ostream & operator<<(std::ostream &out, const ifdata &rhs)
{
  out << "Up: " << rhs.up() << '\t' << "Down: " << rhs.down();
  return out;
}
