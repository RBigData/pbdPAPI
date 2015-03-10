#' Idle
#' 
#' Measuring idle cycles.
#' 
#' This function measures the evaluation of the provided expression, expr, for
#' the number of cycles that specific specialized processing units are idle.
#' The additional arguments, gcFirst and burnin, can be toggled to better
#' simulate the target context.
#' 
#' @param expr 
#' A valid R expression to be profiled.
#' @param events 
#' The class of processing unit to profile; options are "float",
#' "int", "numeric", "branch", "load", and "all".
#' @param gcFirst 
#' logical; determines if garbage collection should be called
#' before profiling.
#' @param burnin 
#' logical; determines if the function should first be evaluated
#' with an empty expression.
#' 
#' @return 
#' The results of the requested PAPI events are returned, in a named
#' list, with values stored in double precision.
#' 
#' @keywords programming
#' 
#' @examples
#' \dontrun{
#' library(pbdPAPI)
#' 
#' system.idle(1+1, events="float")
#' }
#' 
#' @export
system.idle <- function(expr, events="float", gcFirst=TRUE, burnin=TRUE)
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

