papi.init <- function()
{
  ret <- .Call("papi_init")
  
  if (ret != 0L)
    stop("Could not initialize PAPI; compile time and run time versions disagree")
  else
    invisible( ret )
}
