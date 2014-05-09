#include "pbdPAPI.h"

#define L1DCM 0
#define L1ICM 1
#define L2DCM 2
#define L2ICM 3
#define L3DCM 4
#define L3ICM 5
#define L1TOT 6
#define L2TOT 7
#define L3TOT 8

#define NUM_EVENTS LENGTH(which)

SEXP papi_cache_misses_on(SEXP which)
{
  SEXP RET;
  int retval;
  int events[NUM_EVENTS];
  int i;
  
  
  for (i=0; i<NUM_EVENTS; i++)
  {
    switch (INTEGER(which)[i])
    {
      // L1
      case L1DCM :
        events[i] = PAPI_L1_DCM;
        break;
      case L1ICM :
        events[i] = PAPI_L1_ICM;
        break;
      // L2
      case L2DCM :
        events[i] = PAPI_L2_DCM;
        break;
      case L2ICM :
        events[i] = PAPI_L2_ICM;
        break;
      // L3
      case L3DCM :
        events[i] = PAPI_L3_DCM;
        break;
      case L3ICM :
        events[i] = PAPI_L3_ICM;
        break;
      // Totals
      case L1TOT :
        events[i] = PAPI_L1_TCM;
        break;
      case L2TOT :
        events[i] = PAPI_L2_TCM;
        break;
      case L3TOT :
        events[i] = PAPI_L3_TCM;
        break;
    }
  }
  
  
  retval = PAPI_start_counters(events, NUM_EVENTS);
  
  RET = R_papi_error(retval);
  
  return RET;
}



SEXP papi_cache_misses_off(SEXP which)
{ 
  int retval, unpt;
  int i;
  long_long values[NUM_EVENTS];
  
  SEXP RET, RET_NAMES;
  SEXP L1D, L1I, L2D, L2I, L3D, L3I, L1T, L2T, L3T;
  
  
  retval = PAPI_stop_counters(values, NUM_EVENTS);
  
  if (retval != PAPI_OK)
  {
    PROTECT(RET = allocVector(INTSXP, 1));
    INTEGER(RET)[0] = PBD_ERROR;
    
    unpt = 1;
  }
  else
  {
    PROTECT(RET = allocVector(VECSXP, NUM_EVENTS));
    PROTECT(RET_NAMES = allocVector(STRSXP, NUM_EVENTS));
    
    for (i=0; i<NUM_EVENTS; i++)
    {
      switch (INTEGER(which)[i])
      {
        // L1
        case L1DCM :
          PROTECT(L1D = allocVector(REALSXP, 1));
          REAL(L1D)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L1D);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L1.data"));
          break;
        case L1ICM :
          PROTECT(L1I = allocVector(REALSXP, 1));
          REAL(L1I)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L1I);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L1.instruction"));
          break;
        // L2
        case L2DCM :
          PROTECT(L2D = allocVector(REALSXP, 1));
          REAL(L2D)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L2D);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L2.data"));
          break;
        case L2ICM :
          PROTECT(L2I = allocVector(REALSXP, 1));
          REAL(L2I)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L2I);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L2.instruction"));
          break;
        // L3
        case L3DCM :
          PROTECT(L3D = allocVector(REALSXP, 1));
          REAL(L3D)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L3D);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L3.data"));
          break;
        case L3ICM :
          PROTECT(L3I = allocVector(REALSXP, 1));
          REAL(L3I)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L3I);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L3.instruction"));
          break;
        // Totals
        case L1TOT :
          PROTECT(L1T = allocVector(REALSXP, 1));
          REAL(L1T)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L1T);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L1.total"));
          break;
        case L2TOT :
          PROTECT(L2T = allocVector(REALSXP, 1));
          REAL(L2T)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L2T);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L2.total"));
          break;
        case L3TOT :
          PROTECT(L3T = allocVector(REALSXP, 1));
          REAL(L3T)[0] = (double) values[i];
          SET_VECTOR_ELT(RET, i, L3T);
          SET_STRING_ELT(RET_NAMES, i, mkChar("L3.total"));
          break;
      }
    }
    
    setAttrib(RET, R_NamesSymbol, RET_NAMES);
    
    unpt = NUM_EVENTS + 2;
  }
  
  
  UNPROTECT(unpt);
  return RET;
}


