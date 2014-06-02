system.ncounters <- function()
{
  ret <- .Call("papi_check_counters", 0L)
  
  if (ret == -2L)
    return( 0L )
  else
    return( ret )
}



papi.check.ncounters <- function(n=1)
{
  ret <- .Call("papi_check_counters", as.integer(n))
  
  if (ret == -2L)
    stop("No hardware counters available")
  else if (ret == -1L)
    stop("Insufficient hardware counters for operation")
  else
    return( ret )
}


