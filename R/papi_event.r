### Translate vector of strings of PAPI events into their C-level int equivs
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



### Initialize counters, start profiling
papi.start <- function(events)
{
  papi.check.ncounters(length(events))
  
  events <- papi.event.init(which=events)
  
  ret <- .Call("papi_event_counter_on", events) # ret: error value
  
  if (!is.integer(ret))
    stop(ret)
  
  return(ret)
}



### Stop profiling, read values of counters
papi.stop <- function(events)
{
  which <- papi.event.init.light(which=events)
  
  ret <- .Call("papi_event_counter_off", which) # ret: counter values
  
  if (length(ret) == 1L && is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return(ret)
}



### Simple wrapper
system.event <- function(expr, events, gcFirst=TRUE, burnin=TRUE)
{
  if (missing(events))
    stop("argument \"events\" is missing, with no default")
  
  if (burnin)
    system.event(events=events, gcFirst=gcFirst, burnin=FALSE)
  
  if (gcFirst) 
    gc(FALSE)
  
  if (missing(expr))
    expr <- NULL
  
  papi.avail.lookup(events=events, shorthand=FALSE)
  
  ret <- papi.start(events)
  
  if (!missing("expr"))
    eval(expr)
  
  ret <- papi.stop(events)
  
  return(ret)
}

