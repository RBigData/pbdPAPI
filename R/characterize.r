system.cpuormem <- function(expr, gcFirst=TRUE, burnin=TRUE)
{
  if (missing(expr))
    expr <- NULL
  
  events <- c("PAPI_FP_INS", "PAPI_L2_TCA")
  papi.avail.lookup(events=events)
  
  ret <- system.event(expr=expr, events=events, gcFirst=gcFirst)
  
  if (ret[[1L]] >= ret[[2L]])
    ret <- c(ret, "compute bound")
  else 
    ret <- c(ret, "memory bound")
  
  names(ret)[3L] <- "Program Characterization"
  
  return( ret )
}


