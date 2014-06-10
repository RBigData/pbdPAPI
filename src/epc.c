/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"

#define NUM_EVENTS 3

/*PAPI_epc( int event, float *rtime, float *ptime, long long *ref, long long *core, long long *evt, float *epc )*/

SEXP papi_epc_on(SEXP event)
{
  float ireal_time, iproc_time, epc;
  long long ref, core, evt;
  int retval;
  
  SEXP RET;
  PROTECT(RET = allocVector(INTSXP, 1));
  
  retval = PAPI_epc(INTEGER(event)[0], &ireal_time, &iproc_time, &ref, &core, &evt, &epc);
  
  if (retval != PAPI_OK)
    INTEGER(RET)[0] = PBD_ERROR;
  
  UNPROTECT(1);
  return RET;
}



SEXP papi_epc_off(SEXP event)
{
  float ireal_time, iproc_time, epc;
  long long ref, core, evt;
  int retval;
  int unpt;
  long_long values[NUM_EVENTS];
  
  SEXP RET, RET_NAMES, REAL_TIME, PROC_TIME, FLPINS, MFLIPS, REF, CORE;
  
  retval = PAPI_epc(INTEGER(event)[0], &ireal_time, &iproc_time, &ref, &core, &evt, &epc);
  
  PAPI_stop_counters(values, NUM_EVENTS);
  
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
    PROTECT(FLPINS = allocVector(REALSXP, 1));
    PROTECT(MFLIPS = allocVector(REALSXP, 1));
    PROTECT(REF = allocVector(REALSXP, 1));
    PROTECT(CORE = allocVector(REALSXP, 1));
    
    REAL(REAL_TIME)[0] = (double) ireal_time;
    REAL(PROC_TIME)[0] = (double) iproc_time;
    REAL(FLPINS)[0] = (double) evt;
    REAL(MFLIPS)[0] = (double) epc;
    REAL(REF)[0] = (double) ref;
    REAL(CORE)[0] = (double) core;
    
    // Fill list
    PROTECT(RET = allocVector(VECSXP, 6));
    PROTECT(RET_NAMES = allocVector(STRSXP, 6));
    
    SET_VECTOR_ELT(RET, 0, REAL_TIME);
    SET_VECTOR_ELT(RET, 1, PROC_TIME);
    SET_VECTOR_ELT(RET, 2, FLPINS);
    SET_VECTOR_ELT(RET, 3, MFLIPS);
    SET_VECTOR_ELT(RET, 4, REF);
    SET_VECTOR_ELT(RET, 5, CORE);
    
    SET_STRING_ELT(RET_NAMES, 0, mkChar("real_time"));
    SET_STRING_ELT(RET_NAMES, 1, mkChar("proc_time"));
    SET_STRING_ELT(RET_NAMES, 2, mkChar("ref"));
    SET_STRING_ELT(RET_NAMES, 3, mkChar("core"));
    SET_STRING_ELT(RET_NAMES, 4, mkChar("evt"));
    SET_STRING_ELT(RET_NAMES, 5, mkChar("epc"));
    
    setAttrib(RET, R_NamesSymbol, RET_NAMES);
    
    unpt = 8;
  }
  
  UNPROTECT(unpt);
  return RET;
}


