papi.avail <- function(events=NA)
{
  papi.init()

  if(length(events)>1){
    arg <- events
  }
  else{
    arg <- if(is.na(events)) c() else events
  }

  ret <- .Call("papi_event_avail", arg)

  if(is.list(ret)){
    ret <- do.call(cbind,ret)
    colnames(ret) <- c("event","avail","desc")
  }

  return(ret)
}
