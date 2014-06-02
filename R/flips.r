papi.flips <- function(expr)
{
  papi.check.ncounters(3L)
  
  ret <- .Call("papi_flips_on")
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call("papi_flips_off")
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}


system.flips <- function(expr, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    system.flips(gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    gc(FALSE)
  
  if (missing(expr))
    expr <- NULL
  
  return( papi.flips(expr=expr) )
}
