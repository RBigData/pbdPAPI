#include "pbdPAPI.h"


SEXP papi_init(SEXP n)
{
  int retval = PAPI_library_init(PAPI_VER_CURRENT);

  SEXP RET;
  PROTECT(RET = allocVector(INTSXP, 1));

  if (retval != PAPI_VER_CURRENT)
    INTEGER(RET)[0] = -1;
  else
    INTEGER(RET)[0] = 0;


  UNPROTECT(1);
  return RET;
}

