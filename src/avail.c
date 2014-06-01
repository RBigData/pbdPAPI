/* This Source Code Form is subject to the terms of the BSD 2-Clause
 * License. If a copy of the this license was not distributed with this
 * file, you can obtain one from http://opensource.org/licenses/BSD-2-Clause. */

// Copyright 2014, Heckendorf.  All rights reserved.


#include "pbdPAPI.h"

#define NUM_EVENTS LENGTH(which)

#define CHARPT(x,i)	((char*)CHAR(STRING_ELT(x,i)))

SEXP papi_event_avail(SEXP which)
{
  SEXP val,name,desc;
  int i;
  int unpt;
  int num;
  const int which_num = NUM_EVENTS;
  SEXP ret;
  int papiret;
  int id;
  PAPI_event_info_t ev;
  char *namep;


  if(which_num>0 && TYPEOF(which)==STRSXP)
  {
    num=which_num;
    PROTECT(ret=allocVector(VECSXP,3));
    PROTECT(name=allocVector(STRSXP,num));
    PROTECT(val=allocVector(LGLSXP,num));
    PROTECT(desc=allocVector(STRSXP,num));
    for (i=0; i<num; i++)
    {
      namep=CHARPT(which,i);
      papiret=PAPI_event_name_to_code(namep,&id);
      if(papiret!=PAPI_OK)
      {
        unpt=i+3;
        UNPROTECT(unpt);
        return R_papi_error(PAPI_ENOEVNT); // Should we make a custom error?
      }

      /* TODO: find out what this returns */
      papiret=PAPI_get_event_info(id,&ev);

      LOGICAL(val)[i]=ev.count>0;
      SET_STRING_ELT(name, i, mkChar(namep));
      SET_STRING_ELT(desc, i, mkChar(ev.long_descr));
    }

    unpt=4;
  }
  else  /* Get all events */
  {
    i=num=0;
    id=0|PAPI_PRESET_MASK;
    PAPI_enum_event(&id,PAPI_ENUM_FIRST);

    do
    {
      num++;
    }
    while(PAPI_enum_event(&id,PAPI_ENUM_EVENTS)==PAPI_OK);  // PAPI_PRESET_ENUM_AVAIL might also be useful

    PROTECT(ret=allocVector(VECSXP,3));
    PROTECT(name=allocVector(STRSXP,num));
    PROTECT(val=allocVector(LGLSXP,num));
    PROTECT(desc=allocVector(STRSXP,num));

    id=0|PAPI_PRESET_MASK;
    PAPI_enum_event(&id,PAPI_ENUM_FIRST);

    do
    {
      papiret=PAPI_get_event_info(id,&ev);

      LOGICAL(val)[i]=ev.count>0;
      SET_STRING_ELT(name, i, mkChar(ev.symbol));
      SET_STRING_ELT(desc, i, mkChar(ev.long_descr));

      i++;
    }
    while(PAPI_enum_event(&id,PAPI_ENUM_EVENTS)==PAPI_OK);  // PAPI_PRESET_ENUM_AVAIL might also be useful

    unpt=4;
  }

  SET_VECTOR_ELT(ret,0,name);
  SET_VECTOR_ELT(ret,1,val);
  SET_VECTOR_ELT(ret,2,desc);

  UNPROTECT(unpt);

  return ret;
}
