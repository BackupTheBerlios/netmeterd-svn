#!/usr/bin/env python

#$Id$

import sys,getopt,exceptions

def help():
  pass


def getData(date,ifaces):
  limitday = 31 #Do you know any month with more than 31 days? 
  tmpday, tmpmonth, tmpyear = touple(date.split('/')[0])
  day1, month1, year1 = int(tmpday), int(tmpmonth), int(tmpyear)
  tmpday, tmpmonth, tmpyear = touple(date.split('/')[1])
  day2, month2, year2 = int(tmpday), int(tmpmonth), int(tmpyear)
  
  while day1 != day2 or month1 != month2 or year1 != year2:
    try:
      fd = open(logpath+'/log-'+str(month1)+'-'+str(year1))
    except:
      fd = 0

    if fd:
      if month1 == month2 and year1 == year2:
        limitday == day
      for line in fd.readlines():
        day = int(line.split[0])
        iface = line.split()[1]
        if day >= day1 <= limitday and ifaces.has_key(iface):
          ifaces[iface][day] = line.split()[2:]

      day1 = 1
      month += 1
      if month > 12:
        month = 1
        year += 1 
  
  return ifaces
  
def main():

  #Some default values
  logdir='/var/log/netmeterd/'
  templatedir='/usr/share/netmeterd/templates/'
  
  #Parse command line
  try:
    options, notUsed = getopt.getopt(sys.argv[1:],'d:l:i:','help')
  except getopt.GetoptError:    
    help()
    sys.exit(1)
  
  for option in options:
    switch, value = option
    if switch == '-d' and value != '': date = value
    elif switch == '-l' and value != '': logdir = value
    elif switch == '-i' and value != '': interfaces = value
    elif switch == '--help' or switch == '-h': help()
      
  

if __name__ == "__main__" :
  main()
  

