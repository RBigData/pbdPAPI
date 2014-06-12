papi.utilization <- function(expr)
{
  papi.check.ncounters(3L)
  
  ret <- .Call("papi_utilization_on", PACKAGE="pbdPAPI")
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call("papi_utilization_off", PACKAGE="pbdPAPI")
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}


system.utilization <- function(expr, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    b <- system.utilization(gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    gc(FALSE)
  
  if (missing(expr))
    expr <- NULL
  
  events <- c("PAPI_TOT_INS", "PAPI_TOT_CYC")
  papi.avail.lookup(events=events, shorthand=FALSE)
  
  ret <- papi.utilization(expr=expr)
  
  if (burnin)
  {
    for (i in 3:(length(ret)-1))
      ret[[i]] <- max(ret[[i]] - b[[i]], 0)
  }
  
  return( ret )
}
