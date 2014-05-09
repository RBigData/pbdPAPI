#ifndef __PBDPAPI_H__                                                    
#define __PBDPAPI_H__


#include <R.h>
#include <Rinternals.h>
#include "papi.h"

#define PBD_ERROR -1
#define PBD_OK 0

SEXP R_papi_error(int retval);


#endif
