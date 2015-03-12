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



cachemelt <- function(df)
{
  len <- ncol(df) - 1
  value <- sapply(sapply(1:len, function(i) df[, i]), c)
  nm <- names(df)
  variable <- as.character(sapply(sapply(1:len, function(i) rep(nm[i], nrow(df))), c))
  Test <- rep(df$Test, len)
  
  data.frame(Test=Test, variable=variable, value=value)
}



plot.cachebench <- function(x, levels=1:3, axis.x.angle=0)
{
  ### To fool R CMD check
  Test <- value <- NULL
  rm(list=c("Test", "value"))
  
  
  tmp <- x
  tmp$summarystats <- NULL
  tmp$type <- NULL
  
  nm <- names(tmp)
  df <- do.call(rbind, lapply(1:length(tmp), function(i) data.frame(tmp[[i]], nm[i])))
  df <- df[, c(levels, 4)]
  
  colnames(df)[ncol(df)] <- "Test"
  colnames(df) <- gsub(colnames(df), pattern=".cache.misses", replacement="", fixed=TRUE)
  
  df <- cachemelt(df)
  
  g1 <- 
    ggplot(df, aes(Test, value)) +
      stat_boxplot(geom ='errorbar')+
      geom_boxplot() + 
      theme(axis.text.x=element_text(angle=axis.x.angle, hjust=1)) +
      xlab("Test") + 
      ylab("") + 
      ggtitle(x$type) + 
      facet_wrap(~ variable)
  
#  g2 <- g3 <- g1
#  
#  plots <- list(g1=g1, g2=g2, g3=g3)
#  label <- x$type
#  row_plotter(plots, levels, label, show.title=TRUE)
  g1
}

### Example
#x <- cachebench(A=rnorm(1e4), B=rnorm(1e5))
#plot(x)

