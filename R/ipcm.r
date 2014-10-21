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

ipcm.cache <- function(expr, type="all", level=NULL, gcFirst=TRUE, burnin=TRUE)
{
  ipcm_events <- ipcm.cache.opts(type, level)

  ret <- ipcm.mid.events(expr, ipcm_events, gcFirst, burnin)

  return( ret )
}
