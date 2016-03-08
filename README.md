# pbdPAPI

* **Version:** 0.3-2
* **URL**: https://github.com/RBigData/pbdPAPI
* **License:** [![License](http://img.shields.io/badge/license-BSD%203--Clause-orange.svg?style=flat)](http://opensource.org/licenses/BSD-3-Clause)
* **Author:** See section below.


pbdPAPI is an R package which offers a set of R bindings for the Performance
Application Programming Interface (PAPI).  PAPI is a high-level C library for 
gathering information from hardware performance counters.  PAPI measures events 
like cache misses, flops, etc.  This allows for very powerful profiling of R 
code (compiled code via, for example, Rcpp, or even just plain old vanilla R), 
but with a very simple interface.

For more information about PAPI, see the official website: 
http://icl.cs.utk.edu/PAPI/



## Installation

pbdPAPI requires R version 2.14.0 or higher.  It also requires the PAPI library,
but a distribution of PAPI v 5.3.0 is bundled with the package for those who
do not already have a system installation available.  For configuration options,
and installation instructions, see the package vignette.

Unfortunately, at this time, PAPI is not supported on Windows or Mac
As such, pbdPAPI does not support these platforms either.

For a complete list of platforms which PAPI supports, see:
http://icl.cs.utk.edu/papi/custom/index.html?lid=62&slid=96



## Notes

If your hardware is very old/new/esoteric, pbdPAPI may build, but be
inable to do any profiling due to a lack of support from the PAPI library.
If every value of `papi.avail()$avail` is `FALSE`, then this is likely
the cause.  It could be that your platform does not support hardware
counters of any kind, however.

AVX instructions may not be monitored correctly (or at all), so profiling
with `system.flops()` with, for example, OpenBLAS, may produce wildly
low flops counts.

Those with Intel Sandy Bridge and/or Intel Ivy Bridge architecutres should be 
aware that flops counts are unreliable on these platforms.  This is a problem 
with the hardware returning incorrect values, not with PAPI or pbdPAPI.  For 
more details, see: 
http://icl.cs.utk.edu/projects/papi/wiki/PAPITopics:SandyFlops

pbdPAPI only supports serial applications.  For parallel codes, see pbdPROF, and
or consider using PAPI/TAU directly in your compiled code.  For more details, 
see:  http://icl.cs.utk.edu/projects/papi/wiki/Threads



## Usage

Like its parent library PAPI, the pbdPAPI package has 2 distinct
interfaces, a low-level and a high-level.



```r
### ~4MiB
m <- 10000
n <- 50

x <- matrix(rnorm(m*n), m, n)

### Centering requires 2mn+1 operations
### SVD requires 6mn^2 + 20n^3 operations
### x %*% svd$v requires 2*m*n^2 operations
op <- expression(prcomp(x, center=TRUE, scale.=FALSE))

report <- system.flops(eval(op))
count <- report$flpops
theoretical <- 6*m*n^2 + 20*n^3 + 2*m*n^2 + 2*m*n + 1
mflops <- report$mflops

data.frame(m=m, n=n, measured=count, theoretical=theoretical, difference=count-theoretical, "pct.error"=(1.0 - (theoretical/count))*100, mflops=mflops)
#       m  n  measured theoretical difference pct.error   mflops
# 1 10000 50 218789742   203500001   15289741  6.988326 1107.302
```


## Authors

pbdPAPI is authored and maintained by:
* Drew Schmidt
* Christian Heckendorf
* Wei-Chen Chen

For copyrights on the shipped PAPI code, see
`pbdPAPI/inst/PAPI_LICENSE.txt`.

