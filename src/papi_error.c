#include "pbdPAPI.h"

SEXP R_papi_error(int retval)
{
  SEXP RET;

  if (retval == PAPI_OK)
  {
    PROTECT(RET = allocVector(INTSXP, 1));
    INTEGER(RET)[0] = 0;
  }
  else
  {
    PROTECT(RET = allocVector(STRSXP, 1));

    switch (retval)
    {
      case PAPI_EINVAL :
        SET_STRING_ELT(RET, 0, mkChar("internal error: invalid argument to PAPI; please report this to pbdPAPI developers"));
        break;
      case PAPI_ENOMEM :
        SET_STRING_ELT(RET, 0, mkChar("insufficient memory"));
        break;
      case PAPI_ESYS :
        SET_STRING_ELT(RET, 0, mkChar("internal error: a library call failed; please report this to pbdPAPI developers"));
        break;
      case PAPI_ESBSTR :
        SET_STRING_ELT(RET, 0, mkChar("substrate returned an error"));
        break;
      case PAPI_ECLOST :
        SET_STRING_ELT(RET, 0, mkChar("access to counters was lost or interrupted"));
        break;
      case PAPI_EBUG :
        SET_STRING_ELT(RET, 0, mkChar("internal papi error: please report to PAPI developers"));
        break;
      case PAPI_ENOEVNT :
        SET_STRING_ELT(RET, 0, mkChar("hardware event does not exist"));
        break;
      case PAPI_ECNFLCT :
        SET_STRING_ELT(RET, 0, mkChar("hardware event exists, but cannot be counted due to counter resource limitations"));
        break;
      case PAPI_ENOTRUN :
        SET_STRING_ELT(RET, 0, mkChar("no events or event sets are currently not counting"));
        break;
      case PAPI_EISRUN :
        SET_STRING_ELT(RET, 0, mkChar("event set currently running"));
        break;
      case PAPI_ENOEVST :
        SET_STRING_ELT(RET, 0, mkChar("internal error: no such event available; please report this to pbdPAPI developers"));
        break;
      case PAPI_ENOTPRESET :
        SET_STRING_ELT(RET, 0, mkChar("internal error: event is not a valid preset; please report this to pbdPAPI developers"));
        break;
      case PAPI_ENOCNTR :
        SET_STRING_ELT(RET, 0, mkChar("hardware does not support performance counters"));
        break;
      case PAPI_EMISC :
        SET_STRING_ELT(RET, 0, mkChar("papi internal error: 'unknown error'"));
        break;
      default :
        SET_STRING_ELT(RET, 0, mkChar("internal error: unknown error; please report to pbdPAPI developers'"));
        break;
    }
  }

  UNPROTECT(1);
  return RET;
}
