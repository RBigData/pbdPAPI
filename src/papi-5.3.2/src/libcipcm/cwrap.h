#ifndef IPCM_CWRAP_H
#define IPCM_CWRAP_H

#ifdef __cplusplus
extern "C" {
#endif
	void* get_events();
	void end_events(void *state);
#ifdef __cplusplus
}
#endif

#endif
