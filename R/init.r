### Make sure PAPI linked with pbdPAPI at compile time is the same as that linked at run time
papi.init <- function()
{
  ret <- .Call("papi_init", PACKAGE="pbdPAPI")
  
  if (ret != 0L)
    stop("Could not initialize PAPI; compile time and run time versions disagree")
  else
    invisible( ret )
}



### Internal checker for number of hw counters
papi.check.ncounters <- function(n=1)
{
  ret <- .Call(papi_check_counters, as.integer(n))
  
  if (ret == -2L)
    stop("No hardware counters available")
  else if (ret == -1L)
    stop("Insufficient hardware counters for operation")
  else
    return( ret )
}

