# Note, this is very fragile and experimental
### TODO include more than just misses...
cachebench <- function(..., nreps=10) 
{
  l <- list(...)
  len <- length(l)
  
  type <- "Cache Misses"
  
  if (len == 0)
    stop("No expressions")
  
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
  
  template <- system.cache(NULL)
  colnames <- names(template)
  ret <- lapply(1:len, function(.) {tmp <- matrix(0L, nrow=nreps, ncol=3); colnames(tmp) <- colnames; tmp})
  names(ret) <- argnames
  class(ret) <- "cachebench"
  
  for (i in 1:len)
  {
    for (j in 1:nreps)
    {
      tmp <- system.cache(expr=eval(args[[i]]))
      ret[[i]][j, ] <- as.integer(tmp)
    }
  }
  
  means <- do.call(rbind, lapply(ret, colMeans))
  colnames(means) <- gsub(colnames(means), pattern=" cache misses", replacement="")
  colnames(means) <- gsub(colnames(means), pattern="^L", replacement="Avg.L")
  
  summarystats <- data.frame(nreps=nreps)
  summarystats <- cbind(summarystats, means)
  ret$summarystats <- summarystats
  
  ret$type <- type
  
  return(ret)
}



print.cachebench <- function(x)
{
  cat(x$type, ":\n")
  print(x$summarystats)
}

