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


/*!     \file pcm-tsx.cpp
  \brief Example of using CPU counters: implements a performance counter monitoring utility for Intel Transactional Synchronization Extensions
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

struct TSXEvent
{
    const char * name;
    unsigned char event;
    unsigned char umask;
    const char * description;
};

TSXEvent eventDefinition[] = {

{"RTM_RETIRED.START",                                      0xC9, 0x01,  "Number of times an RTM execution started."},
{"RTM_RETIRED.COMMIT",                                     0xC9, 0x02,  "Number of times an RTM execution successfully committed"},
{"RTM_RETIRED.ABORTED",                                    0xC9, 0x04,  "Number of times an RTM execution aborted due to any reasons (multiple categories may count as one)"},
{"RTM_RETIRED.ABORTED_MISC1",                              0xC9, 0x08,  "Number of times an RTM execution aborted due to various memory events"},
{"RTM_RETIRED.ABORTED_MISC2",                              0xC9, 0x10,  "Number of times an RTM execution aborted due to uncommon conditions"},
{"RTM_RETIRED.ABORTED_MISC3",                              0xC9, 0x20,  "Number of times an RTM execution aborted due to HLE-unfriendly instructions"},
{"RTM_RETIRED.ABORTED_MISC4",                              0xC9, 0x40,  "Number of times an RTM execution aborted due to incompatible memory type"},
{"RTM_RETIRED.ABORTED_MISC5",                              0xC9, 0x80,  "Number of times an RTM execution aborted due to none of the previous 4 categories (e.g. interrupt)"},

{"HLE_RETIRED.START",                                      0xC8, 0x01,  "Number of times an HLE execution started."},
{"HLE_RETIRED.COMMIT",                                     0xC8, 0x02,  "Number of times an HLE execution successfully committed"},
{"HLE_RETIRED.ABORTED",                                    0xC8, 0x04,  "Number of times an HLE execution aborted due to any reasons (multiple categories may count as one)"},
{"HLE_RETIRED.ABORTED_MISC1",                              0xC8, 0x08,  "Number of times an HLE execution aborted due to various memory events"},
{"HLE_RETIRED.ABORTED_MISC2",                              0xC8, 0x10,  "Number of times an HLE execution aborted due to uncommon conditions"},
{"HLE_RETIRED.ABORTED_MISC3",                              0xC8, 0x20,  "Number of times an HLE execution aborted due to HLE-unfriendly instructions"},
{"HLE_RETIRED.ABORTED_MISC4",                              0xC8, 0x40,  "Number of times an HLE execution aborted due to incompatible memory type"},
{"HLE_RETIRED.ABORTED_MISC5",                              0xC8, 0x80,  "Number of times an HLE execution aborted due to none of the previous 4 categories (e.g. interrupt)"},

{"TX_MEM.ABORT_CONFLICT",                                  0x54, 0x01,  "Number of times a transactional abort was signaled due to a data conflict on a transactionally accessed address"},
{"TX_MEM.ABORT_CAPACITY_WRITE",                            0x54, 0x02,  "Number of times a transactional abort was signaled due to limited resources for transactional stores"},
{"TX_MEM.ABORT_HLE_STORE_TO_ELIDED_LOCK",                  0x54, 0x04,  "Number of times a HLE transactional region aborted due to a non XRELEASE prefixed instruction writing to an elided lock in the elision buffer"},
{"TX_MEM.ABORT_HLE_ELISION_BUFFER_NOT_EMPTY",              0x54, 0x08,  "Number of times an HLE transactional execution aborted due to NoAllocatedElisionBuffer being nonzero."},
{"TX_MEM.ABORT_HLE_ELISION_BUFFER_MISMATCH",               0x54, 0x10,  "Number of times an HLE transactional execution aborted due to XRELEASE lock not satisfying the address and value requirements in the elision buffer."},
{"TX_MEM.ABORT_HLE_ELISION_BUFFER_UNSUPPORTED_ALIGNMENT",  0x54, 0x20,  "Number of times an HLE transactional execution aborted due to an unsupported read alignment from the elision buffer."},
{"TX_MEM.HLE_ELISION_BUFFER_FULL",                         0x54, 0x40,  "Number of times HLE lock could not be elided due to ElisionBufferAvailable being zero."},
                                                       
{"TX_EXEC.MISC1",                                          0x5D, 0x01,  "Counts the number of times a class of instructions that may cause a transactional abort was executed. Since this is the count of execution, it may not always cause a transactional abort."},
{"TX_EXEC.MISC2",                                          0x5D, 0x02,  "Counts the number of times a class of instructions that may cause a transactional abort was executed inside a transactional region"},
{"TX_EXEC.MISC3",                                          0x5D, 0x04,  "Counts the number of times an instruction execution caused the nest count supported to be exceeded"},
{"TX_EXEC.MISC4",                                          0x5D, 0x08,  "Counts the number of times an HLE XACQUIRE instruction was executed inside an RTM transactional region"}

};

void print_usage(const char * progname)
{
      std::cout << "\nUsage "<<progname<<" (delay | \"external_program\") [-C] [-e event1 ] [-e event2 ] [-e event3 ] [-e event4 ]\n\n";
      std::cout << "  <delay>            - delay in seconds between updates. Either delay or \"external program\" parameters must be supplied\n";
      std::cout << "  \"external_program\" - start external program and print the performance metrics for the execution at the end\n";
      std::cout << "  -C             - output in csv format (optional)\n";
      std::cout << "  -e eventX      - monitor custom TSX event (up to 4) - optional. List of supported events: \n\n";
      for(uint32 i=0; i< sizeof(eventDefinition)/sizeof(TSXEvent); ++i)
      {
          cout << eventDefinition[i].name << "\t"<<eventDefinition[i].description<< endl;
      }
      cout << endl;
}

template <class StateType>
void print_basic_stats(const StateType & BeforeState, const StateType & AfterState, bool csv)
{
        uint64 cycles = getCycles(BeforeState, AfterState);
        uint64 instr = getInstructionsRetired(BeforeState, AfterState);
        const uint64 TXcycles = getNumberOfCustomEvents(3, BeforeState, AfterState);
        const uint64 TXcycles_commited = getNumberOfCustomEvents(2, BeforeState, AfterState);
        const uint64 Abr_cycles = (TXcycles > TXcycles_commited)?(TXcycles-TXcycles_commited):0ULL;
        uint64 nRTM = getNumberOfCustomEvents(0, BeforeState, AfterState);
        uint64 nHLE = getNumberOfCustomEvents(1, BeforeState, AfterState);

        if(csv)
        {
            cout << double(instr)/double(cycles) << ",";
            cout << instr << ",";
            cout << cycles << ",";
            cout << TXcycles << ","<< std::setw(5) <<100.*double(TXcycles)/double(cycles) << "%,";
            cout << Abr_cycles << ","<< std::setw(5) <<100.*double(Abr_cycles)/double(cycles) << "%,";
            cout << nRTM << ",";
            cout << nHLE << ",";
        }
        else
        {
            cout << double(instr)/double(cycles) << "       ";
            cout << unit_format(instr) << "     ";
            cout << unit_format(cycles) << "      ";
            cout << unit_format(TXcycles) << " ("<< std::setw(5) <<100.*double(TXcycles)/double(cycles) << "%)       ";
            cout << unit_format(Abr_cycles) << " ("<< std::setw(5) <<100.*double(Abr_cycles)/double(cycles) << "%) ";
            cout << unit_format(nRTM) << "   ";
            cout << unit_format(nHLE) << "    ";
        }

        if(nRTM + nHLE)
        {
           uint64 cyclesPerTransaction = TXcycles/(nRTM + nHLE);
           if(csv)
             cout << cyclesPerTransaction<< "\n";
           else
             cout << unit_format(cyclesPerTransaction)<< "\n";
        }
        else
            cout << " N/A"<< "\n";

}

template <class StateType>
void print_custom_stats(const StateType & BeforeState, const StateType & AfterState ,bool csv)
{
        for(int i=0;i<4;++i)
           if(!csv)
                cout << unit_format(getNumberOfCustomEvents(i, BeforeState, AfterState)) << "    ";
           else
                cout << getNumberOfCustomEvents(i, BeforeState, AfterState)<<",";

        cout << "\n";
}



int findEvent(const char * name)
{
    const int all = sizeof(eventDefinition)/sizeof(TSXEvent);
    for(int i=0; i<all; ++i)
    {
        if(strcmp(name,eventDefinition[i].name) == 0)
            return i;
    }
    return -1;
}

int main(int argc, char * argv[])
{
#ifdef PCM_FORCE_SILENT
    null_stream nullStream1, nullStream2;
    std::cout.rdbuf(&nullStream1);
    std::cerr.rdbuf(&nullStream2);
#endif

    cout << endl;
    cout << " Intel(r) Performance Counter Monitor: Intel(r) Transactional Synchronization Extensions Monitoring Utility "<< endl;
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
    std::vector<int> events;
    int cur_event;
    bool csv = false;

    int my_opt = -1;
    while ((my_opt = getopt(argc, argv, "e:cC")) != -1)
    {
           switch(my_opt)
           {
                   case 'e':
                       if(events.size() >= 4)
                       {
                           cout << "At most 4 events are allowed"<< endl;
                           return -1;
                       }
                       cur_event = findEvent(optarg);
                       if(cur_event < 0)
                       {
                          cout << "Event "<<optarg<<" is not supported. See the list of supported events"<< endl;
                          print_usage(argv[0]);
                          return -1;
                       }
                       events.push_back(cur_event);
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
    EventSelectRegister regs[4];
    conf.gpCounterCfg = regs;
    for(int i=0;i<4;++i)
        regs[i] = def_event_select_reg;

    if(events.empty())
    {
        regs[0].fields.event_select = 0xc9;
        regs[0].fields.umask = 0x01;
        regs[1].fields.event_select = 0xc8;
        regs[1].fields.umask = 0x01;
        regs[2].fields.event_select = 0x3c;
        regs[2].fields.in_tx = 1;
        regs[2].fields.in_txcp = 1;
        regs[3].fields.event_select = 0x3c;
        regs[3].fields.in_tx = 1;
    }
    else
    {
        for(unsigned int i=0;i<events.size();++i)
        {
            regs[i].fields.event_select = eventDefinition[events[i]].event;
            regs[i].fields.umask = eventDefinition[events[i]].umask;
        }
    }

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

        if(events.empty())
        {
            if(csv)
                cout << "Core,IPC,Instructions,Cycles,Transactional Cycles,Aborted Cycles,#RTM,#HLE,Cycles/Transaction \n";
            else
                cout << "Core | IPC  | Instructions | Cycles  | Transactional Cycles | Aborted Cycles  | #RTM  | #HLE  | Cycles/Transaction \n";
        }
        else
        {
            for(uint32 i=0;i<events.size();++i)
            {
                cout <<"Event"<<i<<": "<<eventDefinition[events[i]].name<<" "<<eventDefinition[events[i]].description<<" (raw 0x"<<
                     std::hex << (uint32)eventDefinition[events[i]].umask << (uint32)eventDefinition[events[i]].event << std::dec <<")"<<endl;
            }
            cout << "\n" ;
            if(csv)
                cout << "Core,Event0,Event1,Event2,Event3\n";
            else
                cout << "Core | Event0  | Event1  | Event2  | Event3 \n";
        }
        for(uint32 i = 0; i<ncores ; ++i)
        {
            if(csv)
                cout <<i<<",";
            else
                cout <<" "<< setw(3) << i << "   " << setw(2) ; 
            if(events.empty())
                print_basic_stats(BeforeState[i], AfterState[i], csv);
            else
                print_custom_stats(BeforeState[i], AfterState[i], csv);
        }
        if(csv)
            cout << "*,";
        else
        {
            cout << "-------------------------------------------------------------------------------------------------------------------\n";
            cout << "   *   ";
        }
        if(events.empty())
            print_basic_stats(SysBeforeState, SysAfterState, csv);
        else
            print_custom_stats(SysBeforeState, SysAfterState, csv);

        std::cout << std::endl;

        swap(BeforeTime, AfterTime);
        swap(BeforeState, AfterState);
        swap(SysBeforeState, SysAfterState);

        if(ext_program) break;

    }

    m->cleanup();

    return 0;
}
