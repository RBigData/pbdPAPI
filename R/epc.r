papi.epc <- function(expr, event)
{
  papi.check.ncounters(3L)
  
  ret <- .Call(papi_epc_on, event)
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call(papi_epc_off, event)
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}




#' epc
#' 
#' Events per cycle.
#' 
#' This function measures the evaluation of the provided expression, expr, for
#' real time, process time, reference clock cycles since last call, core clock
#' cycles since last call, total events since first call, and incremental
#' events per cycle since last call. The additional arguments, gcFirst and
#' burnin, can be toggled to better simulate the target context.
#' 
#' @param expr 
#' A valid R expression to be profiled.
#' @param event 
#' A PAPI event counter.
#' @param gcFirst 
#' logical; determines if garbage collection should be called
#' before profiling.
#' @param burnin 
#' logical; determines if the function should first be evaluated
#' with an empty expression.
#' 
#' @return 
#' The return is a list consisting of: \tabular{ll}{ \code{real_time}
#' \tab real time spent evaluating expression \cr \code{proc_time} \tab total
#' process time spent evaluating expression \cr \code{ref} \tab Incremental
#' reference clock cycles since last call. \cr \code{core} \tab Incremental
#' clock cycles since last call. \cr \code{evt} \tab Total events since first
#' call. \cr \code{epc} \tab Incremental events per cycle since last call. \cr
#' }
#' 
#' @keywords programming
#' 
#' @examples
#' \dontrun{
#' library(pbdPAPI)
#' 
#' system.epc(1+1,"PAPI_L1_DCM")
#' }
#' 
#' @export
system.epc <- function(expr, event, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    b <- system.epc(event=event, gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    invisible(gc(FALSE))
  
  if (missing(expr))
    expr <- NULL
  
  papi.avail.lookup(events=event, shorthand=FALSE)
  event.num <- papi.event.init(which=event)
  
  ret <- papi.epc(expr=expr, event=event.num)
  
  if (burnin)
  {
    for (i in 3:length(ret))
      ret[[i]] <- max(ret[[i]] - b[[i]], 0)
  }
  
  return( ret )
}
