#!/usr/bin/env python

#$Id$

import sys,getopt,exceptions

def help():
  pass


def getData(date,interfaces):
  
  #Init the dict
  ifaces = {}
  buf = ""
  for char in interfaces:
    if char != ',':
      buf += char
    else:
      ifaces[buf] = {}
      buf = ""
  ifaces[buf] = {}
  
  limitday = 31 #Do you know any month with more than 31 days? 
  tmpday, tmpmonth, tmpyear = tuple(date.split('-')[0].split('/'))
  day1, month1, year1 = int(tmpday), int(tmpmonth), int(tmpyear)
  tmpday, tmpmonth, tmpyear = tuple(date.split('-')[1].split('/'))
  day2, month2, year2 = int(tmpday), int(tmpmonth), int(tmpyear)
  
  while day1 != day2 or month1 != month2 or year1 != year2:
    try:
      fd = open(logpath+'/log-'+str(month1)+'-'+str(year1))
    except:
      fd = 0

    if fd:
      print 'existe'
      if month1 == month2 and year1 == year2:
        limitday == day2
      for line in fd.readlines():
        day = int(line.split[0])
        iface = line.split()[1]
        if day >= day1 <= limitday and ifaces.has_key(iface):
          ifaces[iface][day] = line.split()[2:]

    day1 = 1
    month1 += 1
    if month1 > 12:
      month1 = 1
      year1 += 1 
  
  return ifaces
  pass


def main():

  #Some default values
  date=""
  interfaces=""
  logdir='/var/log/netmeterd/'
  templatedir='/usr/share/netmeterd/templates/'
  
  #Parse command line
  try:
    options, notUsed = getopt.getopt(sys.argv[1:],'d:l:i:t:T:','help')
  except getopt.GetoptError:    
    help()
    print 'mierda'
    sys.exit(1)
  
  for option in options:
    switch, value = option
    if switch == '-d' and value != '': date = value
    elif switch == '-l' and value != '': logdir = value
    elif switch == '-i' and value != '': interfaces = value
    elif switch == '-t' and value != '': template = value
    elif switch == '-T' and value != '': templatedir = value
    elif switch == '--help' or switch == '-h': help()
      
  if not interfaces:# or not date:
    print "You must specify one interface and one date range at least"
    help()
    sys.exit(1)

  ifaces = getData(date,interfaces)
  print ifaces

if __name__ == "__main__" :
  main()
  

