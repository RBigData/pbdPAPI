/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Schmidt.  All rights reserved.


#include "pbdPAPI.h"

SEXP papi_init(SEXP n)
{
  int retval;
  SEXP RET;

  if(PAPI_is_initialized())
	retval = PAPI_VER_CURRENT;
  else
	retval = PAPI_library_init(PAPI_VER_CURRENT);

  PROTECT(RET = allocVector(INTSXP, 1));

  if (retval != PAPI_VER_CURRENT)
    INTEGER(RET)[0] = -1;
  else
    INTEGER(RET)[0] = 0;


  UNPROTECT(1);
  return RET;
}

