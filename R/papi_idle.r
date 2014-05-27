system.idle <- function(expr, events="float")
{
  events <- match.arg(tolower(events), c("float", "int", "numeric", "branch", "load", "all"))
  
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
  
  ret <- papi.event(expr=expr, events=events)
  
  return( ret )
}

