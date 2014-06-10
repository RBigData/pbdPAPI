papi.epc <- function(expr, event)
{
  papi.check.ncounters(3L)
  
  ret <- .Call("papi_epc_on", event)
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call("papi_epc_off", event)
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}


system.epc <- function(expr, event, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    system.epc(event=event, gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    gc(FALSE)
  
  if (missing(expr))
    expr <- NULL
  
  papi.avail.lookup(events=event, shorthand=FALSE)
  event.num <- papi.event.init(which=event)
  
  ret <- papi.epc(expr=expr, event=event.num)
  
  return( ret )
}
