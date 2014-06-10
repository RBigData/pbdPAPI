/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"

#define NUM_EVENTS 3

int _hl_rate_calls(float *real_time, float *proc_time, int *events, long long *values, long long *ins, float *rate, int mode);

#define HL_STOP		0
#define HL_START	1
#define HL_FLIP		2
#define HL_FLOP		3
#define HL_IPC		4
#define HL_EPC		5
#define HL_READ		6
#define HL_ACCUM	7

int R_proc_utilization(float *rtime, float *ptime, long long *util)
{
  int retval;
  int events = PAPI_FP_INS;
  long long values = 0;
  
  if (rtime == NULL || ptime == NULL || util == NULL)
    return PAPI_EINVAL;
  
  retval = _hl_rate_calls(rtime, ptime, &events, &values, util, mflips, HL_FLIP );
  
  return retval;
}


SEXP papi_flops_on()
{
  float ireal_time, iproc_time, imflops;
  long long iflpops;
  int retval;

  SEXP RET;
  PROTECT(RET = allocVector(INTSXP, 1));

  retval = PAPI_flops(&ireal_time, &iproc_time, &iflpops, &imflops);

  if (retval != PAPI_OK)
    INTEGER(RET)[0] = PBD_ERROR;

  UNPROTECT(1);
  return RET;
}



SEXP papi_flops_off()
{
  float real_time, proc_time, mflops;
  long long flpops;
  int retval;
  int unpt;
  long_long values[NUM_EVENTS];

  SEXP RET, RET_NAMES, REAL_TIME, PROC_TIME, FLPOPS, MFLOPS;


  retval = PAPI_flops(&real_time, &proc_time, &flpops, &mflops);

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
    PROTECT(FLPOPS = allocVector(REALSXP, 1));
    PROTECT(MFLOPS = allocVector(REALSXP, 1));

    REAL(REAL_TIME)[0] = (double) real_time;
    REAL(PROC_TIME)[0] = (double) proc_time;
    REAL(FLPOPS)[0] = (double) flpops;
    REAL(MFLOPS)[0] = (double) mflops;

    // Fill list
    PROTECT(RET = allocVector(VECSXP, 4));
    PROTECT(RET_NAMES = allocVector(STRSXP, 4));

    SET_VECTOR_ELT(RET, 0, REAL_TIME);
    SET_VECTOR_ELT(RET, 1, PROC_TIME);
    SET_VECTOR_ELT(RET, 2, FLPOPS);
    SET_VECTOR_ELT(RET, 3, MFLOPS);

    SET_STRING_ELT(RET_NAMES, 0, mkChar("real_time"));
    SET_STRING_ELT(RET_NAMES, 1, mkChar("proc_time"));
    SET_STRING_ELT(RET_NAMES, 2, mkChar("flpops"));
    SET_STRING_ELT(RET_NAMES, 3, mkChar("mflops"));

    setAttrib(RET, R_NamesSymbol, RET_NAMES);

    unpt = 6;
  }


  // Turn off counters
  PAPI_stop_counters(values, NUM_EVENTS);

  UNPROTECT(unpt);
  return RET;
}


