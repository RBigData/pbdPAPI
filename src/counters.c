/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"


SEXP papi_counter_on()
{
  int events[2] = {PAPI_L1_DCM, PAPI_FP_OPS }, ret;

  SEXP RET;
  PROTECT(RET = allocVector(INTSXP, 1));

  retval = PAPI_start_counters(events, 2);

  if (retval < PAPI_OK)
    INTEGER(RET)[0] = PBD_ERROR;

  UNPROTECT(1);
  return RET;
}
