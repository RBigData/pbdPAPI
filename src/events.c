#include "pbdPAPI.h"

#define NUM_EVENTS LENGTH(which)

#define CHARPT(x,i)	((char*)CHAR(STRING_ELT(x,i)))
#define SETLISTVAR(RVAR,i,RNAME) PROTECT(RVAR=allocVector(REALSXP,1));REAL(RVAR)[0]=(double) values[i];SET_VECTOR_ELT(RET, i, RVAR);SET_STRING_ELT(RET_NAMES, i, mkChar(RNAME))

#define EM(id,rn) {id, #id, rn}

struct eventmap_s{
  int id;
  const char *iname;
  const char *oname;
};

const static struct eventmap_s eventmap[]={
  EM(PAPI_L1_DCM, "Level 1 data cache misses"),
  EM(PAPI_L1_ICM, "Level 1 instruction cache misses"),
  EM(PAPI_L2_DCM, "Level 2 data cache misses"),
  EM(PAPI_L2_ICM, "Level 2 instruction cache misses"),
  EM(PAPI_L3_DCM, "Level 3 data cache misses"),
  EM(PAPI_L3_ICM, "Level 3 instruction cache misses"),
  EM(PAPI_L1_TCM, "Level 1 total cache misses"),
  EM(PAPI_L2_TCM, "Level 2 total cache misses"),
  EM(PAPI_L3_TCM, "Level 3 total cache misses"),
  EM(PAPI_CA_SNP, "Snoops"),
  EM(PAPI_CA_SHR, "Request for access to shared cache line (SMP)"),
  EM(PAPI_CA_CLN, "Request for access to clean cache line (SMP)"),
  EM(PAPI_CA_INV, "Cache Line Invalidation (SMP)"),
  EM(PAPI_CA_ITV, "Cache Line Intervention (SMP)"),
  EM(PAPI_BRU_IDL, "Cycles branch units are idle"),
  EM(PAPI_FXU_IDL, "Cycles integer units are idle"),
  EM(PAPI_FPU_IDL, "Cycles floating point units are idle"),
  EM(PAPI_LSU_IDL, "Cycles load/store units are idle"),
  EM(PAPI_TLB_DM, "Data translation lookaside buffer misses"),
  EM(PAPI_TLB_IM, "Instruction translation lookaside buffer misses"),
  EM(PAPI_TLB_TL, " Total translation lookaside buffer misses"),
  EM(PAPI_L1_LDM, "Level 1 load misses"),
  EM(PAPI_L1_STM, "Level 1 store misses"),
  EM(PAPI_L2_LDM, "Level 2 load misses"),
  EM(PAPI_L2_STM, "Level 2 store misses"),
  EM(PAPI_L3_LDM, "Level 3 load misses"),
  EM(PAPI_L3_STM, "Level 3 store misses"),
  EM(PAPI_BTAC_M, "BTAC miss"),
  EM(PAPI_PRF_DM, "Prefetch data instruction caused a miss"),
  EM(PAPI_L3_DCH, "Level 3 Data Cache Hit"),
  EM(PAPI_TLB_SD, "Translation lookaside buffer shootdowns (SMP)"),
  EM(PAPI_CSR_FAL, "Failed store conditional instructions"),
  EM(PAPI_CSR_SUC, "Successful store conditional instructions"),
  EM(PAPI_CSR_TOT, "Total store conditional instructions"),
  EM(PAPI_MEM_SCY, "Cycles Stalled Waiting for Memory Access"),
  EM(PAPI_MEM_RCY, "Cycles Stalled Waiting for Memory Read"),
  EM(PAPI_MEM_WCY, "Cycles Stalled Waiting for Memory Write"),
  EM(PAPI_STL_ICY, "Cycles with No Instruction Issue"),
  EM(PAPI_FUL_ICY, "Cycles with Maximum Instruction Issue"),
  EM(PAPI_STL_CCY, "Cycles with No Instruction Completion"),
  EM(PAPI_FUL_CCY, "Cycles with Maximum Instruction Completion"),
  EM(PAPI_HW_INT, "Hardware interrupts"),
  EM(PAPI_BR_UCN, "Unconditional branch instructions executed"),
  EM(PAPI_BR_CN, "Conditional branch instructions executed"),
  EM(PAPI_BR_TKN, "Conditional branch instructions taken"),
  EM(PAPI_BR_NTK, "Conditional branch instructions not taken"),
  EM(PAPI_BR_MSP, "Conditional branch instructions mispredicted"),
  EM(PAPI_BR_PRC, "Conditional branch instructions correctly predicted"),
  EM(PAPI_FMA_INS, "FMA instructions completed"),
  EM(PAPI_TOT_IIS, "Total instructions issued"),
  EM(PAPI_TOT_INS, "Total instructions executed"),
  EM(PAPI_INT_INS, "Integer instructions executed"),
  EM(PAPI_FP_INS, "Floating point instructions executed"),
  EM(PAPI_LD_INS, "Load instructions executed"),
  EM(PAPI_SR_INS, "Store instructions executed"),
  EM(PAPI_BR_INS, "Total branch instructions executed"),
  EM(PAPI_VEC_INS, "Vector/SIMD instructions executed"),
  //EM(PAPI_FLOPS, "Floating Point Instructions executed per second"),
  EM(PAPI_RES_STL, "Cycles processor is stalled on resource"),
  EM(PAPI_FP_STAL, "FP units are stalled"),
  EM(PAPI_TOT_CYC, "Total cycles"),
  //EM(PAPI_IPS, "Instructions executed per second"),
  EM(PAPI_LST_INS, "Total load/store instructions executed"),
  EM(PAPI_SYC_INS, "Synchronization instructions executed"),
  EM(PAPI_L1_DCH, "L1 D Cache Hit"),
  EM(PAPI_L2_DCH, "L2 D Cache Hit"),
  EM(PAPI_L1_DCA, "L1 D Cache Access"),
  EM(PAPI_L2_DCA, "L2 D Cache Access"),
  EM(PAPI_L3_DCA, "L3 D Cache Access"),
  EM(PAPI_L1_DCR, "L1 D Cache Read"),
  EM(PAPI_L2_DCR, "L2 D Cache Read"),
  EM(PAPI_L3_DCR, "L3 D Cache Read"),
  EM(PAPI_L1_DCW, "L1 D Cache Write"),
  EM(PAPI_L2_DCW, "L2 D Cache Write"),
  EM(PAPI_L3_DCW, "L3 D Cache Write"),
  EM(PAPI_L1_ICH, "L1 instruction cache hits"),
  EM(PAPI_L2_ICH, "L2 instruction cache hits"),
  EM(PAPI_L3_ICH, "L3 instruction cache hits"),
  EM(PAPI_L1_ICA, "L1 instruction cache accesses"),
  EM(PAPI_L2_ICA, "L2 instruction cache accesses"),
  EM(PAPI_L3_ICA, "L3 instruction cache accesses"),
  EM(PAPI_L1_ICR, "L1 instruction cache reads"),
  EM(PAPI_L2_ICR, "L2 instruction cache reads"),
  EM(PAPI_L3_ICR, "L3 instruction cache reads"),
  EM(PAPI_L1_ICW, "L1 instruction cache writes"),
  EM(PAPI_L2_ICW, "L2 instruction cache writes"),
  EM(PAPI_L3_ICW, "L3 instruction cache writes"),
  EM(PAPI_L1_TCH, "L1 total cache hits"),
  EM(PAPI_L2_TCH, "L2 total cache hits"),
  EM(PAPI_L3_TCH, "L3 total cache hits"),
  EM(PAPI_L1_TCA, "L1 total cache accesses"),
  EM(PAPI_L2_TCA, "L2 total cache accesses"),
  EM(PAPI_L3_TCA, "L3 total cache accesses"),
  EM(PAPI_L1_TCR, "L1 total cache reads"),
  EM(PAPI_L2_TCR, "L2 total cache reads"),
  EM(PAPI_L3_TCR, "L3 total cache reads"),
  EM(PAPI_L1_TCW, "L1 total cache writes"),
  EM(PAPI_L2_TCW, "L2 total cache writes"),
  EM(PAPI_L3_TCW, "L3 total cache writes"),
  EM(PAPI_FML_INS, "FM ins"),
  EM(PAPI_FAD_INS, "FA ins"),
  EM(PAPI_FDV_INS, "FD ins"),
  EM(PAPI_FSQ_INS, "FSq ins"),
  EM(PAPI_FNV_INS, "Finv ins"),
  {-1,NULL,NULL}
};

