#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "cwrap.h"

int main(){
	ipcm_event_val_t ev[3];
	if(!ipcm_init())
		return 1;

	ev[0].code=IPCM_L3_TCM;
	ev[1].code=IPCM_L2_TCH;
	ev[2].code=IPCM_CYC;

	ipcm_get_events();
	ipcm_end_events(ev,3);

	printf("%lld %lld %lld\n%lld %d\n",ev[0].val,ev[1].val,ev[2].val,ipcm_get_frequency(),ipcm_get_cpus());

	return 0;
}
