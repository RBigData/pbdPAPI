cache.opts <- function(which)
{
  if (which == "total")
    which <- c("PAPI_L1_TCM", "PAPI_L2_TCM", "PAPI_L3_TCM")
  else if (which == "data")
    which <- c("PAPI_L1_DCM", "PAPI_L2_DCM", "PAPI_L3_DCM")
  else if (which == "instruction")
    which <- c("PAPI_L1_ICM", "PAPI_L2_ICM", "PAPI_L3_ICM")
  else if (which == "all")
    which <- c("PAPI_L1_ICM", "PAPI_L1_DCM", "PAPI_L2_ICM", "PAPI_L2_DCM", "PAPI_L3_ICM", "PAPI_L3_DCM")
  else if (which == "l1.all")
    which <- c("PAPI_L1_ICM", "PAPI_L1_DCM")
  else if (which == "l2.all")
    which <- c("PAPI_L2_ICM", "PAPI_L2_DCM")
  else if (which == "l3.all")
    which <- c("PAPI_L3_ICM", "PAPI_L3_DCM")
  
  return( which )
}

system.cache <- function(expr, which="total")
{
  which <- match.arg(tolower(which), c("total", "data", "instruction", "all", "l1.all", "l2.all", "l3.all"))
  which <- cache.opts(which=which)
  
  ret <- papi.event(expr=expr, which=which)
  
  return( ret )
}