const static struct eventmap_s* eventfind_iname(const char *iname){
  int i;
  
  for(i=0;eventmap[i].iname!=NULL;i++){
    if(strcmp(iname,eventmap[i].iname)==0)
      return eventmap+i;
  }
  
  return NULL;
}

const static struct eventmap_s* eventfind_id(const int id){
  int i;
  
  for(i=0;eventmap[i].iname!=NULL;i++){
    if(id==eventmap[i].id)
      return eventmap+i;
  }
  
  return NULL;
}



SEXP papi_event_counter_init(SEXP which)
{
  SEXP vec;
  int i;
  const int num = NUM_EVENTS;
  const struct eventmap_s *ev;
  
  vec = PROTECT(allocVector(INTSXP,num));
  
  for (i=0; i<num; i++){
    ev=eventfind_iname(CHARPT(which, i));
    if(ev==NULL){
      UNPROTECT(1);
      return R_papi_error(PAPI_ENOEVNT); // Should we make a custom error?
    }
    INTEGER(vec)[i]=ev->id;
  }
  
  UNPROTECT(1);
  
  return vec;
}



SEXP papi_event_counter_on(SEXP which)
{
  SEXP ret;
  int retval;
  int *events;
  int i;
  const int num = NUM_EVENTS;
  
  events=INTEGER(which);
  retval = PAPI_start_counters(events, num);
  
  ret = R_papi_error(retval);
  
  return ret;
}



SEXP papi_event_counter_off(SEXP which)
{
  int retval, unpt;
  int i;
  const int num = NUM_EVENTS;
  long_long values[NUM_EVENTS];
  const struct eventmap_s *ev;
  
  SEXP RET, RET_NAMES;
  SEXP *svals;
  
  svals=malloc(sizeof(*svals)*num);
  
  retval = PAPI_stop_counters(values, num);
  
  if (retval != PAPI_OK)
  {
    PROTECT(RET = allocVector(INTSXP, 1));
    INTEGER(RET)[0] = PBD_ERROR;
    
    unpt = 1;
  }
  else
  {
    PROTECT(RET = allocVector(VECSXP, num));
    PROTECT(RET_NAMES = allocVector(STRSXP, num));
    
    for (i=0; i<num; i++)
    {
      ev=eventfind_id(INTEGER(which)[i]);
      SETLISTVAR(svals[i],i,ev->oname);
    }
    
    setAttrib(RET, R_NamesSymbol, RET_NAMES);
    
    unpt = num + 2;
  }
  
  
  UNPROTECT(unpt);
  
  free(svals);
  
  return RET;
}


