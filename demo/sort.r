library(pbdPAPI)

x <- runif(1e6)

### Sorting is not a floating point operation.
system.flops(sort(x))

system.flops(x+1)

### It does require lots of memory access, though.
system.cache(sort(x))

system.utilization(sort(x))
