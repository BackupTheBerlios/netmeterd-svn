//$Id$

#ifndef _IFDATA_H
#define _IFDATA_H

#include "counter.h"
#include <ctime>

class ifdata
{
  friend std::ostream & operator<<(std::ostream & out, const ifdata &rhs);
  
  public:
    
    ifdata(counter sup, counter sdown);
    ifdata(const ifdata &source);
    ifdata(double upcount, short int upunit, double downcount, short int downunit);
    inline ~ifdata(){};
    inline void setUp(const counter &sup){ Up = sup;};
    inline void setDown(const counter & sdown){ Down = sdown;};
    inline counter up(){ return Up;};
    inline counter up() const{ return Up;};
    inline counter down(){ return Down;};
    inline counter down() const{ return Down;};
    const ifdata operator+(const ifdata &add);
    const ifdata operator=(const ifdata &source);
  private:
    counter Up;
    counter Down;
    tm date;
};
#endif
