/*
   Copyright (c) 2009-2013, Intel Corporation
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Intel Corporation nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// written by Roman Dementiev


/*!     \file pcm-numa.cpp
  \brief Example of using CPU counters: implements a performance counter monitoring utility for NUMA (remote and local memory accesses counting). Example for programming offcore response events 
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

#include <vector>

using namespace std;

void print_usage(const char * progname)
{
      std::cout << "\nUsage "<<progname<<" (delay | \"external_program\") [-C] \n\n";
      std::cout << "  <delay>            - delay in seconds between updates. Either delay or \"external program\" parameters must be supplied\n";
      std::cout << "  \"external_program\" - start external program and print the performance metrics for the execution at the end\n";
      std::cout << "  -C             - output in csv format (optional)\n";
      cout << endl;
}

template <class StateType>
void print_stats(const StateType & BeforeState, const StateType & AfterState, bool csv)
{
        uint64 cycles = getCycles(BeforeState, AfterState);
        uint64 instr = getInstructionsRetired(BeforeState, AfterState);

        if(csv)
        {
            cout << double(instr)/double(cycles) << ",";
            cout << instr << ",";
            cout << cycles << ",";
        }
        else
        {
            cout << double(instr)/double(cycles) << "       ";
            cout << unit_format(instr) << "     ";
            cout << unit_format(cycles) << "      ";
        }

        for(int i=0;i<2;++i)
           if(!csv)
                cout << unit_format(getNumberOfCustomEvents(i, BeforeState, AfterState)) << "              ";
           else
                cout << getNumberOfCustomEvents(i, BeforeState, AfterState)<<",";

        cout << "\n";
}


int main(int argc, char * argv[])
{
#ifdef PCM_FORCE_SILENT
    null_stream nullStream1, nullStream2;
    std::cout.rdbuf(&nullStream1);
    std::cerr.rdbuf(&nullStream2);
#endif

    cout << endl;
    cout << " Intel(r) Performance Counter Monitor: NUMA monitoring utility "<< endl;
    cout << endl;
    cout << " Copyright (c) 2013 Intel Corporation" << endl;
    cout << endl;
#ifdef _MSC_VER
    // Increase the priority a bit to improve context switching delays on Windows
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

    TCHAR driverPath[1024];
    GetCurrentDirectory(1024, driverPath);
    wcscat(driverPath, L"\\msr.sys");

    SetConsoleCtrlHandler((PHANDLER_ROUTINE)cleanup, TRUE);
#else
    signal(SIGPIPE, cleanup);
    signal(SIGINT, cleanup);
    signal(SIGKILL, cleanup);
    signal(SIGTERM, cleanup);
#endif

    int delay = -1;
    char * ext_program = NULL;
    bool csv = false;

    int my_opt = -1;
    while ((my_opt = getopt(argc, argv, "e:cC")) != -1)
    {
           switch(my_opt)
           {
                   case 'e':
                       break;
                   case 'C':
                   case 'c':
                        csv = true;
                        break;
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
     if(delay == 0)
         ext_program = argv[optind];
     else
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

    EventSelectRegister def_event_select_reg;
    def_event_select_reg.value = 0;
    def_event_select_reg.fields.usr = 1;
    def_event_select_reg.fields.os = 1;
    def_event_select_reg.fields.enable = 1;

    PCM::ExtendedCustomCoreEventDescription conf;
    conf.fixedCfg = NULL; // default
    conf.nGPCounters = 4;
    switch(m->getCPUModel())
    {
        case PCM::WESTMERE_EX:
            conf.OffcoreResponseMsrValue[0] = 0x40FF; // OFFCORE_RESPONSE.ANY_REQUEST.LOCAL_DRAM:  Offcore requests satisfied by the local DRAM
            conf.OffcoreResponseMsrValue[1] = 0x20FF; // OFFCORE_RESPONSE.ANY_REQUEST.REMOTE_DRAM: Offcore requests satisfied by a remote DRAM
            break;
        default:
            cout << "pcm-numa tool does not support your processor currently." << endl;
            return -1;
    }
    EventSelectRegister regs[4];
    conf.gpCounterCfg = regs;
    for(int i=0;i<4;++i)
        regs[i] = def_event_select_reg;

    regs[0].fields.event_select = 0xB7; // OFFCORE_RESPONSE 0 event
    regs[0].fields.umask = 0x01;
    regs[1].fields.event_select = 0xBB; // OFFCORE_RESPONSE 1 event
    regs[1].fields.umask = 0x01;

    PCM::ErrorCode status = m->program(PCM::EXT_CUSTOM_CORE_EVENTS, &conf);
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

    uint64 BeforeTime = 0, AfterTime = 0;
    SystemCounterState SysBeforeState, SysAfterState;
    const uint32 ncores = m->getNumCores();
    std::vector<CoreCounterState> BeforeState, AfterState;
    std::vector<SocketCounterState> DummySocketStates;
    cout << "Update every "<<delay<<" seconds"<< endl;

    BeforeTime = m->getTickCount();
    m->getAllCounterStates(SysBeforeState, DummySocketStates, BeforeState);

    std::cout.precision(2);
    std::cout << std::fixed; 

    while(1)
    {
#ifdef _MSC_VER
        int delay_ms = delay * 1000;
        // compensate slow Windows console output
        if(AfterTime) delay_ms -= (int)(m->getTickCount() - BeforeTime);
        if(delay_ms < 0) delay_ms = 0;
#else
        int delay_ms = delay * 1000;
#endif

        if(ext_program)
            MySystem(ext_program);
        else
            MySleepMs(delay_ms);

        AfterTime = m->getTickCount();
        m->getAllCounterStates(SysAfterState, DummySocketStates, AfterState);

        cout << "Time elapsed: "<<dec<<fixed<<AfterTime-BeforeTime<<" ms\n";
        //cout << "Called sleep function for "<<dec<<fixed<<delay_ms<<" ms\n";

        if(csv)
            cout << "Core,IPC,Instructions,Cycles,Local DRAM accesses,Remote DRAM accesses \n";
        else
            cout << "Core | IPC  | Instructions | Cycles  |  Local DRAM accesses | Remote DRAM Accesses \n";

        for(uint32 i = 0; i<ncores ; ++i)
        {
            if(csv)
                cout <<i<<",";
            else
                cout <<" "<< setw(3) << i << "   " << setw(2) ;

            print_stats(BeforeState[i], AfterState[i], csv);
        }

        
        if(csv)
            cout << "*,";
        else
        {
            cout << "-------------------------------------------------------------------------------------------------------------------\n";
            cout << "   *   ";
        }

        print_stats(SysBeforeState, SysAfterState, csv);

        std::cout << std::endl;

        swap(BeforeTime, AfterTime);
        swap(BeforeState, AfterState);
        swap(SysBeforeState, SysAfterState);

        if(ext_program) break;

    }

    m->cleanup();

    return 0;
}
