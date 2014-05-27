library(pbdPAPI)

events <- c("PAPI_L1_DCM","PAPI_L2_DCM")

### High-level interface
system.event(1+1, events)


### Low-level interface --- you have to check your own errors
ret <- papi.start(events)
my.expr <- 1+1
papi.stop(events)
