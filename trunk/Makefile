CXX = g++
CXXFLAGS += -Wall
SOURCES = counter.o iface.o main.cc
EXE = main
KERNEL_VERSION = $(shell uname -r|cut -d. -f 1-2)

ifeq ($(KERNEL_VERSION),2.6)
  CXXFLAGS += -DLINUX26
endif

.PHONY: all clean

all: $(EXE)

clean: 
	${RM} $(EXE) *.o

$(EXE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@
