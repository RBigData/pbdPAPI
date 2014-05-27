library(pbdPAPI)
library(inline)

bad_cache_access <- "
  int i, j;
  const int n = INTEGER(n_)[0];
  Rcpp::NumericMatrix x(n, n);
  
  
  for (i=0; i<n; i++)
    for (j=0; j<n; j++)
      x(i, j) = 1.;
  
  return x;
"

good_cache_access <- "
  int i, j;
  const int n = INTEGER(n_)[0];
  Rcpp::NumericMatrix x(n, n);
  
  
  for (j=0; j<n; j++)
    for (i=0; i<n; i++)
      x(i, j) = 1.;
  
  return x;
"


bad <- cxxfunction(signature(n_="integer"), body=bad_cache_access, plugin="Rcpp")
good <- cxxfunction(signature(n_="integer"), body=good_cache_access, plugin="Rcpp")

n <- 10000L

system.cache(bad(n))
system.cache(good(n))
