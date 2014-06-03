hw.info <- function()
{
  info <- .Call("papi_hwinfo")
  
  val <- info[[2L]]
  attr(x=val, which="names") <- info[[1L]]
  
  return(val)
}

hw.info.name <- function()
{
  intinfo <- hw.info()
  info <- .Call("papi_hwname")
  
  id.family <- intinfo["cpuid_family"]
  id.model <- intinfo["cpuid_model"]
  
  val <- c(info[[2]], arch.lookup(id.family, id.model))
  attr(x=val, which="names") <- c(info[[1]], "codename")
  
  return(val)
}


# -----------------------------------------------------------------------------
# Processor model lookup
# -----------------------------------------------------------------------------


#hw.arch <- function(vendor)
#{
#  if (vendor == "GenuineIntel")
#    return( arch.intel(family, model) )
##  else if (vendor == "AuthenticAMD")
##    
#  else
#    stop("Hardware vendor not supported")
#}


arch.lookup <- function(family, model)
{
  if (family == 0x06)
  {
    return(arch.intel(family, model))
  }
  else
  {
    return("Unknown")
  }
}

#https://software.intel.com/en-us/articles/intel-architecture-and-processor-identification-with-cpuid-model-and-family-numbershttps://software.intel.com/en-us/articles/intel-architecture-and-processor-identification-with-cpuid-model-and-family-numbers
arch.intel <- function(family, model)
{
  if (family == 0x06)
  {
    if (model == 0x3A)
    {
      arch <- "IvyBdridge"
    }
    else if (model == 0x2A || model == 0x2D)
    {
      arch <- "SandyBridge"
    }
    else if (model == 0x25 || model == 0x2C || model == 0x2F)
    {
      arch <- "Westmere"
    }
    else if (model == 0x1E || model == 0x1A || model == 0x2E)
    {
      arch <- "Nehalem"
    }
    else if (model == 0x17 || model == 0x1D)
    {
      arch <- "Penryn"
    }
    else if (model == 0x0F || model == 0x16)
    {
      arch <- "Merom"
    }
    else if (model == 0x06)
    {
      arch <- "Presler"
    }
    else if (model == 0x03)
    {
      arch <- "Nocona"
    }
    else if (model == 0x04)
    {
      arch <- "Prescott"
    }
    else if (model == 0x0D)
    {
      arch <- "Dothan"
    }
    else
    {
      arch <- "Unknown"
    }
    
    ret <- list(archname=arch)
    return( ret )
  }
}








