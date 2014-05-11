papi.flips <- function(expr)
{
  papi.init()
  papi.check(3L)
  
  ret <- .Call("papi_flips_on")
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call("papi_flips_off")
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}


system.flips <- function(expr, gcFirst=TRUE)
{
  if (gcFirst) 
    gc(FALSE)
  
  return( papi.flips(expr=expr) )
}
