library(pbdPAPI)

### Reimplementation of PAPI's FlopsInnerProduct.m in R

n <- 50

x <- matrix(rnorm(n), n, 1)
y <- matrix(rnorm(n), n, 1)

op <- expression(crossprod(x,y))

report <- papi.flops(eval(op))
count <- report$flpops
theoretical <- 2*n
mflops <- report$mflops

data.frame(n=n, measured=count, theoretical=theoretical, difference=count-theoretical, "pct.error"=(1.0 - (theoretical/count))*100, mflops=mflops)

