papi.example <- function()
{
  papi.check.ncounters(2L)
  
  .Call("papi_example")
  
  invisible()
}
