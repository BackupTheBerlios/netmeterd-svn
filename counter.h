//$Id$

#ifndef _COUNTER_H
#define _COUNTER_H

#include <ostream>

class counter
{
  friend std::ostream & operator<<(std::ostream &out, const counter &rhs);
  
  public:
   
    
    static enum Unit_t {B,KB,MB,GB,TB,PT,HB} Units;
  
    counter(double count=0, short int unit=0);
    counter(const counter &source);
    double count() const;
    short int unit() const;
    void setCount(const double &count);
    void setUnit(const short int &unit);
    void reduce();
    counter operator+(const counter &add);
    counter operator=(const counter &source);
  private:
    double Count;
    short int Unit;
};
#endif
