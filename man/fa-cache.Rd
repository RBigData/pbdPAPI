\name{system.cache}
\alias{system.cache}

\title{Cache}
\description{
  Measuring cache misses, hits, accesses, and reads.
}
\usage{
  system.cache(expr, type="miss", events="total", gcFirst=TRUE, burnin=TRUE)
}
\arguments{
  \item{expr}{A valid R expression to be profiled.}
  \item{type}{The type of cache event to profile; options are
  "miss", "hit", "access", and "read".}
  \item{events}{The class of cache events to profile; options are
  "total", "data", "instruction", "all", "l1.all", "l2.all", and 
  "l3.all".}
  \item{gcFirst}{logical; determines if garbage collection should be
  called before profiling.}
  \item{burnin}{logical; determines if the function should first be
  evaluated with an empty expression.}
}
\details{
  .
}
\value{
  The results of the requested PAPI events are returned, in a 
  named list, with values stored in double precision.
}
\examples{
\dontrun{
library(pbdPAPI)

system.cache(1+1, type="miss", events="l2.all")
}
}
\keyword{programming}

