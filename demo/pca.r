library(pbdPAPI)

### ~4MiB
m <- 10000
n <- 50

x <- matrix(rnorm(m*n), m, n)

### Centering requires 2mn+1 ops
### SVD requires 6mn^2 + 20n^3 ops
### x %*% svd$v requires 2*m*n^2 ops
op <- expression(prcomp(x, center=TRUE, scale.=FALSE))

report <- system.flops(eval(op))
count <- report$flpops
theoretical <- 6*m*n^2 + 20*n^3 + 2*m*n^2 + 2*m*n + 1
mflops <- report$mflops

data.frame(m=m, n=n, measured=count, theoretical=theoretical, difference=count-theoretical, "pct.error"=(1.0 - (theoretical/count))*100, mflops=mflops)

