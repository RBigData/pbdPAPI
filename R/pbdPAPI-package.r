#' Performance Analysis Tools for R
#' 
#' pbdPAPI is an R package which offers a set of R bindings for the Performance
#' Application Programming Interface (PAPI).  PAPI is a high-level C library
#' for gathering information from hardware performance counters.  PAPI measures
#' events like cache misses, flops, etc.
#' 
#' \tabular{ll}{ Package: \tab pbdPAPI\cr Type: \tab Package\cr License: \tab
#' BSD 2-Clause\cr }
#' 
#' @useDynLib papi_event_avail, papi_epc_on, papi_epc_off, papi_flips_on, 
#' papi_flips_off, papi_flops_on, papi_flops_off, papi_hwinfo, 
#' R_papi_cpuinfo, papi_hwname, papi_check_counters,
#' papi_event_counter_init, papi_event_counter_on,
#' papi_event_counter_off, papi_utilization_on, papi_utilization_off
#' 
#' @name pbdPAPI-package
#' @docType package
#' @author Drew Schmidt \email{schmidt AT math.utk.edu}, Christian Hckendorf,
#' and Wei-Chen Chen.
#' @references Programming with Big Data in R Website: \url{http://r-pbd.org/}
#' PAPI website: \url{http://icl.cs.utk.edu/PAPI/}
NULL

