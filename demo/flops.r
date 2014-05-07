library(pbdPAPI, quietly=TRUE)

x <- matrix(rnorm(1e6), ncol=250)

papi_get_flops(cov(x))
