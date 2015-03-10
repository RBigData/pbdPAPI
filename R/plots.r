check_classes <- function(l)
{
  classes <- unique(as.vector(sapply(l, class)))
  if (length(classes) > 2)
    stop("You must pass in objects of the same type", call.=FALSE)
  
  return(TRUE)
}



cache_lookup_type_levels <- function(x)
{
  if (any(class(x) == "papi_cache"))
  {
    type <- sub(names(x)[1], pattern=".*cache ", replacement="")
    type <- gsub(type, pattern="(^|[[:space:]])([[:alpha:]])", replacement="\\1\\U\\2", perl=TRUE)
    levels <- gsub(names(x), pattern="(^L| cache misses)", replacement="")
  }
  
  ## ...
  return(list(type=type, levels=levels))
}



plot.papi_cache <- function(x, ..., title, opnames, color=FALSE, show.opnames=TRUE, facet.by="opnames")
{
  facet.by <- match.arg(tolower(facet.by), c("opnames", "level"))
  
  tmp <- cache_lookup_type_levels(x=x)
  type <- tmp$type
  levels <- tmp$levels
  nlevels <- length(levels)
  
  l <- list(x, ...)
  check_classes(l)
  
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
  
  if (length(unique(opnames)) != len)
    stop("operation names are not unique.")
  
  opnames <- factor(opnames, levels=unique(opnames))
  
  df <- cbind(df, opnames=opnames)
  
  if (facet.by == "opnames")
  {
    xvar <- "level"
    facetvar <- "opnames"
    xlab <- "Cache Level"
  }
  else if (facet.by == "level")
  {
    xvar <- "opnames"
    facetvar <- "level"
    xlab <- "Operation"
    
    df$level <- paste("Level", df$level)
  }
  
  yvar <- "val"
  
  if (len > 1)
  {
    if (color)
      g <- ggplot(data=df, aes_string(x=xvar, y=yvar, fill="opnames"))
    else
      g <- ggplot(data=df, aes_string(x=xvar, y=yvar))
  }
  else
    g <- ggplot(data=df, aes_string(x=xvar, y=yvar))
  
  g <- g + 
    geom_bar(stat="identity") +
    geom_text(data=df, aes(label=val, y=val), vjust=0) +
    theme_bw() +
    xlab(xlab) +
    ylab(paste("Cache", type))
  
  g <- g + facet_wrap(as.formula(paste("~", facetvar)))
  
  if (!show.opnames)
    g <- g + theme(strip.background=element_blank(),
      strip.text.x=element_blank())
  
  if (!missing(title))
    g <- g + ggtitle(title)
  
  return(g)
}


#plot(x, y)
#plot(x, y, facet.by="level")


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



