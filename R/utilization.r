papi.utilization <- function(expr)
{
  papi.check.ncounters(3L)
  
  ret <- .Call(papi_utilization_on)
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.\nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call(papi_utilization_off)
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}




#' utilization
#' 
#' CPU Utilization.
#' 
#' We define processor utilization as:\cr
#' 
#' \eqn{utilization = \frac{ins}{clockrate\ *\ real\_time\ *\ ncpus}} \cr
#' 
#' where \code{ins} is the number of instructions measured by PAPI,
#' \code{clockrate} is the clock frequency of the processor in Hz,
#' \code{real_time} is the wall clock time, and \code{ncpus} is the number of
#' "cores" (physical+logical) detected by PAPI.
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
#' @return The return is a list consisting of: 
#' \tabular{ll}{ 
#' \code{real_time} \tab real time spent evaluating expression \cr 
#' \code{proc_time} \tab total process time spent evaluating expression \cr 
#' \code{ins} \tab Number of instructions. \cr 
#' \code{ipc} \tab Instructions per cycle. \cr
#' \code{utilization} \tab CPU utilization (proportion) \cr 
#' }
#' 
#' @keywords programming
#' 
#' @examples
#' 
#' \dontrun{
#' library(pbdPAPI)
#' 
#' system.flops(1+1)
#' }
#' 
#' @export
system.utilization <- function(expr, gcFirst=TRUE, burnin=TRUE)
{
  if (burnin)
    b <- system.utilization(gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    invisible(gc(FALSE))
  
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
