VERSION = "\"0.1\""
EXE = netmeterd
RELNAME = $(EXE)-$(VERSION)
CXX = g++
CXXFLAGS += -Wall
SOURCES = counter.o iface.o $(EXE).cc
KV = $(shell uname -r|cut -d. -f 1-2)

ifeq ($(KV),2.6)
	CXXFLAGS += -DLINUX26
endif

ifeq ($(LOGDIR),)
	LOGDIR = "\"/var/log/netmeterd\""
endif

ifeq ($(INSDIR),)
	INSDIR = /usr/sbin
endif

.PHONY: all clean

all: $(EXE)

clean: 
	${RM} $(EXE) *.o *~

iface.o: iface.cc
	$(CXX) -c $(CXXFLAGS) -DLOGDIR=$(LOGDIR) $^ -o $@
	
$(EXE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -DVERSION=$(VERSION) -DLOGDIR=$(LOGDIR) $^ -o $@

install:
	install -d $(LOGDIR)
	install -m 755 netmeterd $(INSDIR)


dist:
	mkdir $(RELNAME)
	cp *.cc *.h LICENSE ChangeLog Makefile $(RELNAME)
	tar cvvjf $(RELNAME).tar.bz2 $(RELNAME)
	${RM} -r $(RELNAME)
