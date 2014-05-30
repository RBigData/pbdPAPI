papi.avail <- function(events=NA)
{
  papi.init()
  ret <- .Call("papi_event_avail", if(is.na(events)) c() else events)
  return(ret)
}
