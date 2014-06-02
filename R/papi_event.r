papi.event.init <- function(which)
{
  ret <- .Call("papi_event_counter_init", which)
  
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

system.event <- function(expr, events, gcFirst=TRUE)
{
  if (gcFirst) 
    gc(FALSE)
  
  ret <- papi.start(events)
  
  if (!missing("expr"))
    eval(expr)
  
  ret <- papi.stop(events)
  
  return(ret)
}
