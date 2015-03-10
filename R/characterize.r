#' Categorizing Computation as CPU or RAM Bound
#' 
#' This attmpts to declare a computation as compute or memory bound.  Some
#' simplifying assumptions are made.  See the details section for more
#' information.
#' 
#' To make our determination, we measure the total number of floating point
#' instructions and the total number of L2 cache accesses.  If the number of
#' floating point instructions is greater, then we say the computation is
#' compute bound, and otherwise we say the computation is memory bound.
#' 
#' @param expr 
#' A valid R expression to be profiled.
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


