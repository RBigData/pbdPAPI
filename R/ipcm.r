ipcm.cache.opts <- function(type,level)
{
  c_miss <- c("IPCM_L2_TCM", "IPCM_L3_TCM")
  c_hit <- c("IPCM_L2_TCH", "IPCM_L3_TCH", "IPCM_L3_TCH_S", "IPCM_L3_TCH_NS")

  if(type=="miss")
    ret <- c_miss
  else if(type=="hit")
    ret <- c_hit
  else
    ret <- c(c_miss, c_hit)

  if(!is.null(level))
    ret <- grep(paste("L", level, sep=""), ret, value=T)

  return( ret )
}

ipcm.mid.events <- function(expr, events, gcFirst, burnin)
{
  ret <- system.event(expr=expr, events=events, gcFirst=gcFirst, burnin=burnin)

  return( ret )
}



#' IPCM Cache
#' 
#' Measuring cache misses and hits
#' 
#' This function measures the evaluation of the provided expression, expr, for
#' different classes of cache events additionally restricted to misses or hits.
#' The additional arguments, gcFirst and burnin, can be toggled to better
#' simulate the target context.
#' 
#' @param expr 
#' A valid R expression to be profiled.
#' @param type 
#' The type of cache event to profile; options are "miss", "hit",
#' and "all".
#' @param level 
#' The cache level to profile; options are "2" and "3".
#' @param gcFirst logical; determines if garbage collection should be called
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
#' ipcm.cache(1+1, type="miss", level="2")
#' }
#' 
#' @export
ipcm.cache <- function(expr, type="all", level=NULL, gcFirst=TRUE, burnin=TRUE)
{
  ipcm_events <- ipcm.cache.opts(type, level)

  ret <- ipcm.mid.events(expr, ipcm_events, gcFirst, burnin)

  return( ret )
}
