.onLoad <- function(libname, pkgname)
{
  ### Intel PCM needs to be loaded before "pbdPAPI.so"

  i.lib <- NULL
  libs <- paste("libs",c("libcipcm","libpcm"),sep="/")
  full.libs <- system.file(libs,"pbdPAPI")
  
  for(i in 1:length(full.libs))
  {
    ret <- try(dyn.load(full.libs[i]), silent = TRUE)
    if(class(ret) != "try-error")
    {
      i.lib <- libs[i]
      break
    }
  }
  
  library.dynam("pbdPAPI", pkgname, libname)
  
  papi.avail.internal()
  
  invisible()
}



.onUnload <- function(libpath)
{
  library.dynam.unload("pbdPAPI", libpath)
  
  libs <- paste("libs",c("libcipcm","libpcm"),sep="/")
  full.libs <- system.file(libs,"pbdPAPI")
  
    for(i.lib in full.libs){
      ret <- try(dyn.unload(i.lib), silent = TRUE)
    }
  invisible()
}



.onAttach <- function(libname, pkgname)
{
  invisible()
}
