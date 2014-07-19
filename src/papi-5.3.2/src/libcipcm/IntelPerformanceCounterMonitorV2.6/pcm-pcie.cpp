/*
   Copyright (c) 2009-2013, Intel Corporation
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Intel Corporation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// written by Patrick Lu


/*!     \file pcm-pcie.cpp
  \brief Example of using uncore CBo counters: implements a performance counter monitoring utility for monitoring PCIe bandwidth
  */
#define HACK_TO_REMOVE_DUPLICATE_ERROR
#include <iostream>
#ifdef _MSC_VER
#pragma warning(disable : 4996) // for sprintf
#include <windows.h>
#include "../PCM_Win/windriver.h"
#else
#include <unistd.h>
#include <signal.h>
#endif
#include <math.h>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <assert.h>
#include "cpucounters.h"
#include "utils.h"
#ifdef _MSC_VER
#include "freegetopt/getopt.h"
#endif


typedef struct
{
    // PCIe read events (PCI devices reading from memory)
    uint64 PCIePRd;   // PCIe UC read
    uint64 PCIeRdCur; // PCIe read current
    uint64 PCIeNSRd;  // PCIe non-snoop read
    // PCIe write events (PCI devices writing to memory)
    uint64 PCIeWiLF;  // PCIe Write (non-allocating)
    uint64 PCIeItoM;  // PCIe Write (allocating)
    uint64 PCIeNSWr;  // PCIe Non-snoop write (partial)
    uint64 PCIeNSWrF; // PCIe Non-snoop write (full)
} PCIeEvents_t;

PCIeEvents_t aggregate_sample;
uint32 num_events = (sizeof(PCIeEvents_t)/sizeof(uint64));

using namespace std;

const uint32 max_sockets = 4;
void getPCIeEvents(PCM *m, PCM::PCIeEventCode opcode, uint32 delay_ms, PCIeEvents_t *sample);

void print_events()
{
    cout << " PCIe event definitions (each event counts as a transfer): \n";
    cout << "   PCIe read events (PCI devices reading from memory - application writes to disk/network/PCIe device):\n";
    cout << "     PCIePRd   - PCIe UC read transfer (partial cache line)\n";
    cout << "     PCIeRdCur - PCIe read current transfer (full cache line)\n";
    cout << "     PCIeNSRd  - PCIe non-snoop read transfer (full cache line)\n";
    cout << "   PCIe write events (PCI devices writing to memory - application reads from disk/network/PCIe device):\n";
    cout << "     PCIeWiLF  - PCIe Write transfer (non-allocating) (full cache line)\n";
    cout << "     PCIeItoM  - PCIe Write transfer (allocating) (full cache line)\n";
    cout << "     PCIeNSWr  - PCIe Non-snoop write transfer (partial cache line)\n";
    cout << "     PCIeNSWrF - PCIe Non-snoop write transfer (full cache line)\n\n";
}

void print_usage(const char * progname)
{
    cout << "\nUsage "<<progname<<" (delay in seconds) [-C] [-B]\n\n";
    cout << "  <delay>        - delay in seconds between updates\n";
    cout << "  -C             - output in csv format (optional)\n";
    cout << "  -B             - Estimate PCIe B/W (B/s) by multiplying the number of transfers by the cache line size (64 bytes). Overestimates the bandwidth under traffic with many partial cache line transfers.\n\n";
    print_events();
}


