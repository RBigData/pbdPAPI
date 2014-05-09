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

#define CACHEVAR(RVAR,i,RNAME) PROTECT(RVAR=allocVector(REALSXP,1));REAL(RVAR)[0]=(double) values[i];SET_VECTOR_ELT(RET, i, RVAR);SET_STRING_ELT(RET_NAMES, i, mkChar(RNAME))

SEXP papi_cache_misses_on(SEXP which)
{
  SEXP RET;
  int retval;
  int events[NUM_EVENTS];
  int i;
  
  const int papi_cache_miss_vals[9] = {
    PAPI_L1_DCM, PAPI_L1_ICM, 
    PAPI_L2_DCM, PAPI_L2_ICM, 
    PAPI_L3_DCM, PAPI_L3_ICM, 
    PAPI_L1_TCM, PAPI_L2_TCM, PAPI_L3_TCM
  };
  
  for (i=0; i<NUM_EVENTS; i++)
    events[i] = papi_cache_miss_vals[INTEGER(which)[i]];
  
  
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
          CACHEVAR(L1D, i, "L1.data");
          break;
        case L1ICM :
          CACHEVAR(L1I, i, "L1.instruction");
          break;
        // L2
        case L2DCM :
          CACHEVAR(L2D, i, "L2.data");
          break;
        case L2ICM :
          CACHEVAR(L2I, i, "L2.instruction");
          break;
        // L3
        case L3DCM :
          CACHEVAR(L3D, i, "L3.data");
          break;
        case L3ICM :
          CACHEVAR(L3I, i, "L3.instruction");
          break;
        // Totals
        case L1TOT :
          CACHEVAR(L1T, i, "L1.total");
          break;
        case L2TOT :
          CACHEVAR(L2T, i, "L2.total");
          break;
        case L3TOT :
          CACHEVAR(L3T, i, "L3.total");
          break;
      }
    }
    
    setAttrib(RET, R_NamesSymbol, RET_NAMES);
    
    unpt = NUM_EVENTS + 2;
  }
  
  
  UNPROTECT(unpt);
  return RET;
}


