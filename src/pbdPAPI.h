#ifndef __PBDPAPI_H__
#define __PBDPAPI_H__


#include <R.h>
#include <Rinternals.h>
#include <papi.h>

#define PBD_ERROR -1
#define PBD_OK 0

SEXP R_papi_error(int retval);
SEXP R_papi_setret(int retval, float real_time, float proc_time, long long x, char *xlab, float y, char *ylab);


#endif
