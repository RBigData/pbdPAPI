/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"

#define NUM_EVENTS 1

SEXP papi_utilization_on()
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



SEXP papi_utilization_off()
{
  float real_time, proc_time, ipc;
  long long ins;
  int retval;
  int unpt;
  long_long values[NUM_EVENTS];
  
  PAPI_option_t opts;
  int clockrate, ncpus;
  
  SEXP RET, RET_NAMES, REAL_TIME, PROC_TIME, INS, IPC, UTILIZATION;
  
  retval = PAPI_ipc(&real_time, &proc_time, &ins, &ipc);
  
  PAPI_stop_counters(values, NUM_EVENTS);
  
  clockrate = PAPI_get_opt(PAPI_CLOCKRATE, &opts);
  ncpus = PAPI_get_opt(PAPI_MAX_CPUS, &opts);
  
  if (retval != PAPI_OK)
  {
    PROTECT(RET = allocVector(INTSXP, 1));
    INTEGER(RET)[0] = PBD_ERROR;
  
    unpt = 1;
  }
  else
  {
    // Fill return values
    PROTECT(REAL_TIME = allocVector(REALSXP, 1));
    PROTECT(PROC_TIME = allocVector(REALSXP, 1));
    PROTECT(INS = allocVector(REALSXP, 1));
    PROTECT(IPC = allocVector(REALSXP, 1));
    PROTECT(UTILIZATION = allocVector(REALSXP, 1));
    
    REAL(REAL_TIME)[0] = (double) real_time;
    REAL(PROC_TIME)[0] = (double) proc_time;
    REAL(INS)[0] = (double) ins;
    REAL(IPC)[0] = (double) ipc;
    REAL(UTILIZATION)[0] = ((double) ins) / ( ((double) clockrate * 1e6) * real_time) / ((double) ncpus);
    
    // Fill list
    PROTECT(RET = allocVector(VECSXP, 5));
    PROTECT(RET_NAMES = allocVector(STRSXP, 5));
    
    SET_VECTOR_ELT(RET, 0, REAL_TIME);
    SET_VECTOR_ELT(RET, 1, PROC_TIME);
    SET_VECTOR_ELT(RET, 2, INS);
    SET_VECTOR_ELT(RET, 3, IPC);
    SET_VECTOR_ELT(RET, 4, UTILIZATION);
    
    SET_STRING_ELT(RET_NAMES, 0, mkChar("real_time"));
    SET_STRING_ELT(RET_NAMES, 1, mkChar("proc_time"));
    SET_STRING_ELT(RET_NAMES, 2, mkChar("ins"));
    SET_STRING_ELT(RET_NAMES, 3, mkChar("ipc"));
    SET_STRING_ELT(RET_NAMES, 4, mkChar("utilization"));
    
    setAttrib(RET, R_NamesSymbol, RET_NAMES);
    
    unpt = 7;
  }
  
  
  UNPROTECT(unpt);
  return RET;
}


