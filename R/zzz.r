### Lastest load into a package.

### Export Namespace does not use .First.lib() and .Last.lib(), but use
### .onLoad() and .onUnload().
# .First.lib <- function(lib, pkg){
# } # End of .First.lib().

# .Last.lib <- function(libpath){
# } # End of .Last.lib().

.onLoad <- function(libname, pkgname){
	### Shared library needs to be loaded before "pbdMPI.so"

	i.lib <- NULL
	libs <- paste("libs",c("libcipcm","libpcm"),sep="/")
	full.libs <- system.file(libs,"pbdPAPI")

	for(i in 1:length(full.libs)){
		ret <- try(dyn.load(full.libs[i]), silent = TRUE)
		if(class(ret) != "try-error"){
			i.lib <- libs[i]
			### CRAN doesn't allow this anymore. Use "pkg_initialize" instead.
			# assign(".__MPI_LIB__", i.libs, envir = .GlobalEnv)
			break
		}
	}

	library.dynam("pbdPAPI", pkgname, libname)

	invisible()
} # End of .onLoad().

.onUnload <- function(libpath){
  library.dynam.unload("pbdPAPI", libpath)

	libs <- paste("libs",c("libcipcm","libpcm"),sep="/")
	full.libs <- system.file(libs,"pbdPAPI")

    for(i.lib in full.libs){
      ret <- try(dyn.unload(i.lib), silent = TRUE)
    }
  invisible()
} # End of .onUnload().

.onAttach <- function(libname, pkgname){
#   if(interactive()){
#     pbdR.msg <-
# "
#     pbdR is mainly designed for batch environment or in SPMD programming.
#     Please utilize High Performance Computing Resource or consider an
#     allociation via ``http://www.nics.tennessee.edu/getting-an-allocation''
#     with XSEDE at ``http://www.xsede.org/overview''.
# "
#   } else{
#     pbdR.msg <-
# "
#     High Performance Computing Resource is available via
#     ``http://www.nics.tennessee.edu/getting-an-allocation''
#     with XSEDE at ``http://www.xsede.org/overview''.
# "
#   }
#   if(spmd.comm.rank(0L) == 0){
#     packageStartupMessage(pbdR.msg)
#   }
  #invisible()
  papi.avail.internal()
} # End of .onAttach().
