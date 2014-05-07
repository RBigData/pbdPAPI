papi.cache <- function(expr)
{
  papi.init()
  papi.check(3L)
  
  ret <- .Call("papi_cache_on", 1L)
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.")
  
  if (!missing("expr"))
    eval(expr)
  
  ret <- .Call("papi_cache_off", 0L)
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}
