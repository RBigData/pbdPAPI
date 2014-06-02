### 
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



### Error checking for the higher interfaces
papi.avail.lookup <- function(events, shorthand)
{
  check.avail <- try(papi.avail(events=events), silent=TRUE)
  
  if (class(check.avail) == "try-error")
    stop("Unknown hardware events; please report this bug to the pbdPAPI developers.")
  else if (!all(check.avail))
  {
    notavail <- -which(check.avail)
    if (length(events) - length(notavail) == 1)
    {
      event <- " event "
      tobe <- " is "
    }
    else
    {
      event <- " events "
      tobe <- " are "
    }
    
    events.fail <- if (length(notavail) == 0) events else events[notavail]
    events.fail <- paste(events.fail, collapse=", ")
    stop(paste("Your platform does not support the hardware", event, "{", events.fail, "} which", tobe, "needed for events=\"", shorthand, "\".\n\nSee ?papi.avail for more information.", sep=""))
  }
  
  invisible()
}

