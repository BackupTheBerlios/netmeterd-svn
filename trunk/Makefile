VERSION = 0.1
CXX = g++
CXXFLAGS += -Wall
EXE = netmeterd
SOURCES = counter.o iface.o $(EXE).cc

KV = $(shell uname -r|cut -d. -f 1-2)

ifeq ($(KV),2.6)
  CXXFLAGS += -DLINUX26
endif

.PHONY: all clean

all: $(EXE)

clean: 
	${RM} $(EXE) *.o

$(EXE): $(SOURCES)
	$(CXX) $(CXXFLAGS) -DVERSION=$(VERSION) $^ -o $@
