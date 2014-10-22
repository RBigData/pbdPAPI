/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"

#define NUM_EVENTS 3

SEXP R_papi_setret(int retval, float real_time, float proc_time, long long x, char *xlab, float y, char *ylab)
{
  int unpt;
  SEXP RET, RET_NAMES, REAL_TIME, PROC_TIME, X, Y;

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
    PROTECT(X = allocVector(REALSXP, 1));
    PROTECT(Y = allocVector(REALSXP, 1));

    REAL(REAL_TIME)[0] = (double) real_time;
    REAL(PROC_TIME)[0] = (double) proc_time;
    REAL(X)[0] = (double) x;
    REAL(Y)[0] = (double) y;

    // Fill list
    PROTECT(RET = allocVector(VECSXP, 4));
    PROTECT(RET_NAMES = allocVector(STRSXP, 4));

    SET_VECTOR_ELT(RET, 0, REAL_TIME);
    SET_VECTOR_ELT(RET, 1, PROC_TIME);
    SET_VECTOR_ELT(RET, 2, X);
    SET_VECTOR_ELT(RET, 3, Y);

    SET_STRING_ELT(RET_NAMES, 0, mkChar("real_time"));
    SET_STRING_ELT(RET_NAMES, 1, mkChar("proc_time"));
    SET_STRING_ELT(RET_NAMES, 2, mkChar(xlab));
    SET_STRING_ELT(RET_NAMES, 3, mkChar(ylab));

    setAttrib(RET, R_NamesSymbol, RET_NAMES);

    unpt = 6;
  }

  UNPROTECT(unpt);
  return RET;
}

