# CC=gcc -g -O3    # WCC: Platform dependent, get from R as gcc -m32 -g
# CXX=g++ -g -O3   # WCC: Platform dependent, get from R as gcc -m64 -g
SONAME=soname
# CORE=IntelPerformanceCounterMonitorV2.6
CORE=ipcm-2.6      # WCC: Total path/file name should less than 100 chars.
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
SONAME=install_name
endif
ifeq ($(UNAME), FreeBSD)
LIB= -lpthread
endif

DEFS=-DPCM_FORCE_SILENT ${PASS_DEFS}

default: test

test: libcipcm.so
	${CC} -Wall -L. -o test test.c -lstdc++ -lcipcm -lpcm $(LIB)
	${CC} -Wall -L. -I../${PAPI_SRC} -o papitest papi_test.c -lstdc++ -lcipcm -lpcm $(LIB)

core:
	cd ${CORE} && make -f ../CoreMakefile "DEFS=$(DEFS)"

libpcm.so: core
	cd ${CORE} && ${CXX} -shared -Wl,-${SONAME},libpcm.${SOEX} -o ../libpcm.${SOEX} msr.o pci.o client_bw.o cpucounters.o $(LIB)

libcipcm.so: libpcm.so
	${CXX} -Wall -fPIC -I../${PAPI_SRC} -I${CORE} ${DEFS} -c cwrap.cpp
	${CXX} -Wall -fPIC -I../${PAPI_SRC} -I${CORE} ${DEFS} -c fake_papi.cpp
	${CXX} -shared -Wl,-${SONAME},libcipcm.${SOEX} -o libcipcm.${SOEX} cwrap.o fake_papi.o -L. -lpcm

install: libcipcm.so
	test -d inst || mkdir inst && cp *.${SOEX} inst/

clean:
	rm -rf *.o *.so *.dll inst

distclean: clean
	cd ${CORE} && make clean
