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

#default: pcmc.a
default: test

test: libpcmc.a
	gcc -Wall -L. -o test test.c -lstdc++ -lpcmc -lpcm $(LIB)

core:
	cd $(CORE) && make -f ../CoreMakefile

libpcm.a: core
	cd $(CORE) && g++ -shared -Wl,-soname,libpcm.so -o ../libpcm.so msr.o pci.o client_bw.o cpucounters.o $(LIB)

libpcmc.a: libpcm.a
	g++ -Wall -fPIC -I$(CORE) -c cwrap.cpp
	g++ -shared -Wl,-soname,libpcmc.so -o libpcmc.so cwrap.o -L. -lpcm

clean:
	rm *.o *.so

distclean: clean
	cd $(CORE) && make clean
