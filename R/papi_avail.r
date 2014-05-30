papi.avail <- function(events=NA)
{
  papi.init()

  ret <- .Call("papi_event_avail", if(is.na(events)) c() else events)

  if(is.list(ret)){
    ret <- t(ret)
    colnames(ret) <- c("event","avail","desc")
  }

  return(ret)
}
