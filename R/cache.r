cache.opts <- function(type, which)
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
  else if (which == "l1.all")
    which <- paste(c("PAPI_L1_IC", "PAPI_L1_DC"), char, sep="")
  else if (which == "l2.all")
    which <- paste(c("PAPI_L2_IC", "PAPI_L2_DC"), char, sep="")
  else if (which == "l3.all")
    which <- paste(c("PAPI_L3_IC", "PAPI_L3_DC"), char, sep="")
  
  
  return( which )
}

system.cache <- function(expr, type="miss", events="total", gcFirst=TRUE)
{
  type <- match.arg(tolower(type), c("miss", "hit", "access", "read"))
  
  shorthand <- events
  
  events <- match.arg(tolower(events), c("total", "data", "instruction", "all", "l1.all", "l2.all", "l3.all"))
  events <- cache.opts(type=type, which=events)
  
  papi.avail.lookup(events=events, shorthand=shorthand)
  
  ret <- system.event(expr=expr, events=events, gcFirst=gcFirst)
  
  return( ret )
}
