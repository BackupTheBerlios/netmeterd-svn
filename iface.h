#ifndef _IFACE_H
#define _IFACE_H


class iface:
{  
  public:
    iface(const char** name, const ifdata & data);
    iface(const iface & source);
    ~iface();
    ifdata gettotal();
    ifdata getdaily();
    ifdata getweekly();
    ifdata getmonthly();
    ifdata getyearly();
    void settotal(const ifdata & data);
    void setdaily(const ifdata & data);
    void setweekly(const ifdata & data);
    void setmonthly(const ifdata & data);
    void setyearly(const ifdata & data);
  private:
    ifdata total;
    ifdata daily;
    ifdata weekly;
    ifdata monthly
    ifdata yearly;
}
  
  
