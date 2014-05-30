papi.avail <- function(events=NA)
{
  ret <- .Call("papi_event_avail", if(is.na(events)) c() else events)
}
