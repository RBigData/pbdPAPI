/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Heckendorf and Schmidt.  All rights reserved.

#include "pbdPAPI.h"

#define INT(x) INTEGER(x)[0]

#define SETINTVAR(n,v) SET_STRING_ELT(name,i,mkChar(n));\
	INTEGER(val)[i]=v;\
	i++;

SEXP papi_hwname()
{
  const PAPI_hw_info_t *p_ret;
  SEXP ret,name,val;

  p_ret=PAPI_get_hardware_info();
  if(p_ret==NULL)
  {
    return R_papi_error(PAPI_ESYS);
  }

  PROTECT(ret=allocVector(VECSXP,2));
  PROTECT(name=allocVector(STRSXP,2));
  PROTECT(val=allocVector(STRSXP,2));

  SET_STRING_ELT(name,0,mkChar("vendor"));
  SET_STRING_ELT(name,1,mkChar("model"));

  SET_STRING_ELT(val,0,mkChar(p_ret->vendor_string));
  SET_STRING_ELT(val,1,mkChar(p_ret->model_string));

  SET_VECTOR_ELT(ret,0,name);
  SET_VECTOR_ELT(ret,1,val);

  UNPROTECT(3);

  return ret;
}

SEXP papi_hwinfo()
{
  const PAPI_hw_info_t *p_ret;
  SEXP ret,name,val;
  int i;

  p_ret=PAPI_get_hardware_info();
  if(p_ret==NULL)
  {
    return R_papi_error(PAPI_ESYS);
  }

  PROTECT(ret=allocVector(VECSXP,2));
  PROTECT(name=allocVector(STRSXP,10));
  PROTECT(val=allocVector(INTSXP,10));

  i=0;
  SETINTVAR("cpuid_family",p_ret->cpuid_family);
  SETINTVAR("cpuid_model",p_ret->cpuid_model);
  SETINTVAR("ncpu",p_ret->ncpu);
  SETINTVAR("threads",p_ret->threads);
  SETINTVAR("cores",p_ret->cores);
  SETINTVAR("sockets",p_ret->sockets);
  SETINTVAR("nnodes",p_ret->nnodes);
  SETINTVAR("totalcpus",p_ret->totalcpus);
  SETINTVAR("vendor",p_ret->vendor);
  SETINTVAR("model",p_ret->model);

  SET_VECTOR_ELT(ret,0,name);
  SET_VECTOR_ELT(ret,1,val);

  UNPROTECT(3);

  return ret;
}

SEXP R_papi_cpuinfo()
{
  SEXP ret, retnames;
  PAPI_option_t opts;
  SEXP clockrate, ncpus;
  
  PROTECT(clockrate = allocVector(INTSXP, 1));
  PROTECT(ncpus = allocVector(INTSXP, 1));
  
  INT(clockrate) = PAPI_get_opt(PAPI_CLOCKRATE, &opts);
  INT(ncpus) = PAPI_get_opt(PAPI_MAX_CPUS, &opts);
  
  PROTECT(ret = allocVector(VECSXP, 2));
  PROTECT(retnames = allocVector(STRSXP, 2));
  setAttrib(ret, R_NamesSymbol, retnames);
  
  SET_VECTOR_ELT(ret, 0, clockrate);
  SET_VECTOR_ELT(ret, 1, ncpus);
  
  SET_STRING_ELT(retnames, 0, mkChar("clockrate"));
  SET_STRING_ELT(retnames, 1, mkChar("ncpus"));
  
  UNPROTECT(4);
  return ret;
}




