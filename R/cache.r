nmatches <- function(x, pattern)
{
  tmp <- grep(x=x, pattern=pattern)
  
  if (length(tmp) == 0)
    return( 0L )
  else
    return( tmp )
}


is.match <- function(x, pattern)
{
  return( nmatches(x=x, pattern=pattern) != 0 )
}


cache.opts <- function(type, which, level)
{
  if (type == "miss")
    char <- "M"
  else if (type == "hit")
    char <- "H"
  else if (type == "access")
    char <- "A"
  else if (type == "read")
    char <- "R"
  
  if (which == "total")
    which <- paste(c("PAPI_L1_TC", "PAPI_L2_TC", "PAPI_L3_TC"), char, sep="")
  else if (which == "data")
    which <- paste(c("PAPI_L1_DC", "PAPI_L2_DC", "PAPI_L3_DC"), char, sep="")
  else if (which == "instruction")
    which <- paste(c("PAPI_L1_IC", "PAPI_L2_IC", "PAPI_L3_IC"), char, sep="")
  else if (which == "all")
    which <- paste(c("PAPI_L1_IC", "PAPI_L1_DC", "PAPI_L2_IC", "PAPI_L2_DC", "PAPI_L3_IC", "PAPI_L3_DC"), char, sep="")
  else if (is.match(x=which, pattern="l[0-9][.]all"))
    which <- paste(c(paste("PAPI_L", level, "_IC", sep=""), paste("PAPI_L", level, "_DC", sep="")), char, sep="")
  else if (is.match(x=which, pattern="l[0-9][.]ratio"))
    which <- c(paste("PAPI_L", level, "_TC", char, sep=""), paste("PAPI_L", level, "_TCA", sep=""))
  
  return( which )
}



system.cache <- function(expr, type="miss", events="total", gcFirst=TRUE, burnin=TRUE)
{
  type <- match.arg(tolower(type), c("miss", "hit", "access", "read"))
  
  if (missing(expr))
    expr <- NULL
  
  shorthand <- events
  
  events <- match.arg(tolower(events), c("total", "data", "instruction", "all", "l1.all", "l2.all", "l3.all", "l1.ratio", "l2.ratio", "l3.ratio"))
  
  is.ratio <- is.match(x=events, pattern="[.]ratio")
  is.all <- is.match(x=events, pattern="[.]all")
  
  if (is.ratio || is.all)
  {
    level <- sub(x=events, pattern="[.].*", replacement="")
    level <- sub(x=level, pattern="l", replacement="")
  }
  else
    level <- NULL
  
  if (type == "access" && is.ratio)
  {
    warning("Duplicate counters requested")
    ret <- 1L
    names(ret) <- names(ret) <- paste("L", level, " cache ", type, " ratio", sep="")
    return( ret )
  }
  
  events <- cache.opts(type=type, which=events, level=level)
  
  papi.avail.lookup(events=events, shorthand=shorthand)
  
  ret <- system.event(expr=expr, events=events, gcFirst=gcFirst, burnin=burnin)
  
  if (is.ratio)
  {
    ret <- ret[[1L]] / ret[[2L]]
    names(ret) <- paste("L", level, " cache ", type, " ratio", sep="")
  }
  
  
  class(ret) <- c(class(ret), "papi_cache")
  
  return( ret )
}

papi.cache <- system.cache
