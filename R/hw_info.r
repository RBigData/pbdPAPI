get.os <- function()
{
  ret <- Sys.info()[1L]
  names(ret) <- NULL
  
  return( ret )
}

hw.info <- function()
{
  info <- .Call("papi_hwinfo")
  val <- info[[2]]
  attr(x=val, which="names") <- info[[1]]
  return(val)
}

hw.info.name <- function()
{
  info <- .Call("papi_hwname")
  val <- info[[2]]
  attr(x=val, which="names") <- info[[1]]
  return(val)
}

system.hw.info <- function()
{
  os <- get.os()
  
  if (os == "Linux")
    return( hw.info.linux() )
  else
    stop(paste("function hw.info() does not support platform", os, "at this stime"))
}



### Linux
grep.cpuinfo <- function(info, x)
{
  x.which <- grep(x=info, x)
  
  ret <- gsub(x=info[x.which], pattern=paste(x, "\t: ", sep=""), replacement="")
  
  return( ret )
}

hw.info.linux <- function()
{
  info <- readLines("/proc/cpuinfo")
  
  ncores.phys <- grep.cpuinfo(info=info, x="cpu cores")[1L]
  ncores.log <- length(grep.cpuinfo(info=info, x="processor"))
  vendor.id <- grep.cpuinfo(info=info, x="vendor_id")[1L]
  cpu.family <- grep.cpuinfo(info=info, x="cpu family")[1L]
  cpu.model <- grep.cpuinfo(info=info, x="model\t")[1L]
  cpu.model.name <- grep.cpuinfo(info=info, x="model name")[1L]
  
#  archname <- hw.arch(vendor=vendor.id)
  
#  ret <- list(ncores=ncores)
#  return( ret )
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








