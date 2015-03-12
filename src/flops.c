/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"

/***********************************************************************
 * if PAPI_FP_OPS is a derived event in your platform, then your       *
 * platform must have at least three counters to support PAPI_flops,   *
 * because PAPI needs one counter to cycles. So in UltraSparcIII, even *
 * the platform supports PAPI_FP_OPS, but UltraSparcIII only has  two  *
 * available hardware counters and PAPI_FP_OPS is a derived event in   *
 * this platform, so PAPI_flops returns an error.                      *
 ***********************************************************************/

#define NUM_EVENTS 3

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
  long_long values[NUM_EVENTS];

  SEXP RET;


  retval = PAPI_flops(&real_time, &proc_time, &flpops, &mflops);

  PAPI_stop_counters(values, NUM_EVENTS);

  RET = R_papi_setret(retval, real_time, proc_time, flpops, "flpops", mflops, "mflops");

  return RET;
}



