#' papiexport
#' 
#' Export pbdPAPI profiler objects.  The typicial use case would
#' be to profile with pbdPAPI, export the profiler objects, 
#' then finally import them to an R session where the hpcvis
#' package is installed.
#' 
#' @details
#' pbdPAPI can only be installed and utilized in a limited number
#' of environments (mostly Linux).  Particularly for managed
#' machines (clusters, supercomputers, servers) where the user
#' does not have root access, installing large sets of dependencies
#' can be very difficult.  The hpcvis package greatly enhances
#' pbdPAPI's capabilities, adding numerous plot utilities, but at 
#' the cost of a very large dependency chain.  With \code{papiexport()},
#' one can easily run the benchmarks on a machine with pbdPAPI
#' installed, export them with \code{papiexport()}, then import
#' the results into an R session with hpcvis installed.
#' 
#' The function is a wrapper around \code{dump()}.
#' 
#' @param ...
#' A set of objects generated
#' @param file
#' An optional argument to 
#' 
#' @return
#' \code{NULL}
#' 
#' @examples
#' \dontrun{
#' library(pbdPAPI)
#' x <- system.cache(rnorm(1e4))
#' y <- system.cache(rnorm(2e4))
#' 
#' papiexport(x, y)
#' }
#' 
#' @export
papiexport <- function(..., file=NULL)
{
  if (is.null(file))
    file <- stdout()
  
  args <- match.call()
  if (!is.null(as.list(args)$file))
    reject <- -c(1, length(args))
  else
    reject <- -1
  
  args <- as.character(args[reject])
  
  dump(list=args, file)
  
  invisible()
}
