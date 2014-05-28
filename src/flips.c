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
  int unpt;
  long_long values[NUM_EVENTS];

  SEXP RET, RET_NAMES, REAL_TIME, PROC_TIME, FLPINS, MFLIPS;


  retval = PAPI_flips(&real_time, &proc_time, &flpins, &mflips);

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

    REAL(REAL_TIME)[0] = (double) real_time;
    REAL(PROC_TIME)[0] = (double) proc_time;
    REAL(FLPINS)[0] = (double) flpins;
    REAL(MFLIPS)[0] = (double) mflips;

    // Fill list
    PROTECT(RET = allocVector(VECSXP, 4));
    PROTECT(RET_NAMES = allocVector(STRSXP, 4));

    SET_VECTOR_ELT(RET, 0, REAL_TIME);
    SET_VECTOR_ELT(RET, 1, PROC_TIME);
    SET_VECTOR_ELT(RET, 2, FLPINS);
    SET_VECTOR_ELT(RET, 3, MFLIPS);

    SET_STRING_ELT(RET_NAMES, 0, mkChar("real_time"));
    SET_STRING_ELT(RET_NAMES, 1, mkChar("proc_time"));
    SET_STRING_ELT(RET_NAMES, 2, mkChar("flpins"));
    SET_STRING_ELT(RET_NAMES, 3, mkChar("mflips"));

    setAttrib(RET, R_NamesSymbol, RET_NAMES);

    unpt = 6;
  }


  // Turn off counters
  PAPI_stop_counters(values, NUM_EVENTS);

  UNPROTECT(unpt);
  return RET;
}


