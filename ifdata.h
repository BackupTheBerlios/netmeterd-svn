#ifndef _IFDATA_H
#define _IFDATA_H


class ifdata
{
  public:
    enum units { BYTES, KILOBYTES, MEGABYTES, GIGABYTES};
    
    ifdata(double prevup=0, double prevdown=0, int prevunit=0);
    ifdata(const ifdata & source);
    ~ifdata();
    void setup(const double & prevup);
    void setdown(const double & prevdown);
    void setunit(const int & prevunit);
    double getup();
    double getdown();
    int getunit();
    const ifdata operator+(const ifdata & add);
    const ifdata operator=(const ifdata & source);
  private:
    double up;
    double down;
    int unit;
};
#endif
