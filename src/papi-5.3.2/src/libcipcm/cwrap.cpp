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

#include "cwrap.h"

using namespace std;
#define SIZE (10000000)
#define DELAY 1 // in seconds

struct CSysCounterState{
	PCM *m;
	SystemCounterState start;
	SystemCounterState end;
	int cpu_model;
};

void print_diff(
		PCM *m,
		const SystemCounterState *sstate1,
		const SystemCounterState *sstate2,
		const int cpu_model){
	if (cpu_model != PCM::ATOM)
	{
		cout << " TOTAL  *     " << getExecUsage(*sstate1, *sstate2) <<
			"   " << getIPC(*sstate1, *sstate2) <<
			"   " << getRelativeFrequency(*sstate1, *sstate2) <<
			"    " << getActiveRelativeFrequency(*sstate1, *sstate2) <<
			"    " << unit_format(getL3CacheMisses(*sstate1, *sstate2)) <<
			"   " << unit_format(getL2CacheMisses(*sstate1, *sstate2)) <<
			"    " << getL3CacheHitRatio(*sstate1, *sstate2) <<
			"    " << getL2CacheHitRatio(*sstate1, *sstate2) <<
			"    " << getCyclesLostDueL3CacheMisses(*sstate1, *sstate2) <<
			"    " << getCyclesLostDueL2CacheMisses(*sstate1, *sstate2);
		if (!(m->memoryTrafficMetricsAvailable()))
			cout << "     N/A     N/A";
		else
			cout << "    " << getBytesReadFromMC(*sstate1, *sstate2) / double(1024ULL * 1024ULL * 1024ULL) <<
				"    " << getBytesWrittenToMC(*sstate1, *sstate2) / double(1024ULL * 1024ULL * 1024ULL);
		cout << "     N/A\n";
	}
	else
		cout << " TOTAL  *     " << getExecUsage(*sstate1, *sstate2) <<
			"   " << getIPC(*sstate1, *sstate2) <<
			"   " << getRelativeFrequency(*sstate1, *sstate2) <<
			"   " << unit_format(getL2CacheMisses(*sstate1, *sstate2)) <<
			"    " << getL2CacheHitRatio(*sstate1, *sstate2) <<
			"     N/A\n";
}

void* get_events(){
	struct CSysCounterState *ret;
	#ifdef PCM_FORCE_SILENT
	null_stream nullStream1, nullStream2;
	std::cout.rdbuf(&nullStream1);
	std::cerr.rdbuf(&nullStream2);
	#endif

#ifdef _MSC_VER
	// Increase the priority a bit to improve context switching delays on Windows
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

	TCHAR driverPath[1040]; // length for current directory + "\\msr.sys"
	GetCurrentDirectory(1024, driverPath);
	wcscat_s(driverPath, 1040, L"\\msr.sys");

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)cleanup, TRUE);
#else
	signal(SIGPIPE, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGKILL, cleanup);
	signal(SIGTERM, cleanup);
#endif

	// interesting start

	PCM * m = PCM::getInstance();
	//if(disable_JKT_workaround) m->disableJKTWorkaround();
	PCM::ErrorCode status = m->program();
	switch (status)
	{
		case PCM::Success:
			break;
		case PCM::MSRAccessDenied:
			cerr << "Access to Intel(r) Performance Counter Monitor has denied (no MSR or PCI CFG space access)." << endl;
			return NULL;
		case PCM::PMUBusy:
			cerr << "Access to Intel(r) Performance Counter Monitor has denied (Performance Monitoring Unit is occupied by other application). Try to stop the application that uses PMU." << endl;
			cerr << "Alternatively you can try to reset PMU configuration at your own risk. Try to reset? (y/n)" << endl;
			char yn;
			std::cin >> yn;
			if ('y' == yn)
			{
				m->resetPMU();
				cout << "PMU configuration has been reset. Try to rerun the program again." << endl;
			}
			return NULL;
		default:
			cerr << "Access to Intel(r) Performance Counter Monitor has denied (Unknown error)." << endl;
			return NULL;
	}

	ret=(struct CSysCounterState*)malloc(sizeof(*ret));
	ret->m=m;
	ret->cpu_model=m->getCPUModel();

	std::vector<CoreCounterState> cstates1, cstates2;
	std::vector<SocketCounterState> sktstate1, sktstate2;
	//SystemCounterState sstate1, sstate2;
	uint64 TimeAfterSleep = 0;

	m->getAllCounterStates(ret->start, sktstate1, cstates1);

	return ret;
}
void end_events(void *state){
	std::vector<CoreCounterState> cstates1, cstates2;
	std::vector<SocketCounterState> sktstate1, sktstate2;
	struct CSysCounterState *sa=(struct CSysCounterState*)state;

	sa->m->getAllCounterStates(sa->end, sktstate2, cstates2);

	print_diff(sa->m,&sa->start,&sa->end,sa->cpu_model);

	free(state);
}
