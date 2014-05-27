papi.event.init <- function(which)
{
  ret <- .Call("papi_event_counter_init", which)
  
  # Error on NA's (bad counter input)
#  nas <- is.na(ret)
#  if (any(nas))
#  {
#    if (length(which(nas==TRUE)) == 1L)
#    {
#      event.name <- "Event "
#      tobe.name <- " was "
#      counter.name <- "This counter "
#    }
#    else
#    {
#      event.name <- "Events "
#      tobe.name <- " were "
#      counter.name <- "These counters "
#    }
#    
#    stop(paste(event.name, paste(which[nas], collapse=", "), tobe.name, "undefined.\n", counter.name, "may not be available on your platform.", sep=""))
#  }
  
  # Warn on duplicates
  ret.uniq <- unique(ret)
  if (length(ret.uniq) != length(ret))
  {
    warning("Duplicate counters detected; using reduced set.")
    return(ret.uniq)
  }
  else
    return(ret)
}

papi.event.init.light <- function(which)
{
  ret <- .Call("papi_event_counter_init", which)
  
  ret.uniq <- unique(ret)
  if (length(ret.uniq) != length(ret))
    return(ret.uniq)
  else
    return(ret)
}

papi.start <- function(events)
{
  papi.init()
  papi.check(length(events))
  
  events <- papi.event.init(which=events)
  
  ret <- .Call("papi_event_counter_on", events) # ret: error value
  
  if (!is.integer(ret))
    stop(ret)
  
  return(ret)
}

papi.stop <- function(events)
{
  which <- papi.event.init.light(which=events)
  
  ret <- .Call("papi_event_counter_off", which) # ret: counter values
  
  if (length(ret) == 1L && is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return(ret)
}

papi.event <- function(expr, events)
{
  ret <- papi.start(events)
  
  if (!missing("expr"))
    eval(expr)
  
  ret <- papi.stop(events)
  
  return(ret)
}
