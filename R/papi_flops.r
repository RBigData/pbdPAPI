papi.flops <- function(expr)
{
  papi.check(3L)
  
  ret <- .Call("papi_flops_on", 1L)
  if (ret == -1L)
    stop("Could not initialise PAPI_flops \nYour platform may not support floating point operation event.\n")
  
  eval(expr)
  
  ret <- .Call("papi_flops_off", 0L)
  if (is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}
