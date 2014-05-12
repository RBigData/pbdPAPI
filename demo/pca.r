library(pbdPAPI)

### Reimplementation of PAPI's FlopsMatrixMatrix.m in R

### ~4MiB
m <- 10000
n <- 50

x <- matrix(rnorm(m*n), m, n)

### SVD requires 6mn^2 + 20n^3 flops
### x %*% svd$v requires 2*m*n^2 flops
op <- expression(prcomp(x, center=FALSE, scale.=FALSE))

report <- papi.flops(eval(op))
count <- report$flpops
theoretical <- 6*m*n^2 + 20*n^3 + 2*m*n^2
mflops <- report$mflops

data.frame(m=m, n=n, measured=count, theoretical=theoretical, difference=count-theoretical, "pct.error"=(1.0 - (theoretical/count))*100, mflops=mflops)

