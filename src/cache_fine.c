#include "pbdPAPI.h"


#define NUM_EVENTS 6


SEXP papi_cache_fine_on()
{ 
  int events[NUM_EVENTS] = {PAPI_L1_DCM, PAPI_L1_ICM, PAPI_L2_DCM, PAPI_L2_ICM, PAPI_L3_DCM, PAPI_L3_ICM};
  int retval;
  
  SEXP RET;
  PROTECT(RET = allocVector(INTSXP, 1));
  
  retval = PAPI_start_counters(events, NUM_EVENTS);
  
  if (retval != PAPI_OK)
    INTEGER(RET)[0] = PBD_ERROR;
  
  UNPROTECT(1);
  return RET;
}



SEXP papi_cache_fine_off()
{ 
  int retval, unpt;
  long_long values[NUM_EVENTS];
  
  SEXP RET, RET_NAMES;
  SEXP L1D, L1I, L2D, L2I, L3D, L3I;
  
  
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
    PROTECT(L1D = allocVector(REALSXP, 1));
    PROTECT(L1I = allocVector(REALSXP, 1));
    PROTECT(L2D = allocVector(REALSXP, 1));
    PROTECT(L2I = allocVector(REALSXP, 1));
    PROTECT(L3D = allocVector(REALSXP, 1));
    PROTECT(L3I = allocVector(REALSXP, 1));
    
    REAL(L1D)[0] = (double) values[0];
    REAL(L1I)[0] = (double) values[1];
    REAL(L2D)[0] = (double) values[2];
    REAL(L2I)[0] = (double) values[3];
    REAL(L3D)[0] = (double) values[4];
    REAL(L3I)[0] = (double) values[5];
    
    // Fill list
    PROTECT(RET = allocVector(VECSXP, 3));
    PROTECT(RET_NAMES = allocVector(STRSXP, 3));
    
    SET_VECTOR_ELT(RET, 0, L1D);
    SET_VECTOR_ELT(RET, 1, L1I);
    SET_VECTOR_ELT(RET, 2, L2D);
    SET_VECTOR_ELT(RET, 3, L2I);
    SET_VECTOR_ELT(RET, 4, L3D);
    SET_VECTOR_ELT(RET, 5, L3I);
    
    SET_STRING_ELT(RET_NAMES, 0, mkChar("L1.data"));
    SET_STRING_ELT(RET_NAMES, 1, mkChar("L1.instruction"));
    SET_STRING_ELT(RET_NAMES, 2, mkChar("L2.data"));
    SET_STRING_ELT(RET_NAMES, 3, mkChar("L2.instruction"));
    SET_STRING_ELT(RET_NAMES, 4, mkChar("L3.data"));
    SET_STRING_ELT(RET_NAMES, 5, mkChar("L3.instruction"));
    
    setAttrib(RET, R_NamesSymbol, RET_NAMES);
    
    unpt = NUM_EVENTS + 2;
  }
  
  
  UNPROTECT(unpt);
  return RET;
}


