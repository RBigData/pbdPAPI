#' cachebench
#' 
#' A benchmarker for measuring cache misses of multiple runs of
#' one or more functions.
#' 
#' @details
#' Currently only total cache misses are profiled.  Future versions
#' will allow the full generality of the \code{systme.cache()}
#' function.
#' 
#' @param ...
#' Expressions to be benchmarked.
#' @param type,events
#' See \code{?system.cache()}.
#' @param nreps
#' Number of replications for each expression.
#' 
#' @return
#' A dataframe (class "cachebench") containng the timings and some
#' basic summary statistics.
#' 
#' @examples
#' \dontrun{
#' pbdPAPI::cachebench(rnorm(1e4), rnorm(2e4), rnorm(3e4))
#' }
#' 
#' @export
cachebench <- function(..., type="miss", events="total", nreps=10) 
{
  l <- list(...)
  len <- length(l)
  
  if (len == 0)
    stop("No expressions")
  
  if (!is.numeric(nreps))
    stop("argument 'nreps' must be a positive integer")
  nreps <- as.integer(nreps)
  if (is.na(nreps) || nreps < 1)
    stop("argument 'nreps' must be a positive integer")
  
  
  typename <- paste("Cache", titlecase(pluralize(type)), "with", nreps, "Replications")
  
  args <- match.call()[-1]
  names <- names(args)
  if (is.null(names))
    argnames <- as.character(args)
  else
  {
    keep <- names(args) != "nreps"
    args <- args[keep]
    argnames <- names[keep]
    charargs <- as.character(args)
    
    argnames <- sapply(1:len, function(i) if (argnames[i] == "") charargs[i] else argnames[i])
  }
  
  template <- system.cache(NULL, type=type, events=events, gcFirst=TRUE, burnin=TRUE)
  colnames <- names(template)
  ret <- lapply(1:len, function(.) {tmp <- matrix(0L, nrow=nreps, ncol=length(template)); colnames(tmp) <- colnames; tmp})
  names(ret) <- argnames
  class(ret) <- "cachebench"
  
  for (i in 1:len)
  {
    for (j in 1:nreps)
    {
      tmp <- system.cache(expr=eval(args[[i]]), type=type, events=events, gcFirst=FALSE, burnin=FALSE)
      ret[[i]][j, ] <- as.integer(tmp)
    }
  }
  
  means <- do.call(rbind, lapply(ret, colMeans))
  colnames(means) <- gsub(colnames(means), pattern=" cache misses", replacement="")
  colnames(means) <- gsub(colnames(means), pattern="^L", replacement="Avg.L")
  
  summarystats <- data.frame(nreps=nreps)
  summarystats <- cbind(summarystats, means)
  ret$summarystats <- summarystats
  
  ret$type <- typename
  class(ret) <- "cachebench"
  
  return(ret)
}



#' Prints cachebench objects
#' 
#' @param x
#' A papi_output object to print.
#' @param ...
#' Extra arguments (ignored).
#' 
#' @name print-cachebench
#' @method print cachebench
#' @export
print.cachebench <- function(x, ...)
{
  cat(x$type, ":\n")
  print(x$summarystats)
}