int main(int argc, char * argv[])
{
#ifdef PCM_FORCE_SILENT
    null_stream nullStream1, nullStream2;
    std::cout.rdbuf(&nullStream1);
    std::cerr.rdbuf(&nullStream2);
#endif

    cout << endl;
    cout << " Intel(r) Performance Counter Monitor: PCIe Bandwidth Monitoring Utility "<< endl;
    cout << endl;
    cout << " Copyright (c) 2013 Intel Corporation" << endl;
    cout << " This utility measures PCIe bandwidth in real-time" << endl;
    cout << endl;
    print_events();
#ifdef _MSC_VER
    // Increase the priority a bit to improve context switching delays on Windows
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

    TCHAR driverPath[1032];
    GetCurrentDirectory(1024, driverPath);
    wcscat_s(driverPath, 1032, L"\\msr.sys");

    SetConsoleCtrlHandler((PHANDLER_ROUTINE)cleanup, TRUE);
#else
    signal(SIGPIPE, cleanup);
    signal(SIGINT, cleanup);
    signal(SIGKILL, cleanup);
    signal(SIGTERM, cleanup);
#endif

    uint32 delay = -1;
    bool csv = false;
    bool print_bandwidth = false;

    int my_opt = -1;
    while ((my_opt = getopt(argc, argv, "hBC")) != -1)
    {
           switch(my_opt)
           {
                   case 'C':
                   case 'c':
                        csv = true;
                        break;
                   case 'B':
                        print_bandwidth = true;
                        break;
                   case 'h':
                   default:
                       print_usage(argv[0]);
                       return -1;
           }
     }

     if (optind >= argc)
     {
         print_usage(argv[0]);
         return -1;
     }

     delay = atoi(argv[optind]);
     delay = (delay<0)?1:delay;

#ifdef _MSC_VER
    // WARNING: This driver code (msr.sys) is only for testing purposes, not for production use
    Driver drv;
    // drv.stop();     // restart driver (usually not needed)
    if (!drv.start(driverPath))
    {
        cout << "Cannot access CPU counters" << endl;
        cout << "You must have signed msr.sys driver in your current directory and have administrator rights to run this program" << endl;
    }
#endif

    PCM * m = PCM::getInstance();
    m->disableJKTWorkaround();
    PCM::ErrorCode status = m->program();
    switch (status)
    {
        case PCM::Success:
            break;
        case PCM::MSRAccessDenied:
            cout << "Access to Intel(r) Performance Counter Monitor has denied (no MSR or PCI CFG space access)." << endl;
            return -1;
        case PCM::PMUBusy:
            cout << "Access to Intel(r) Performance Counter Monitor has denied (Performance Monitoring Unit is occupied by other application). Try to stop the application that uses PMU." << endl;
            cout << "Alternatively you can try to reset PMU configuration at your own risk. Try to reset? (y/n)" << endl;
            char yn;
            std::cin >> yn;
            if ('y' == yn)
            {
                m->resetPMU();
                cout << "PMU configuration has been reset. Try to rerun the program again." << endl;
            }
            return -1;
        default:
            cout << "Access to Intel(r) Performance Counter Monitor has denied (Unknown error)." << endl;
            return -1;
    }
    
    cout << "\nDetected "<< m->getCPUBrandString() << " \"Intel(r) microarchitecture codename "<<m->getUArchCodename()<<"\""<<endl;
    if(!(m->hasPCICFGUncore()))
    {
        cout << "Jaketown or Ivytown CPU is required for this tool! Program aborted" << endl;
        m->cleanup();
        return -1;
    }

    if(m->getNumSockets() > max_sockets)
    {
        cout << "Only systems with up to "<<max_sockets<<" sockets are supported! Program aborted" << endl;
        m->cleanup();
        return -1;
    }

    cout << "Update every "<<delay<<" seconds"<< endl;

#define NUM_SAMPLES (1)

    uint32 i;
    uint32 delay_ms = delay * 1000 / num_events / NUM_SAMPLES;
    delay_ms * num_events * NUM_SAMPLES < delay * 1000 ? delay_ms += 1 : delay_ms = delay_ms; //Adjust the sleep if it's less than delay time
    PCIeEvents_t sample[max_sockets];
    printf("delay_ms:%u\n",delay_ms);
    
    while(1)
    {
        memset(sample,0,sizeof(sample));
        memset(&aggregate_sample,0,sizeof(aggregate_sample));

        for(i=0;i<NUM_SAMPLES;i++)
        {
            getPCIeEvents(m, m->PCIePRd, delay_ms, sample);
            getPCIeEvents(m, m->PCIeRdCur, delay_ms, sample);
            getPCIeEvents(m, m->PCIeNSRd, delay_ms, sample);
            getPCIeEvents(m, m->PCIeWiLF, delay_ms, sample);
            getPCIeEvents(m, m->PCIeItoM, delay_ms, sample);
            getPCIeEvents(m, m->PCIeNSWr, delay_ms, sample);
            getPCIeEvents(m, m->PCIeNSWrF, delay_ms, sample);
        }
        
        if(csv)
            if(print_bandwidth)
                cout << "Skt,PCIePRd,PCIeRdCur,PCIeNSRd,PCIeWiLF,PCIeItoM,PCIeNSWr,PCIeNSWrF,PCIe Rd (B),PCIe Wr (B)\n";
            else
                cout << "Skt,PCIePRd,PCIeRdCur,PCIeNSRd,PCIeWiLF,PCIeItoM,PCIeNSWr,PCIeNSWrF\n";
        else
            if(print_bandwidth)
                cout << "Skt | PCIePRd  | PCIeRdCur | PCIeNSRd  | PCIeWiLF | PCIeItoM | PCIeNSWr | PCIeNSWrF | PCIe Rd (B) | PCIe Wr (B)\n";
            else
                cout << "Skt | PCIePRd  | PCIeRdCur | PCIeNSRd  | PCIeWiLF | PCIeItoM | PCIeNSWr | PCIeNSWrF\n";

        //report extrapolated read and write PCIe bandwidth per socket using the data from the sample
        for(i=0; i<m->getNumSockets(); ++i)
        {
            if(csv)
            {
                cout << i;
                cout << "," << sample[i].PCIePRd;
                cout << "," << sample[i].PCIeRdCur;
                cout << "," << sample[i].PCIeNSWr;
                cout << "," << sample[i].PCIeWiLF;
                cout << "," << sample[i].PCIeItoM;
                cout << "," << sample[i].PCIeNSWr;
                cout << "," << sample[i].PCIeNSWrF;
                if(print_bandwidth)
                {
                    cout << "," << ((sample[i].PCIePRd+ sample[i].PCIeRdCur+ sample[i].PCIeNSWr)*64ULL);
                    cout << "," << ((sample[i].PCIeWiLF+sample[i].PCIeItoM+sample[i].PCIeNSWr+sample[i].PCIeNSWrF)*64ULL);
                }
                cout << "\n";
            }
            else
            {
                cout << " " << i;
                cout << "    " << unit_format(sample[i].PCIePRd);
                cout << "      " << unit_format(sample[i].PCIeRdCur);
                cout << "      " << unit_format(sample[i].PCIeNSWr);
                cout << "      " << unit_format(sample[i].PCIeWiLF);
                cout << "     " << unit_format(sample[i].PCIeItoM);
                cout << "     " << unit_format(sample[i].PCIeNSWr);
                cout << "     " << unit_format(sample[i].PCIeNSWrF);
                if(print_bandwidth)
                {
                    cout << "        " << unit_format((sample[i].PCIePRd+ sample[i].PCIeRdCur+ sample[i].PCIeNSWr)*64ULL);
                    cout << "         " << unit_format((sample[i].PCIeWiLF+sample[i].PCIeItoM+sample[i].PCIeNSWr+sample[i].PCIeNSWrF)*64ULL);
                }
                cout << "\n";
            }
        }
        if(!csv)
        {
            if(print_bandwidth)
                cout << "----------------------------------------------------------------------------------------------------------------\n";
            else
                cout << "-----------------------------------------------------------------------------------\n";
            cout << " * ";
            cout << "   " << unit_format(aggregate_sample.PCIePRd);
            cout << "      " << unit_format(aggregate_sample.PCIeRdCur);
            cout << "      " << unit_format(aggregate_sample.PCIeNSWr);
            cout << "      " << unit_format(aggregate_sample.PCIeWiLF);
            cout << "     " << unit_format(aggregate_sample.PCIeItoM);
            cout << "     " << unit_format(aggregate_sample.PCIeNSWr);
            cout << "     " << unit_format(aggregate_sample.PCIeNSWrF);
            if(print_bandwidth)
            {
                cout << "        " << unit_format((aggregate_sample.PCIePRd+ aggregate_sample.PCIeRdCur+ aggregate_sample.PCIeNSWr)*64ULL);
                cout << "         " << unit_format((aggregate_sample.PCIeWiLF+aggregate_sample.PCIeItoM+aggregate_sample.PCIeNSWr+aggregate_sample.PCIeNSWrF)*64ULL);
            }
            cout << "\n\n";
        }
    }

    m->cleanup();

    return 0;
}

