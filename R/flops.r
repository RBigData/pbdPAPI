papi.flops <- function(expr)
{
  papi.check.ncounters(3L)
  
  ret <- .Call("papi_flops_on")
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call("papi_flops_off")
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}


system.flops <- function(expr, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    b <- system.flops(gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    gc(FALSE)
  
  if (missing(expr))
    expr <- NULL
  
  events <- "PAPI_FP_INS"
  papi.avail.lookup(events=events, shorthand=FALSE)
  
  ret <- papi.flops(expr=expr)
  
  if (burnin)
  {
    for (i in 3:length(ret))
      ret[[i]] <- max(ret[[i]] - b[[i]], 0)
  }
  
  return( ret )
}
