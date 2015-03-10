hw.info.internal <- function()
{
  info <- .Call(papi_hwinfo)
  
  val <- info[[2L]]
  attr(x=val, which="names") <- info[[1L]]
  
  return(val)
}

cpuinfo <- function()
{
  .Call(R_papi_cpuinfo)
}



#' Hardware Information
#' 
#' This function returns some basic hardware information.
#' 
#' If you are submitting a bug to the pbdPAPI developers, please include the
#' output of \code{hw.info()}.  You may also be asked to submit the output of
#' \code{pbdPAPI:::hw.info.internal()}.
#' 
#' 
#' @return 
#' The return is a list consisting of: 
#' \tabular{ll}{ 
#'    \code{vendor} \tab Hardware vendor. \cr 
#'    \code{model} \tab CPU model. \cr 
#'    \code{codename} \tab CPU codename. \cr 
#'    \code{clockrate} \tab Clock rate in MHz. \cr 
#'    \code{ncpus} \tab The number of "cores" (physical + logical) detected. 
#' }
#' 
#' @examples
#' \dontrun{
#' library(pbdPAPI)
#' 
#' hw.info()
#' }
#' 
#' @export
hw.info <- function()
{
  intinfo <- hw.info.internal()
  info <- .Call(papi_hwname)
  
  attr(x=info[[2L]], which="names") <- info[[1L]]
  
  id.vendor <- info[[2L]]["vendor"]
  id.family <- intinfo["cpuid_family"]
  id.model <- intinfo["cpuid_model"]
  
  val <- c(info[[2L]], arch.lookup(id.vendor, id.family, id.model))
  
  val <- c(val, cpuinfo())
  
  return(val)
}


# -----------------------------------------------------------------------------
# Processor model lookup
# -----------------------------------------------------------------------------

arch.lookup <- function(vendor, family, model)
{
  if (vendor == "GenuineIntel")
   arch <- arch.intel(family, model)
  else if (vendor == "AuthenticAMD")
    arch <- arch.amd(family,model)
  else
    arch <- "Unknown"

  ret <- list(codename=arch)
  return(ret)
}

arch.amd <- function(family, model)
{
  #TODO
  warning("AMD chipsets are not supported at this time.")
  return ("Unknown AMD")
}

#http://www.cpu-world.com/cgi-bin/CPUID.pl
#https://software.intel.com/en-us/articles/intel-architecture-and-processor-identification-with-cpuid-model-and-family-numbershttps://software.intel.com/en-us/articles/intel-architecture-and-processor-identification-with-cpuid-model-and-family-numbers
arch.intel <- function(family, model)
{
  if (family == 0x0F && model == 0x06)
  {
    return("Presler")
  }

  if (family != 0x06)
  {
    return("Unknown")
  }

  if (model == 0x3C || model == 0x45 || model == 0x46)
  {
    arch <- "Haswell"
  }
  else if (model == 0x3A || model == 0x3E)
  {
    arch <- "IvyBridge"
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
    arch <- "Unknown Intel"
  }
  
  return(arch)
}


