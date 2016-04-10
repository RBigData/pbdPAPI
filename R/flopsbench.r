#' flopsbench
#' 
#' A benchmarker for measuring flops (floating point operations per
#' second) of one or more functions.
#' 
#' @param data
#' A list of data for the provided functions.
#' @param ...
#' Expressions to be benchmarked.
#' 
#' @return
#' A dataframe (class "flopsbench") containng the timings and some
#' basic summary statistics.
#' 
#' @examples
#' \dontrun{
#' data <- list(runif(1e4), runif(2e4), runif(3e4))
#' flopsbench(sqrt, exp, data=data)
#' }
#' 
#' @export
flopsbench <- function(..., data)
{
  l <- list(...)
  len <- length(l)
  if (len == 0)
    stop("No expressions")
  
  datalen <- length(data)
  
  
  args <- match.call()[-1]
  names <- names(args)
  if (is.null(names))
    argnames <- as.character(args)
  else
  {
    keep <- names(args) != "data"
    args <- args[keep]
    argnames <- names[keep]
    charargs <- as.character(args)
    
    argnames <- sapply(1:len, function(i) if (argnames[i] == "") charargs[i] else argnames[i])
  }
  
  template <- system.flops(NULL, gcFirst=TRUE, burnin=TRUE)
  colnames <- names(template)
  ret <- lapply(1:len, function(.) {tmp <- matrix(0, nrow=datalen, ncol=4); colnames(tmp) <- colnames; tmp})
  names(ret) <- argnames
  
  for (i in 1:len)
  {
    for (j in 1:datalen)
    {
      expr <- parse(text=paste0(args[[i]], "(data[[j]])"))
      tmp <- system.flops(expr=eval(expr), gcFirst=TRUE, burnin=FALSE)
      ret[[i]][j, ] <- as.numeric(tmp)
    }
  }
  
  
  n <- sapply(data, NROW) * sapply(data, NCOL)
  ret <- lapply(ret, function(x) cbind(x, n))
  
  # means <- do.call(rbind, lapply(ret, colMeans))
  # ret$summarystats <- means
  
  # type <- "FLOP/s"
  # ret$type <- type
  class(ret) <- "flopsbench"
  
  return(ret)
}
