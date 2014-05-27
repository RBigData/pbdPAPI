papi.event.init <- function(which)
{
  ret <- .Call("papi_event_counter_init", which) # ret: name to id conversion
  return(ret)
}

papi.start <- function(which)
{
  papi.init()
  
  which <- papi.event.init(which=which)
  
  ret <- .Call("papi_event_counter_on", which) # ret: error value
  
  if (!is.integer(ret))
    stop(ret)
  
  return(ret)
}

papi.stop <- function(which)
{
  which <- papi.event.init(which=which)
  
  ret <- .Call("papi_event_counter_off", which) # ret: counter values
  
  if (length(ret) == 1L && is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return(ret)
}

papi.event <- function(expr, which)
{
  ret <- papi.start(which)
  
  if (!missing("expr"))
    expr
  
  ret <- papi.stop(which)
  
  return(ret)
}
