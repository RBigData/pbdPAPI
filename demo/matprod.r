library(pbdPAPI)

### Reimplementation of PAPI's FlopsMatrixMatrix.m in R

n <- 50

x <- matrix(rnorm(n*n), n, n)
y <- matrix(rnorm(n*n), n, n)
z <- matrix(rnorm(n*n), n, n)

op <- expression(z + x %*% y)

report <- system.flops(eval(op))
count <- report$flpops
theoretical <- 2*n^3
mflops <- report$mflops

data.frame(n=n, measured=count, theoretical=theoretical, difference=count-theoretical, "pct.error"=(1.0 - (theoretical/count))*100, mflops=mflops)

