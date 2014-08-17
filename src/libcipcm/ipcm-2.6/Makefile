#
# Copyright (c) 2009-2013 Intel Corporation
# written by Roman Dementiev
#

all: pcm-numa.x pcm-power.x pcm.x pcm-sensor.x pcm-msr.x pcm-memory.x pcm-tsx.x pcm-pcie.x

CC=g++ -Wall
OPT= -g -O3 

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


msr.o: msr.h msr.cpp 
	$(CC) $(OPT) -c msr.cpp

pci.o: pci.h pci.cpp
	$(CC) $(OPT) -c pci.cpp

client_bw.o: client_bw.h pci.h client_bw.cpp
	$(CC) $(OPT) -c client_bw.cpp

pcm-msr.o: msr.h msr.cpp pcm-msr.cpp
	$(CC) $(OPT) -c pcm-msr.cpp

pcm-memory.o: utils.h pcm-memory.cpp msr.h pci.h cpucounters.h types.h width_extender.h
	$(CC) $(OPT) -c pcm-memory.cpp

pcm-pcie.o: utils.h pcm-pcie.cpp msr.h pci.h cpucounters.h types.h width_extender.h
	$(CC) $(OPT) -c pcm-pcie.cpp

cpucounters.o: msr.h msr.cpp cpucounters.h cpucounters.cpp types.h width_extender.h 
	$(CC) $(OPT) -c cpucounters.cpp

msrtest.x: msrtest.cpp msr.o cpucounters.o cpucounters.h  msr.h  types.h
	$(CC) $(OPT) msrtest.cpp -o msrtest.x msr.o cpucounters.o pci.o $(LIB)

cpucounterstest.o: utils.h cpucounterstest.cpp cpucounters.h pci.h msr.h  types.h
	$(CC) $(OPT) -c cpucounterstest.cpp

pcm-power.o: utils.h pcm-power.cpp msr.h types.h pci.h cpucounters.h
	$(CC) $(OPT) -c pcm-power.cpp

pcm-numa.o: utils.h pcm-numa.cpp msr.h types.h pci.h cpucounters.h
	$(CC) $(OPT) -c pcm-numa.cpp

realtime.o: realtime.cpp cpucounters.h  msr.h  types.h
	$(CC) $(OPT) -c realtime.cpp

pcm.x: msr.o cpucounters.o cpucounterstest.o pci.o client_bw.o
	$(CC) $(OPT) msr.o pci.o client_bw.o cpucounters.o cpucounterstest.o -o pcm.x $(LIB)

pcm-tsx.o: pcm-tsx.cpp cpucounters.h pci.h msr.h  types.h
	$(CC) $(OPT) -c pcm-tsx.cpp

pcm-tsx.x: msr.o cpucounters.o pcm-tsx.o pci.o client_bw.o
	$(CC) $(OPT) msr.o pci.o client_bw.o cpucounters.o pcm-tsx.o -o pcm-tsx.x $(LIB)

pcm-power.x: msr.o cpucounters.o pci.o pcm-power.o client_bw.o
	$(CC) $(OPT) msr.o client_bw.o cpucounters.o pci.o pcm-power.o -o pcm-power.x $(LIB)

pcm-msr.x: msr.o pcm-msr.o
	$(CC) $(OPT) msr.o pcm-msr.o -o pcm-msr.x $(LIB)

realtime.x: msr.o cpucounters.o realtime.o
	$(CC) $(OPT) pci.o msr.o cpucounters.o realtime.o -o realtime.x $(LIB)

pcm-memory.x: msr.o pcm-memory.o pci.o cpucounters.o client_bw.o
	$(CC) $(OPT) msr.o pci.o cpucounters.o client_bw.o pcm-memory.o -o pcm-memory.x $(LIB)

pcm-pcie.x: msr.o pcm-pcie.o pci.o cpucounters.o client_bw.o
	$(CC) $(OPT) msr.o pci.o cpucounters.o client_bw.o pcm-pcie.o -o pcm-pcie.x $(LIB)

pcm-numa.x: msr.o pcm-numa.o pci.o cpucounters.o client_bw.o
	$(CC) $(OPT) msr.o pci.o cpucounters.o client_bw.o pcm-numa.o -o pcm-numa.x $(LIB)
pcm-sensor.o: pcm-sensor.cpp cpucounters.h cpuasynchcounter.h msr.h  types.h
	$(CC) $(OPT) -c pcm-sensor.cpp

pcm-sensor.x: msr.o cpucounters.o pcm-sensor.o pci.o client_bw.o
	$(CC) $(OPT) msr.o pci.o client_bw.o cpucounters.o pcm-sensor.o -o pcm-sensor.x $(LIB)

nice:
	uncrustify --replace -c ~/uncrustify.cfg *.cpp *.h WinMSRDriver/Win7/*.h WinMSRDriver/Win7/*.c WinMSRDriver/WinXP/*.h WinMSRDriver/WinXP/*.c  PCM_Win/*.h PCM_Win/*.cpp  

clean:
	rm -rf *.x *.o *~
