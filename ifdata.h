//$Id$

#ifndef _IFDATA_H
#define _IFDATA_H

#include "counter.h"


class ifdata
{
  friend std::ostream & operator<<(std::ostream & out, const ifdata &rhs);
  
  public:
    
    ifdata(counter sup, counter sdown);
    ifdata(const ifdata & source);
    ifdata(double upcount, short int upunit, double downcount, double downunit);
    //~ifdata();
    void setUp(const counter & sup);
    void setDown(const counter & sdown);
    counter up();
    counter up() const;
    counter down();
    counter down() const;
    const ifdata operator+(const ifdata & add);
    const ifdata operator=(const ifdata & source);
  private:
    counter Up;
    counter Down;
};
#endif
