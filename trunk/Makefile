CXX = g++
SOURCES = counter.o iface.o main.cc functions.cc
EXE = main
KERNEL_VERSION = $(shell uname -r|cut -d. -f 1-2)

ifeq ($(KERNEL_VERSION),2.6)
  CXXFLAGS+= -DLINUX26
endif

$(EXE): $(SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $@


.PHONY: all clean
all: $(EXE)
clean: 
	${RM} $(EXE) *.o
