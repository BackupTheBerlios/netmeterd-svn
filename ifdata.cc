//$Id$

#include "ifdata.h"


ifdata::ifdata(counter sup, counter sdown)
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
  Up = counter up(upcount, upunit);
  Down = counter down(downcount, downunit);
}

void ifdata::setUp(const counter &sup)
{
  Up = sup;
}

void ifdata::setDown(const counter &sdown)
{
  Down = sdown;
}

counter ifdata::up()
{
  return Up;
}

counter ifdata::up() const
{
  return up();
}

counter ifdata::down()
{
  return Down;
}

counter ifdata::down() const
{
  return down();
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
