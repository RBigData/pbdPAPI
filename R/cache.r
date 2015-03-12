nmatches <- function(x, pattern)
{
  tmp <- grep(x=x, pattern=pattern)
  
  if (length(tmp) == 0)
    return( 0L )
  else
    return( tmp )
}



is.match <- function(x, pattern)
{
  return( nmatches(x=x, pattern=pattern) != 0 )
}



cache.opts <- function(type, which, level)
{
  if (type == "miss")
    char <- "M"
  else if (type == "hit")
    char <- "H"
  else if (type == "access")
    char <- "A"
  else if (type == "read")
    char <- "R"
  
  if (which == "total")
    which <- paste(c("PAPI_L1_TC", "PAPI_L2_TC", "PAPI_L3_TC"), char, sep="")
  else if (which == "data")
    which <- paste(c("PAPI_L1_DC", "PAPI_L2_DC", "PAPI_L3_DC"), char, sep="")
  else if (which == "instruction")
    which <- paste(c("PAPI_L1_IC", "PAPI_L2_IC", "PAPI_L3_IC"), char, sep="")
  else if (which == "all")
    which <- paste(c("PAPI_L1_IC", "PAPI_L1_DC", "PAPI_L2_IC", "PAPI_L2_DC", "PAPI_L3_IC", "PAPI_L3_DC"), char, sep="")
  else if (is.match(x=which, pattern="l[0-9][.]all"))
    which <- paste(c(paste("PAPI_L", level, "_IC", sep=""), paste("PAPI_L", level, "_DC", sep="")), char, sep="")
  else if (is.match(x=which, pattern="l[0-9][.]ratio"))
    which <- c(paste("PAPI_L", level, "_TC", char, sep=""), paste("PAPI_L", level, "_TCA", sep=""))
  
  return( which )
}



#' Cache
#' 
#' Measuring cache misses, hits, accesses, and reads.
#' 
#' This function measures the evaluation of the provided expression, expr, for
#' different classes of cache events additionally restricted to misses, hits,
#' accesses, or reads. The additional arguments, gcFirst and burnin, can be
#' toggled to better simulate the target context.
#' 
#' @param expr 
#' A valid R expression to be profiled.
#' @param type 
#' The type of cache event to profile; options are "miss", "hit",
#' "access", and "read".
#' @param events 
#' The class of cache events to profile; options are "total",
#' "data", "instruction", "all", "l1.all", "l2.all", "l3.all", "l1.ratio",
#' "l2.ratio", and "l3.ratio".
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
#' system.cache(1+1, type="miss", events="l2.all")
#' }
#' 
#' @export
system.cache <- function(expr, type="miss", events="total", gcFirst=TRUE, burnin=TRUE)
{
  type <- match.arg(tolower(type), c("miss", "hit", "access", "read"))
  
  if (missing(expr))
    expr <- NULL
  
  shorthand <- events
  
  events <- match.arg(tolower(events), c("total", "data", "instruction", "all", "l1.all", "l2.all", "l3.all", "l1.ratio", "l2.ratio", "l3.ratio"))
  
  is.ratio <- is.match(x=events, pattern="[.]ratio")
  is.all <- is.match(x=events, pattern="[.]all")
  
  if (is.ratio || is.all)
  {
    level <- sub(x=events, pattern="[.].*", replacement="")
    level <- sub(x=level, pattern="l", replacement="")
  }
  else
    level <- NULL
  
  if (type == "access" && is.ratio)
  {
    warning("Duplicate counters requested")
    ret <- 1L
    names(ret) <- names(ret) <- paste("L", level, " cache ", type, " ratio", sep="")
    return( ret )
  }
  
  events <- cache.opts(type=type, which=events, level=level)
  
  papi.avail.lookup(events=events, shorthand=shorthand)
  
  ret <- system.event(expr=expr, events=events, gcFirst=gcFirst, burnin=burnin)
  
  if (is.ratio)
  {
    ret <- ret[[1L]] / ret[[2L]]
    names(ret) <- paste("L", level, " cache ", type, " ratio", sep="")
    class(ret) <- "papi_output"
  }
  
  names(ret) <- gsub(names(ret), pattern="Level ", replacement="L")
  
  # Store expression call for plotting
  attr(ret, "call") <- as.character(match.call()[2])
  class(ret) <- c(class(ret), "papi_cache")
  
  return( ret )
}

