#include "pbdPAPI.h"


#define NUM_EVENTS 3


SEXP papi_cache_on()
{ 
  int events[NUM_EVENTS] = {PAPI_L1_TCM, PAPI_L2_TCM, PAPI_L3_TCM};
  int retval;
  
  SEXP RET;
  PROTECT(RET = allocVector(INTSXP, 1));
  
  retval = PAPI_start_counters(events, NUM_EVENTS);
  
  if (retval != PAPI_OK)
    INTEGER(RET)[0] = PBD_ERROR;
  
  UNPROTECT(1);
  return RET;
}



SEXP papi_cache_off()
{ 
  int retval, unpt;
  long_long values[NUM_EVENTS];
  
  SEXP RET, RET_NAMES, L1TOT, L2TOT, L3TOT;
  
  
  retval = PAPI_stop_counters(values, NUM_EVENTS);
  
  if (retval != PAPI_OK)
  {    
    PROTECT(RET = allocVector(INTSXP, 1));
    INTEGER(RET)[0] = PBD_ERROR;
    
    unpt = 1;
  }
  else
  {
    // Fill return values
    PROTECT(L1TOT = allocVector(REALSXP, 1));
    PROTECT(L2TOT = allocVector(REALSXP, 1));
    PROTECT(L3TOT = allocVector(REALSXP, 1));
    
    REAL(L1TOT)[0] = (double) values[0];
    REAL(L2TOT)[0] = (double) values[1];
    REAL(L3TOT)[0] = (double) values[2];
    
    // Fill list
    PROTECT(RET = allocVector(VECSXP, 3));
    PROTECT(RET_NAMES = allocVector(STRSXP, 3));
    
    SET_VECTOR_ELT(RET, 0, L1TOT);
    SET_VECTOR_ELT(RET, 1, L2TOT);
    SET_VECTOR_ELT(RET, 2, L3TOT);
    
    SET_STRING_ELT(RET_NAMES, 0, mkChar("L1.total"));
    SET_STRING_ELT(RET_NAMES, 1, mkChar("L2.total"));
    SET_STRING_ELT(RET_NAMES, 2, mkChar("L3.total"));
    
    setAttrib(RET, R_NamesSymbol, RET_NAMES);
    
    unpt = 5;
  }
  
  
  UNPROTECT(unpt);
  return RET;
}


