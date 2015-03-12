#' Num.Counters
#' 
#' Number of counters available on the system.
#' 
#' This function returns the number of counters available on the
#' system.  A return value of zero indicates that your system
#' does not support profiling by hardware event counters.
#' 
#' @return 
#' The number of counters available on the system.
#' 
#' @keywords programming
#' 
#' @examples
#' \dontrun{
#' library(pbdPAPI)
#' 
#' system.ncounters()
#' }
#' 
#' @export
system.ncounters <- function()
{
  ret <- .Call(papi_check_counters, 0L)
  
  if (ret == -2L)
    return( 0L )
  else
    return( ret )
}
