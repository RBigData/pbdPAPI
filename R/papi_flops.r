papi_get_flops <- function(expr)
{
  .Call("papi_get_flops", 1L)
  
  eval(expr)
  
  .Call("papi_get_flops", 0L)
  
  invisible()
}
