#ifndef IPCM_CWRAP_H
#define IPCM_CWRAP_H

#ifdef __cplusplus
extern "C" {
#endif

	enum event_codes{
		IPCM_START_EVENT=0,
		IPCM_L2_TCM=0,
		IPCM_L2_TCH,
		IPCM_INS_RET,
		IPCM_CYC,
		IPCM_L3_TCH_NS,
		IPCM_L3_TCH_S,
		IPCM_L3_TCH,
		IPCM_L3_TCM,
		IPCM_NULL_EVENT,
	};

	typedef struct ipcm_event_val_s{
		int code;
		long long val;
	}ipcm_event_val_t;

	int ipcm_init();
	void ipcm_get_events();
	void ipcm_end_events(ipcm_event_val_t *values, const int num);
	int ipcm_event_avail(const int code);
	long long ipcm_get_frequency();
	int ipcm_get_cpus();
	void ipcm_cpu_strings(char *vendor, char *model, char *codename);
	void ipcm_get_cpuid(int *family, int *model);
#ifdef __cplusplus
}
#endif

#endif
