library(pbdPAPI)

### ~40MiB
m <- 50000
n <- 100

x <- matrix(rnorm(m*n), m, n)
y <- matrix(rnorm(m), m, 1)

op <- expression(lm.fit(x=x, y=y))

report <- papi.flops(eval(op))
count <- report$flpops
theoretical <- 2*n^2*(m-n/3)
mflops <- report$mflops

data.frame(m=m, n=n, measured=count, theoretical=theoretical, difference=count-theoretical, "pct.error"=(1.0 - (theoretical/count))*100, mflops=mflops)

