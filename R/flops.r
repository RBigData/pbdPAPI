papi.flops <- function(expr)
{
  papi.check.ncounters(3L)
  
  ret <- .Call(papi_flops_on)
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call(papi_flops_off)
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}




#' flops
#' 
#' Measuring floating point operations.
#' 
#' This function measures the evaluation of the provided expression, expr, for
#' real time, process time, number of floating point operations, and floating
#' point operation rate. The additional arguments, gcFirst and burnin, can be
#' toggled to better simulate the target context.
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
#' The return is a list consisting of: 
#' \tabular{ll}{ 
#'    \code{real_time} \tab real time spent evaluating expression \cr 
#'    \code{proc_time} \tab total process time spent evaluating expression \cr 
#'    \code{flpops} \tab FLoating Point OPerationS (count) \cr 
#'    \code{mflops} \tab Mflops (flpops per second) \cr 
#' }
#' 
#' @keywords programming
#' 
#' @examples
#' \dontrun{
#' library(pbdPAPI)
#' 
#' system.flops(1+1)
#' }
#' 
#' @export
system.flops <- function(expr, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    b <- system.flops(gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    invisible(gc(FALSE))
  
  if (missing(expr))
    expr <- NULL
  
  events <- c("PAPI_FP_OPS", "PAPI_TOT_CYC")
  papi.avail.lookup(events=events, shorthand=FALSE)
  
  ret <- papi.flops(expr=expr)
  
  if (burnin)
  {
    for (i in 3:length(ret))
      ret[[i]] <- max(ret[[i]] - b[[i]], 0)
  }
  
  # Store expression call for plotting
  attr(ret, "call") <- as.character(match.call()[2])
  class(ret) <- c("papi_output", "papi_flops")
  
  return( ret )
}
