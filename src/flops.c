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


