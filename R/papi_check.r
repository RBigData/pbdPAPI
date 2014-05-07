papi.ncounters <- function(n=1)
{
  ret <- .Call("papi_check_counters", as.integer(n))
  
  if (ret == -2L)
    return( NA )
  else
    return( ret )
}



papi.check <- function(n=1)
{
  ret <- .Call("papi_check_counters", as.integer(n))
  
  if (ret == -2L)
    stop("No hardware counters available")
  else if (ret == -1L)
    stop("Insufficient hardware counters for operation")
  else
    return( ret )
}


