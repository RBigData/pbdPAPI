papi.avail.internal <- function()
{
  papi.init()
  
  ret <- .Call("papi_event_avail", NULL)
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
    fail <- events[-which(events %in% events.all)]
    stop(paste("Event(s) ", paste(fail, collapse=", "), "is/are not recognized PAPI events"))
  }
  
  l <- sapply(events, function(i) which(i == events.all))
  
  ret <- avail[l, 2L]
  return( ret )
}

