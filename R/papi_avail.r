papi.avail.internal <- function()
{
  papi.init()
  
  ret <- .Call("papi_event_avail", NULL)
  ret[[2L]] <- as.logical(ret[[2L]])
  ret <- as.data.frame(ret, stringsAsFactors=FALSE)
  colnames(ret) <- c("event","avail","desc")
  
  assign(".__pbdPAPI_avail", ret, envir=.__pbdPAPIEnv)
  
  invisible()
}



papi.avail <- function(events)
{
  avail <- get(".__pbdPAPI_avail", envir=.__pbdPAPIEnv)
  
  if (missing(events))
    return(avail)
  
  events.all <- avail[, 1L]
  
  if (!all(events %in% events.all))
  {
    stop("")
  }
  
  l <- sapply(events, function(i) which(i == events.all))
  
  ret <- avail[l, 2L]
  return( ret )
}

