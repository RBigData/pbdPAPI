### Translate vector of strings of PAPI events into their C-level int equivs
papi.event.init <- function(which)
{
  ret <- .Call(papi_event_counter_init, which)
  
  # Warn on duplicates
  ret.uniq <- unique(ret)
  if (length(ret.uniq) != length(ret))
  {
    warning("Duplicate counters detected; using reduced set.")
    return(ret.uniq)
  }
  else
    return(ret)
}

papi.event.init.light <- function(which)
{
  ret <- .Call(papi_event_counter_init, which)
  
  ret.uniq <- unique(ret)
  if (length(ret.uniq) != length(ret))
    return(ret.uniq)
  else
    return(ret)
}



#' Low Level Interface
#' 
#' Low level interface for pbdPAPI.
#' 
#' This is the low-level interface for accessing hardware counters, and should
#' look somewhat similar to PAPI's own low-level interface.  This offers a
#' great deal of control, at the expense of ease of use.  If you are new to
#' pbdPAPI and/or PAPI, it is recommended that you start with the higher level
#' interfaces.
#' 
#' To use, you should first construct an events vector, which consists of a set
#' of pre-specified strings (listed in detail below; scroll down, you can't
#' miss it).  For example, if you want to profile level 1 and level 2 data
#' cache misses, you would set
#' 
#' \tabular{c}{ 
#'    \code{events <- c("PAPI_L1_DCM", "PAPI_L2_DCM")} 
#' }
#' 
#' You would then enclose the code you wish to profile with a call to
#' \code{papi.start(events)}, call that code, then call
#' \code{papi.stop(events)} to end profiling.
#' 
#' Another version of the low-level interface is in the \code{system.event()}
#' function, which has syntax reminiscent of \code{system.time()}.  Using this
#' instead of \code{papi.start()} and \code{papi.stop()} in the above example,
#' you would instead call \code{system.event(expr, events)}, where \code{expr}
#' is the valid R expression you want profiled.
#' 
#' @aliases papi.start papi.stop system.event
#' 
#' @param events 
#' A vector of PAPI events (as strings).  See details section for
#' more information.
#' @param expr 
#' A valid R expression to be profiled.
#' @param gcFirst 
#' logical; determines if garbage collection should be called
#' before profiling.
#' @param burnin 
#' logical; determines if the function should first be evaluated
#' with an empty expression. This can be especially important for measuring
#' cache effects.
#' 
#' @return 
#' The results of the requested PAPI events are returned, in a named
#' list, with values stored in double precision.
#' 
#' @examples
#' \dontrun{
#' library(pbdPAPI)
#' 
#' events <- c("PAPI_L1_DCM", "PAPI_L2_DCM")
#' 
#' # Start profiling
#' papi.start(which=events)
#' 
#' # Computationally interesting code
#' sum(sapply(1:100, sqrt))
#' 
#' # Stop profiling and collect results
#' results <- papi.stop(which=events)
#' results
#' }
#' 
#' @rdname lowlevel
#' @export
papi.start <- function(events)
{
  papi.check.ncounters(length(events))
  
  events <- papi.event.init(which=events)
  
  ret <- .Call(papi_event_counter_on, events) # ret: error value
  
  if (!is.integer(ret))
    stop(ret)
  
  return(ret)
}



### Stop profiling, read values of counters
#' @rdname lowlevel
#' @export
papi.stop <- function(events)
{
  which <- papi.event.init.light(which=events)
  
  ret <- .Call(papi_event_counter_off, which) # ret: counter values
  
  if (length(ret) == 1L && is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return(ret)
}



### Simple wrapper
#' @rdname lowlevel
#' @export
system.event <- function(expr, events, gcFirst=TRUE, burnin=TRUE)
{
  papi.start(events=events)
  papi.stop(events=events)
  
  if (missing(events))
    stop("argument \"events\" is missing, with no default")
  
  if (burnin)
    b <- system.event(events=events, gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    invisible(gc(FALSE))
  
  if (missing(expr))
    expr <- NULL
  
  papi.avail.lookup(events=events, shorthand=FALSE)
  
  ret <- papi.start(events=events)
  err <- tryCatch(eval(expr), error=identity)
  ret <- papi.stop(events=events)
  
  if (inherits(err, "simpleError"))
    stop(err)
  
  if (burnin)
  {
    for (i in 1:length(ret))
      ret[[i]] <- max(ret[[i]] - b[[i]], 0)
  }
  
  class(ret) <- "papi_output"
  
  return(ret)
}

