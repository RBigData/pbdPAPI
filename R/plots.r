plot.papi_cache <- function(x, ..., title, opnames, show.opnames=TRUE)
{
  type <- sub(names(x)[1], pattern=".*cache ", replacement="")
  type <- gsub(type, pattern="(^|[[:space:]])([[:alpha:]])", replacement="\\1\\U\\2", perl=TRUE)
  levels <- gsub(names(x), pattern="(Level | cache misses)", replacement="")
  nlevels <- length(levels)
  
  l <- list(x, ...)
  if (length(unique(sapply(l, class))) > 1)
    stop("You must pass in objects of the same type")
  
  len <- length(l)
  
  val <- as.numeric(sapply(l, as.numeric))
  
  df <- data.frame(val=val, level=levels)
  
  
  if (!missing(opnames))
  {
    if (length(opnames) != len)
    {
      badlen <- TRUE
      warning("Length of argument 'opnames' does not match number of inputs; ignoring")
    }
    else
    {
      badlen <- FALSE
      opnames <- as.vector(sapply(opnames, function(nm) rep(nm, nlevels)))
    }
  }
  
  if (missing(opnames) || badlen)
  {
    opnames <- sapply(l, function(y) attributes(y)$call)
    opnames <- as.vector(sapply(opnames, function(nm) rep(nm, nlevels)))
  }
  
  opnames <- factor(opnames, levels=unique(opnames))
  df <- cbind(df, opnames=opnames)
  
  g <- 
  ggplot(data=df, aes(x=level, y=val)) + 
    geom_bar(stat="identity") + 
    xlab("Level") + 
    ylab(paste("Cache", type))
  
  g <- g + facet_wrap(~ opnames)
  
  if (!show.opnames)
    g <- g + theme(strip.background=element_blank(),
      strip.text.x=element_blank())
  
  if (!missing(title))
    g <- g + ggtitle(title)
  
  return(g)
}


### Examples
#library(pbdPAPI)
#x <- system.cache(rnorm(2e2))
#y <- system.cache(rnorm(1e4))
#z <- system.cache(rnorm(1e6))

#plot(x)
#plot(x, show.opnames=FALSE)

#plot(x, y, opnames=c("2e2", "1e4"), title="rnorm()")

#plot(x, y, z, opnames=c("rnorm(2e2)", "rnorm(1e4)", "rnorm(1e6)"))

#plot(x, y, z, opnames=c("2e2", "1e4"))


