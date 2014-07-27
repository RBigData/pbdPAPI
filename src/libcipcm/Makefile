CC=gcc
CXX=g++

CORE=IntelPerformanceCounterMonitorV2.6
#msr.o cpucounters.o pci.o client_bw.o

# uncomment if you want to rely on Linux perf support (user needs CAP_SYS_ADMIN privileges)
ifneq ($(wildcard /usr/include/linux/perf_event.h),)
#OPT+= -DPCM_USE_PERF 
endif

UNAME:=$(shell uname)

ifeq ($(UNAME), Linux)
LIB= -lpthread -lrt
endif
ifeq ($(UNAME), Darwin)
LIB= -lpthread -lPcmMsr
endif
ifeq ($(UNAME), FreeBSD)
LIB= -lpthread
endif

DEFS=-DPCM_FORCE_SILENT

default: test

test: libcipcm.so
	$(CC) -Wall -L. -o test test.c -lstdc++ -lcipcm -lpcm $(LIB)

core:
	cd $(CORE) && make -f ../CoreMakefile DEFS=$(DEFS)

libpcm.so: core
	cd $(CORE) && $(CXX) -shared -Wl,-soname,libpcm.so -o ../libpcm.so msr.o pci.o client_bw.o cpucounters.o $(LIB)

libcipcm.so: libpcm.so
	$(CXX) -Wall -fPIC -I$(PAPI_SRC) -I$(CORE) $(DEFS) -c cwrap.cpp
	$(CXX) -Wall -fPIC -I$(PAPI_SRC) -I$(CORE) $(DEFS) -c fake_papi.cpp
	$(CXX) -shared -Wl,-soname,libcipcm.so -o libcipcm.so cwrap.o fake_papi.o -L. -lpcm

install: libcipcm.so
	cp *.so ${PREFIX}/

clean:
	rm *.o *.so

distclean: clean
	cd $(CORE) && make clean
