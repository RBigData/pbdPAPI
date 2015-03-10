papi.flips <- function(expr)
{
  papi.check.ncounters(3L)
  
  ret <- .Call(papi_flips_on)
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call(papi_flips_off)
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}




#' flips
#' 
#' Measuring floating point instructions.
#' 
#' This function measures the evaluation of the provided expression, expr, for
#' real time, process time, number of floating point instructions, and floating
#' point instruction rate. The additional arguments, gcFirst and burnin, can be
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
#'    \code{flpins} \tab FLoating Point INStructions (count) \cr 
#'    \code{mflips} \tab Mflips (flpins per second) \cr 
#' }
#' 
#' @keywords programming
#' 
#' @examples
#' \dontrun{
#' library(pbdPAPI)
#' 
#' system.flips(1+1)
#' }
#' 
#' @export
system.flips <- function(expr, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    b <- system.flips(gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    invisible(gc(FALSE))
  
  if (missing(expr))
    expr <- NULL
  
  events <- "PAPI_FP_INS"
  papi.avail.lookup(events=events, shorthand=FALSE)
  
  ret <- papi.flips(expr=expr)
  
  if (burnin)
  {
    for (i in 3:length(ret))
      ret[[i]] <- max(ret[[i]] - b[[i]], 0)
  }
  
  return( ret )
}
