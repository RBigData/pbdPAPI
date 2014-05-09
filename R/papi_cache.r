cache.opts <- function(which)
{
  if (which == "total")
    which <- 6L:8L
  else if (which == "data")
    which <- c(0L, 2L, 4L)
  else if (which == "instruction")
    which <- c(1L, 3L, 5L)
  else if (which == "all")
    which <- 0L:5L
  else if (which == "l1.all")
    which <- c(0L, 1L)
  else if (which == "l2.all")
    which <- c(2L, 3L)
  else if (which == "l3.all")
    which <- c(4L, 5L)
  
  return( as.integer(which) )
}

papi.cache <- function(expr, which="total")
{
  which <- match.arg(tolower(which), c("total", "data", "instruction", "all", "l1.all", "l2.all", "l3.all"))
  papi.init()
  papi.check(length(which))
  
  which <- cache.opts(which=which)
  
  ret <- .Call("papi_cache_misses_on", which)
  
  if (!is.integer(ret))
    stop(ret)
  
  if (!missing("expr"))
    eval(expr)
  
  ret <- .Call("papi_cache_misses_off", which)
  
  if (length(ret) == 1L && is.integer(ret) && ret == -1L)
    stop("There was a problem recovering the counter information.")
  
  return( ret )
}
