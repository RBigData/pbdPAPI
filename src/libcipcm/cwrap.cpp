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
}global_state;

struct event_list_s{
	uint64(*event_function)(const SystemCounterState&,const SystemCounterState&);
	int atom;
};

static const struct event_list_s event_list[]={
	{getL2CacheMisses,1},
	{getL2CacheHits,1},
	{getInstructionsRetired,1},
	{getCycles,1},
	{getL3CacheHitsNoSnoop,0},
	{getL3CacheHitsSnoop,0},
	{getL3CacheHits,0},
	{getL3CacheMisses,0},
};

static int global_cpu_model=0;
static long long global_nominal_frequency=0;
static int global_max_cpus=0;

long long ipcm_get_frequency(){
	return global_nominal_frequency;
}

int ipcm_get_cpus(){
	return global_max_cpus;
}

void ipcm_get_cpuid(int *family, int *model){
	*family=global_state.m->getCPUFamily();
	*model=global_state.m->getCPUModel();
}

void ipcm_cpu_strings(char *vendor, char *model, char *codename){
	int i;
	const char *tmp = global_state.m->getCPUBrandString().c_str();

	for(i=0;tmp[i] && tmp[i]!=' ';i++);

	memcpy(vendor,tmp,i);

	tmp+=i+1;
	strcpy(model,tmp);

	tmp=global_state.m->getUArchCodename();
	strcpy(codename,tmp);
}

int ipcm_event_avail(const int code){
	if(code>=IPCM_START_EVENT && code<IPCM_NULL_EVENT){
		if(global_cpu_model==PCM::ATOM)
			return event_list[code].atom;
		else
			return 1;
	}

	return 0;
}

static void set_diff_vals(PCM *m, const SystemCounterState *sstate1, const SystemCounterState *sstate2, const int cpu_model, ipcm_event_val_t *values, const int num){
	int i;

	for(i=0;i<num;i++){
		//if(values[i].code>=IPCM_START_EVENT && values[i].code<IPCM_NULL_EVENT){
		if(ipcm_event_avail(values[i].code))
			values[i].val=event_list[values[i].code].event_function(*sstate1,*sstate2);
		else
			values[i].val=-1;
	}
}

static void print_diff(
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

int ipcm_init(){
	//struct CSysCounterState *ret;
#ifdef PCM_FORCE_SILENT
	streambuf *oldout, *olderr;
	null_stream nullStream1, nullStream2;
	oldout=std::cout.rdbuf();
	olderr=std::cerr.rdbuf();
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
	/*
	signal(SIGPIPE, cleanup);
	signal(SIGINT, cleanup);
	signal(SIGKILL, cleanup);
	signal(SIGTERM, cleanup);
	*/
#endif

	// interesting start

	int tryagain=0;
	PCM * m = PCM::getInstance();
	//if(disable_JKT_workaround) m->disableJKTWorkaround();
	PCM::ErrorCode status = m->program();
	PCM::ErrorCode secondstatus;
	switch (status)
	{
		case PCM::Success:
			break;
		case PCM::MSRAccessDenied:
			cerr << "Access to Intel(r) Performance Counter Monitor has denied (no MSR or PCI CFG space access)." << endl;
			return 0;
		case PCM::PMUBusy:
			cerr << "Access to Intel(r) Performance Counter Monitor has denied (Performance Monitoring Unit is occupied by other application). Try to stop the application that uses PMU." << endl;
			cerr << "Alternatively you can try to reset PMU configuration at your own risk. Try to reset? (y/n)" << endl;
			//char yn;
			//std::cin >> yn;
			//if ('y' == yn)
			//{
				m->resetPMU();
				cout << "PMU configuration has been reset. Try to rerun the program again." << endl;
			//}
			//return 0;
				m = PCM::getInstance();
				secondstatus=m->program();
				if(secondstatus!=PCM::Success)
					return 0;
				break;
		default:
			cerr << "Access to Intel(r) Performance Counter Monitor has denied (Unknown error)." << endl;
			return 0;
	}

	//global_state=(struct CSysCounterState*)malloc(sizeof(*ret));
	global_state.m=m;
	global_cpu_model=m->getCPUModel();
	global_nominal_frequency=m->getNominalFrequency();
	global_max_cpus=m->getNumCores();

#ifdef PCM_FORCE_SILENT
	std::cout.rdbuf(oldout);
	std::cerr.rdbuf(olderr);
#endif

	return 1;
}

void ipcm_get_events(){
	std::vector<CoreCounterState> cstates1, cstates2;
	std::vector<SocketCounterState> sktstate1, sktstate2;
	//SystemCounterState sstate1, sstate2;
	uint64 TimeAfterSleep = 0;

	global_state.m->getAllCounterStates(global_state.start, sktstate1, cstates1);

	//return ret;
}

void ipcm_end_events(ipcm_event_val_t *values, const int num){
	std::vector<CoreCounterState> cstates1, cstates2;
	std::vector<SocketCounterState> sktstate1, sktstate2;
	//struct CSysCounterState *sa=(struct CSysCounterState*)state;

	global_state.m->getAllCounterStates(global_state.end, sktstate2, cstates2);

	//print_diff(sa->m,&sa->start,&sa->end,sa->cpu_model);
	set_diff_vals(global_state.m,&global_state.start,&global_state.end,global_state.cpu_model,values,num);

	//free(state);
}
