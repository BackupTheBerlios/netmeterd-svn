#!/usr/bin/env python
# -*- coding: ISO8859-15 -*-
#$Id$


import getopt,exceptions,sys,os,time

units = {0: 'B',1: 'KB',2: 'MB', 3: 'GB', \
          4: 'TB', 5: 'PB', 6: 'HB', 'B': 0, \
          'KB': 1, 'MB': 2, 'GB': 3, 'TB': 4,
          'PB': 5, 'HB': 6 }


class counter:
  def __init__(self,count=0,unit=0):
    self.count = count
    try:
      unit.isdigit()
      self.unit = units[unit]
    except:
      self.unit = unit
  def __str__(self):
    return str(self.count)+' '+str(self.unit)
  def __repr__(self):
    return str(self.count)+' '+str(self.unit)
  def __eq__(self,other):
    if self.count == other.count and \
        self.unit == other.unit: return True
    return False
  def __gt__(self,other):
    if self.unit == other.unit:
      return self.counter > other.counter
    elif self.unit < other.unit:
      return
    





def help():
  revision = '$Rev$'[6:-2]
  print os.path.basename(sys.argv[0]),'revision', revision
  print os.path.basename(sys.argv[0]),'is part of netmeterd'
  print 'Copyright: Alberto Garcia Hierro and Ayoze Roberto Bernardo'
  print 'Usage:',os.path.basename(sys.argv[0]),'[options]'
  print 'Available options are:'
  print ' -i:    Interfaces to generate stats (interface1,interface2,...)'
  print ' -l:    Netmeterd logdir (defaults to /var/log/netmeterd)'
  print ' -o:    Output this for the HTML (defaults to .)'
  print ' -d:    Date specification in the form day/month/year-day/month/year'
  print '        Values "today", "month", and "year" are also accepted'
  print ' -t:    HTML template (defaults to "default")'
  print ' -T:    Template dir (defaults to /usr/share/netmeterd/templates/)'
  print ' -h:    Shows this help'
  print ''
  pass


def isAfter(refDay,refMonth,refYear,day,month,year):
  """Returns True if the date specified is after
  the reference date, False otherwise."""
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
  """Parses files in logdir, recollecting data that
  matchs the specified timespec and the selected
  interfaces. Returns a dict containing all the info""" 

  #Init the dict
  ifaces = {}
  buf = ""
  if logdir[-1] == '/':
    logdir = logdir[:-1]
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
    if date == 'year':
      stMonth = 1
      endMonth = 12
            
  while not isAfter(endDay,endMonth,endYear,stDay,stMonth,stYear):
    limitDay = 31 #Do you know any month with more than 31 days? 
    try:
      fd = open(logdir+'/log-'+str(stMonth)+'-'+str(stYear))
    except:
      fd = 0

    if fd:
    
      print 'Parsing:',logdir+'/log-'+str(stMonth)+'-'+str(stYear)    
      if stMonth == endMonth and stYear == endYear:
        limitDay = endDay
      for line in fd.readlines():
        day = int(line.split()[0])
        iface = line.split()[1]
        if day >= stDay <= limitDay and ifaces.has_key(iface):
          ifaces[iface][str(day)+'-'+str(stMonth)+'-'\
          +str(stYear)] = line.split()[2:]
    
    stDay = 1
    stMonth += 1
    if stMonth > 12:
      stMonth = 1
      stYear += 1 
  
  return ifaces
  pass

def expandVal(pair):
  val = float(pair[0])
   
  if pair[1] == 'B': pow=-2
  elif pair[1] == 'KB': pow=-1
  elif pair[1] == 'MB': pow=0
  elif pair[1] == 'GB': pow=1
  elif pair[1] == 'TB': pow=2
  elif pair[1] == 'PB': pow=3
  elif pair[1] == 'HB': pow=4
  val = val * (1024**pow)
  
  return val
  
def getTheBiggest(ifaces):
  value = 0
  for iface in ifaces.keys():
    for date in ifaces[iface].keys():
      tmpval = expandVal(ifaces[iface][date][:2])
      if tmpval > value: value=tmpval
      tmpval = expandVal(ifaces[iface][date][2:]) 
      if tmpval > value: value=tmpval
  return value
      
def createHTML(ifaces,templatedir,template,outdir,type):
  
  
  
  
  max = getTheBiggest(ifaces)
  print max
  pass

def main():

  #Some default values
  date=""
  interfaces=""
  logdir='/var/log/netmeterd/'
  template='default'
  templatedir='/usr/share/netmeterd/templates/'
  outdir='.'
  type = 'D'
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
    elif switch == '-o' and value != '': outdir = value
    elif switch == '-i' and value != '': interfaces = value
    elif switch == '-t' and value != '': template = value
    elif switch == '-T' and value != '': templatedir = value
    elif switch == '--help' or switch == '-h': help()
      
  if not interfaces or not date:
    print 'You must specify one interface and one date range at least'
    help()
    sys.exit(1)

  ifaces = getData(date,interfaces,logdir)
  print ifaces
  createHTML(ifaces,templatedir,template,outdir,type)

if __name__ == '__main__' :
  main()
  

