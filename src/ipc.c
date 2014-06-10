/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"

#define NUM_EVENTS 3


SEXP papi_ipc_on()
{
  float ireal_time, iproc_time, ipc;
  long long iflpins;
  int retval;
  
  SEXP RET;
  PROTECT(RET = allocVector(INTSXP, 1));
  
  retval = PAPI_ipc(&ireal_time, &iproc_time, &iflpins, &ipc);
  
  if (retval != PAPI_OK)
    INTEGER(RET)[0] = PBD_ERROR;
  
  UNPROTECT(1);
  return RET;
}



SEXP papi_ipc_off()
{
  float real_time, proc_time, ipc;
  long long ins;
  int retval;
  int unpt;
  long_long values[NUM_EVENTS];
  
  SEXP RET;
  
  
  retval = PAPI_ipc(&real_time, &proc_time, &ins, &ipc);
  
  PAPI_stop_counters(values, NUM_EVENTS);
  
  RET = R_papi_setret(retval, real_time, proc_time, ins, "ins", ipc, "ipc");
  
  return RET;
}


