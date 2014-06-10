papi.ipc <- function(expr)
{
  papi.check.ncounters(3L)
  
  ret <- .Call("papi_ipc_on")
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call("papi_ipc_off")
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}


system.ipc <- function(expr, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    system.ipc(gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    gc(FALSE)
  
  if (missing(expr))
    expr <- NULL
  
  events <- c("PAPI_TOT_INS", "PAPI_TOT_CYC")
  papi.avail.lookup(events=events, shorthand=FALSE)
  
  ret <- papi.ipc(expr=expr)
  
  return( ret )
}
