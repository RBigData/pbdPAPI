/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"

#define NUM_EVENTS 3

SEXP papi_flips_on()
{
  float ireal_time, iproc_time, imflips;
  long long iflpins;
  int retval;

  SEXP RET;
  PROTECT(RET = allocVector(INTSXP, 1));

  retval = PAPI_flips(&ireal_time, &iproc_time, &iflpins, &imflips);

  if (retval != PAPI_OK)
    INTEGER(RET)[0] = PBD_ERROR;

  UNPROTECT(1);
  return RET;
}



SEXP papi_flips_off()
{
  float real_time, proc_time, mflips;
  long long flpins;
  int retval;
  long_long values[NUM_EVENTS];

  SEXP RET;


  retval = PAPI_flips(&real_time, &proc_time, &flpins, &mflips);

  PAPI_stop_counters(values, NUM_EVENTS);

  RET = R_papi_setret(retval, real_time, proc_time, flpins, "flpins", mflips, "mflips");

  return RET;
}

