/*  Taken from $PAPI/src/examples/high_level.c  */

#include "pbdPAPI.h"

#include <stdio.h>
#include <stdlib.h>

#define NUM_EVENTS 2
#define THRESHOLD 10000
#define ERROR_RETURN(retval) { fprintf(stderr, "Error %d %s:line %d: \n", retval,__FILE__,__LINE__);  exit(retval); }

/* stupid codes to be monitored */
void computation_mult()
{
  double tmp=1.0;
  int i=1;
  for( i = 1; i < THRESHOLD; i++ )
  {
    tmp = tmp*i;
  }
}

/* stupid codes to be monitored */
void computation_add()
{
  int tmp = 0;
  int i=0;

  for( i = 0; i < THRESHOLD; i++ )
  {
    tmp = tmp + i;
  }

}


SEXP papi_example()
{
  /*Declaring and initializing the event set with the presets*/
  int Events[2] = {PAPI_TOT_INS, PAPI_TOT_CYC};
  /*The length of the events array should be no longer than the
    value returned by PAPI_num_counters.*/

  /*declaring place holder for no of hardware counters */
  int num_hwcntrs = 0;
  int retval;
  char errstring[PAPI_MAX_STR_LEN];
  /*This is going to store our list of results*/
  long long values[NUM_EVENTS];


  /***************************************************************************
  *  This part initializes the library and compares the version number of the*
  * header file, to the version of the library, if these don't match then it *
  * is likely that PAPI won't work correctly.If there is an error, retval    *
  * keeps track of the version number.                                       *
  ***************************************************************************/

  #if 0
  if((retval = PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT )
  {
    fprintf(stderr, "Error: %d %s\n",retval, errstring);
    exit(1);
  }
  #endif


  /**************************************************************************
   * PAPI_num_counters returns the number of hardware counters the platform *
   * has or a negative number if there is an error                          *
   **************************************************************************/
  if ((num_hwcntrs = PAPI_num_counters()) < PAPI_OK)
  {
    Rprintf("There are no counters available. \n");
/*    exit(1);*/
    return R_NilValue;
  }

  Rprintf("There are %d counters in this system\n",num_hwcntrs);

  /**************************************************************************
   * PAPI_start_counters initializes the PAPI library (if necessary) and    *
   * starts counting the events named in the events array. This function    *
   * implicitly stops and initializes any counters running as a result of   *
   * a previous call to PAPI_start_counters.                                *
   **************************************************************************/

  if ( (retval = PAPI_start_counters(Events, NUM_EVENTS)) != PAPI_OK)
    return R_NilValue;
/*    ERROR_RETURN(retval);*/

  Rprintf("\nCounter Started: \n");

  /* Your code goes here*/
  computation_add();



  /**********************************************************************
   * PAPI_read_counters reads the counter values into values array      *
   **********************************************************************/

  if ( (retval=PAPI_read_counters(values, NUM_EVENTS)) != PAPI_OK)
    return R_NilValue;
/*    ERROR_RETURN(retval);*/

  Rprintf("Read successfully\n");



  Rprintf("The total instructions executed for addition are %lld \n",values[0]);
  Rprintf("The total cycles used are %lld \n", values[1] );

  Rprintf("\nNow we try to use PAPI_accum to accumulate values\n");

  /* Do some computation here */
  computation_add();


  /************************************************************************
   * What PAPI_accum_counters does is it adds the running counter values  *
   * to what is in the values array. The hardware counters are reset and  *
   * left running after the call.                                         *
   ************************************************************************/

  if ( (retval=PAPI_accum_counters(values, NUM_EVENTS)) != PAPI_OK)
    return R_NilValue;
/*    ERROR_RETURN(retval);*/

  Rprintf("We did an additional %d times addition!\n", THRESHOLD);
  Rprintf("The total instructions executed for addition are %lld \n",
          values[0] );
  Rprintf("The total cycles used are %lld \n", values[1] );

  /***********************************************************************
   * Stop counting events(this reads the counters as well as stops them  *
   ***********************************************************************/

  Rprintf("\nNow we try to do some multiplications\n");
  computation_mult();

  /******************* PAPI_stop_counters **********************************/
  if ((retval=PAPI_stop_counters(values, NUM_EVENTS)) != PAPI_OK)
    return R_NilValue;
/*    ERROR_RETURN(retval);*/

  Rprintf("The total instruction executed for multiplication are %lld \n",
          values[0] );
  Rprintf("The total cycles used are %lld \n", values[1] );
  
  
  return R_NilValue;
}