void getPCIeEvents(PCM *m, PCM::PCIeEventCode opcode, uint32 delay_ms, PCIeEvents_t *sample)
{
    PCIeCounterState * before = new PCIeCounterState[m->getNumSockets()];
    PCIeCounterState * after = new PCIeCounterState[m->getNumSockets()];
    uint32 i;

    m->programPCIeCounters(opcode);
    for(i=0; i<m->getNumSockets(); ++i)
        before[i] = m->getPCIeCounterState(i);
    MySleepUs(delay_ms*1000);
    for(i=0; i<m->getNumSockets(); ++i)
        after[i] = m->getPCIeCounterState(i);

    for(i=0; i<m->getNumSockets(); ++i)
    {
        switch(opcode)
        {
            case PCM::PCIePRd:
                sample[i].PCIePRd += (sizeof(PCIeEvents_t)/sizeof(uint64)) * getNumberOfEvents(before[i], after[i]);
                aggregate_sample.PCIePRd += sample[i].PCIePRd;
                break;
            case PCM::PCIeRdCur:
                sample[i].PCIeRdCur += (sizeof(PCIeEvents_t)/sizeof(uint64)) * getNumberOfEvents(before[i], after[i]);
                aggregate_sample.PCIeRdCur += sample[i].PCIeRdCur;
                break;
            case PCM::PCIeNSRd:
                sample[i].PCIeNSRd += (sizeof(PCIeEvents_t)/sizeof(uint64)) * getNumberOfEvents(before[i], after[i]);
                aggregate_sample.PCIeNSRd += sample[i].PCIeNSRd;
                break;
            case PCM::PCIeWiLF:
                sample[i].PCIeWiLF += (sizeof(PCIeEvents_t)/sizeof(uint64)) * getNumberOfEvents(before[i], after[i]);
                aggregate_sample.PCIeWiLF += sample[i].PCIeWiLF;
                break;
            case PCM::PCIeItoM:
                sample[i].PCIeItoM += (sizeof(PCIeEvents_t)/sizeof(uint64)) * getNumberOfEvents(before[i], after[i]);
                aggregate_sample.PCIeItoM += sample[i].PCIeItoM;
                break;
            case PCM::PCIeNSWr:
                sample[i].PCIeNSWr += (sizeof(PCIeEvents_t)/sizeof(uint64)) * getNumberOfEvents(before[i], after[i]);
                aggregate_sample.PCIeNSWr += sample[i].PCIeNSWr;
                break;
            case PCM::PCIeNSWrF:
                sample[i].PCIeNSWrF += (sizeof(PCIeEvents_t)/sizeof(uint64)) * getNumberOfEvents(before[i], after[i]);
                aggregate_sample.PCIeNSWrF += sample[i].PCIeNSWrF;
                break;
        }
    }

    delete[] before;
    delete[] after;
}
