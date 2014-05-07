cachefun <- function(fine)
{
  if (fine)
    .Call("papi_cache_fine_on")
  else
    .Call("papi_cache_on")
}

papi.cache <- function(expr, fine=FALSE)
{
  papi.init()
  papi.check(3L)
  
  ret <- cachefun(fine=fine)
  if (ret == -1L)
    stop("PAPI failed to initialize hardware counters.")
  
  if (!missing("expr"))
    eval(expr)
  
  ret <- cachefun(fine=fine)
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}
