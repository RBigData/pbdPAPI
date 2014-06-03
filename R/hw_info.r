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
  
  attr(x=info[[2L]], which="names") <- info[[1L]]
  
  id.family <- intinfo["cpuid_family"]
  id.model <- intinfo["cpuid_model"]
  
  val <- c(info[[2L]], arch.lookup(id.family, id.model))
  
  return(val)
}


# -----------------------------------------------------------------------------
# Processor model lookup
# -----------------------------------------------------------------------------

arch.lookup <- function(family, model)
{
  if (family == 0x06)
  {
    return(arch.intel(model))
  }
  else
  {
    return("Unknown")
  }
}

#http://www.cpu-world.com/cgi-bin/CPUID.pl
#https://software.intel.com/en-us/articles/intel-architecture-and-processor-identification-with-cpuid-model-and-family-numbershttps://software.intel.com/en-us/articles/intel-architecture-and-processor-identification-with-cpuid-model-and-family-numbers
arch.intel <- function(model)
{
  if (model == 0x3C || model == 0x45 || model == 0x46)
  {
    arch <- "Haswell"
  }
  else if (model == 0x3A || model == 0x3E)
  {
    arch <- "IvyBdridge"
    #IvyBridge-EP #0x3E
  }
  else if (model == 0x2A || model == 0x2D)
  {
    arch <- "SandyBridge" #0x2A
    #SandyBridge-EP #0x2D
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
  
  ret <- list(codename=arch)
  return( ret )
}


