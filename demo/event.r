library(pbdPAPI)

events <- c("PAPI_L1_DCM","PAPI_L2_DCM")

### High-level interface
papi.event(1+1, events)


### Low-level interface --- you have to check your own errors
papi.start(events)
1+1
papi.stop(events)
