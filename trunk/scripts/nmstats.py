#!/usr/bin/env python

#$Id$

import sys,getopt,exceptions,time

def help():
  pass


def isAfter(refDay,refMonth,refYear,day,month,year):
  if year > refYear: return True
  elif year < refYear: return False
  else:
    if month > refMonth: return True
    elif month < refMonth: return False
    else:
      if day > refDay: return True
      else: return False
  pass


def getData(date,interfaces,logdir):
  
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
  
  if not date in ('today','month','year'):
    try:
      tmpDay, tmpMonth, tmpYear = tuple(date.split('-')[0].split('/'))
      stDay, stMonth, stYear = int(tmpDay), int(tmpMonth), int(tmpYear)
      tmpDay, tmpMonth, tmpYear = tuple(date.split('-')[1].split('/'))
      endDay, endMonth, endYear = int(tmpDay), int(tmpMonth), int(tmpYear)
    except:
      print 'Please, specify a correct date in the form'
      print 'day/month/year-day/month/year or today or month or year'
      print 'i.e. 1/1/1983-25/5/1985'
      sys.exit(1)
  else:
    stYear, stMonth, stDay = endYear, endMonth, endDay = \
    time.gmtime()[:3]
    if date == 'month':
      stDay = 1
      endDay = 31
    elif date == 'year':
      stDay = 1
      endDay = 31
      stMonth = 1
      endMonth = 12
            
  while not isAfter(endDay,endMonth,endYear,stDay,stMonth,stYear):
    limitDay = 31 #Do you know any month with more than 31 days? 
    try:
      fd = open(logdir+'/log-'+str(stMonth)+'-'+str(stYear))
    except:
      fd = 0

    if fd:
        
      if stMonth == endMonth and stYear == endYear:
        limitDay = endDay
      for line in fd.readlines():
        day = int(line.split()[0])
        iface = line.split()[1]
        if day >= stDay <= limitDay and ifaces.has_key(iface):
          ifaces[iface][day] = line.split()[2:]
    
    stDay = 1
    stMonth += 1
    if stMonth > 12:
      stMonth = 1
      stYear += 1 
  
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
    sys.exit(1)
  
  for option in options:
    switch, value = option
    if switch == '-d' and value != '': date = value
    elif switch == '-l' and value != '': logdir = value
    elif switch == '-i' and value != '': interfaces = value
    elif switch == '-t' and value != '': template = value
    elif switch == '-T' and value != '': templatedir = value
    elif switch == '--help' or switch == '-h': help()
      
  if not interfaces or not date:
    print "You must specify one interface and one date range at least"
    help()
    sys.exit(1)

  ifaces = getData(date,interfaces,logdir)
  print ifaces

if __name__ == "__main__" :
  main()
  

