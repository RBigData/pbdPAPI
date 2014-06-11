system.stall <- function(expr, events="float", gcFirst=TRUE, burnin=TRUE)
{
  events <- match.arg(tolower(events), c("float", "int", "numeric", "branch", "load", "all"))
  
  if (missing(expr))
    expr <- NULL
  
  shorthand <- events
  
  if (events == "float")
    events <- "PAPI_FPU_IDL"
  else if (events == "int")
    events <- "PAPI_FXU_IDL"
  else if (events == "numeric")
    events <- c("PAPI_FPU_IDL", "PAPI_FXU_IDL")
  else if (events == "branch")
    events <- "PAPI_BRU_IDL"
  else if (events == "load")
    events <- "PAPI_LSU_IDL"
  else if (events == "all")
    events <- c("PAPI_FPU_IDL", "PAPI_FXU_IDL", "PAPI_BRU_IDL", "PAPI_LSU_IDL")
  
  papi.avail.lookup(events=events, shorthand=shorthand)
  
  ret <- system.event(expr=expr, events=events, gcFirst=gcFirst, burnin=burnin)
  
  return( ret )
}

