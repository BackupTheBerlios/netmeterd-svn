//$Id$

#ifndef _COUNTER_H
#define _COUNTER_H

typedef short int sint;

class counter
{
  
  public:
    static enum Unit_t {B,KB,MB,GB,TB,PT,HB} Units;
  
    counter(double , sint, double, sint);
    counter(const counter &source);
    counter();
    void reduce();
    void print();
    counter operator+(const counter &add);
    counter operator=(const counter &source);
  private:
    double up;
    sint upunit;
    double down;
    sint downunit;
};
#endif