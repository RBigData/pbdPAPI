library(pbdPAPI)
library(Rcpp)

badcode <- "
#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::NumericMatrix bad(const int n)
{
  int i, j;
  Rcpp::NumericMatrix x(n, n);
  
  
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      x(i, j) = 1.;
  
  return x;
}
"

goodcode <- "
#include <Rcpp.h>

// [[Rcpp::export]]
Rcpp::NumericMatrix good(const int n)
{
  int i, j;
  Rcpp::NumericMatrix x(n, n);
  
  
  for (j=0; j<n; j++)
    for (i=0; i<n; i++)
      x(i, j) = 1.;
  
  return x;
}
"


### Compile, link, load into R
sourceCpp(code=badcode)
sourceCpp(code=goodcode)


n <- 10000L


### Summary of cache misses
system.cache(bad(n))
system.cache(good(n))

### Ratio of total cache misses to total cache accesses
system.cache(bad(n), events="l2.ratio")
system.cache(good(n), events="l2.ratio")
