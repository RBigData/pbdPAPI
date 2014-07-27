#include <string.h>
#include <stdlib.h>
#include <papi.h>
#include "cwrap.h"

ipcm_event_val_t *ipcm_val;
static int global_is_init=0;

#define EM(id,rn) {id, #id, rn}

struct eventmap_s{
	int id;
	const char *iname;
	const char *oname;
};

const static struct eventmap_s eventmap[]={
	EM(IPCM_L2_TCM,"L2 cache misses"),
	EM(IPCM_L2_TCH,"L2 cache hits"),
	EM(IPCM_INS_RET,"Instructions retired"),
	EM(IPCM_CYC,"Cycles used"),
	EM(IPCM_L3_TCH_NS,"L3 cache hits without snooping on L2 cache"),
	EM(IPCM_L3_TCH_S,"L3 cache hits with snooping on L2 cache"),
	EM(IPCM_L3_TCH,"L3 cache hits"),
	EM(IPCM_L3_TCM,"L3 cache misses"),
	{-1,NULL,NULL}
};

const static struct eventmap_s* eventfind_iname(const char *iname){
	int i;

	for(i=0;eventmap[i].iname!=NULL;i++){
		if(strcmp(iname,eventmap[i].iname)==0)
			return eventmap+i;
	}

	return NULL;
}

const static struct eventmap_s* eventfind_id(const int id){
	int i;

	for(i=0;eventmap[i].iname!=NULL;i++){
		if(id==eventmap[i].id)
			return eventmap+i;
	}

	return NULL;
}

int PAPI_event_name_to_code(char *name, int *code){
	const struct eventmap_s *ret;

	ret=eventfind_iname(name);

	if(ret){
		*code=ret->id;
		return PAPI_OK;
	}

	return PAPI_ENOEVNT;
}

int PAPI_get_event_info(int code, PAPI_event_info_t *ev){ /* ev.(count|symbol|long_descr) */
	const struct eventmap_s *ret;

	ret=eventfind_id(code);

	if(ret){
		ev->count=ipcm_event_avail(code);
		strcpy(ev->symbol,ret->iname);
		strcpy(ev->long_descr,ret->oname);
		return PAPI_OK;
	}

	return PAPI_ENOEVNT;
}

int PAPI_enum_event(int *id,int mod){
	if(mod==PAPI_ENUM_FIRST){
		*id=0;
		return PAPI_OK;
	}

	if(mod==PAPI_ENUM_EVENTS){
		(*id)++;
		if(*id<IPCM_NULL_EVENT)
			return PAPI_OK;
	}

	/* TODO: PAPI_PRESET_ENUM_AVAIL */

	return PAPI_EINVAL;
}

int PAPI_num_counters(){
	return IPCM_NULL_EVENT; /* Does IPCM have a counter limit? */
}

int PAPI_start_counters(int *codes,int num){
	ipcm_val=(ipcm_event_val_t*)malloc(sizeof(*ipcm_val)*num);
	ipcm_get_events();

	return PAPI_OK;
}

int PAPI_stop_counters(int *values,int num){
	int i;

	ipcm_end_events(ipcm_val,num);

	for(i=0;i<num;i++)
		values[i]=ipcm_val[i].val;

	free(ipcm_val);

	//state=NULL;
	ipcm_val=NULL;

	return PAPI_OK;
}

int PAPI_epc(int event, float *rtime, float *ptime, long long *ref, long long *core, long long *evt, float *epc){ /* dummy */
	return PAPI_ENOEVNT;
}

int PAPI_flips(float *rtime, float *ptime, long long * flpins, float *mflips){ /* dummy */
	return PAPI_ENOEVNT;
}

int PAPI_flops(float *rtime, float *ptime, long long * flpops, float *mflops){ /* dummy */
	return PAPI_ENOEVNT;
}

const PAPI_hw_info_t* PAPI_get_hardware_info(){
	return NULL; /* TODO: get numeric codes for family/model or have alternate R call for full string? */
}

int PAPI_get_opt(int option, PAPI_option_t *ptr){ /* PAPI_CLOCKRATE, PAPI_MAX_CPUS */
	switch(option){
		case PAPI_CLOCKRATE:
			return ipcm_get_frequency();
		case PAPI_MAX_CPUS:
			return ipcm_get_cpus();
	}
	return PAPI_ESYS;
}

int PAPI_is_initialized(){
	return global_is_init;
}

int PAPI_library_init(int version){ /* PAPI_VER_CURRENT for both I and O */
	if((global_is_init=ipcm_init()))
		return PAPI_VER_CURRENT;
	return PAPI_ESYS;
}

int PAPI_ipc(float *rtime, float *ptime, long long * ins, float *ipc){ /* dummy */
	return PAPI_ENOEVNT;
}
