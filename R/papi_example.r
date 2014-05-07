papi.example <- function()
{
  papi.check(2L)
  
  .Call("papi_example")
  
  invisible()
}
