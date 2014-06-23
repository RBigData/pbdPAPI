\name{system.epc}
\alias{system.epc}

\title{epc}
\description{
  Events per cycle.
}
\usage{
  system.epc(expr, event, gcFirst=TRUE, burnin=TRUE)
}
\arguments{
  \item{expr}{A valid R expression to be profiled.}
  \item{event}{A PAPI event counter.}
  \item{gcFirst}{logical; determines if garbage collection should be
  called before profiling.}
  \item{burnin}{logical; determines if the function should first be
  evaluated with an empty expression.}
}
\details{
  This function measures the evaluation of the provided expression,
  expr, for real time, process time, reference clock cycles since
  last call, core clock cycles since last call, total events since
  first call, and incremental events per cycle since last call. The
  additional arguments, gcFirst and burnin, can be toggled to better
  simulate the target context.
}
\value{
  The return is a list consisting of:
  \tabular{ll}{
  \code{real_time} \tab real time spent evaluating expression \cr
  \code{proc_time} \tab total process time spent evaluating expression \cr
  \code{ref} \tab Incremental reference clock cycles since last call. \cr
  \code{core} \tab Incremental clock cycles since last call. \cr
  \code{evt} \tab Total events since first call. \cr
  \code{epc} \tab Incremental events per cycle since last call. \cr
}
}
\examples{
\dontrun{
library(pbdPAPI)

system.epc(1+1,"PAPI_L1_DCM")
}
}
\keyword{programming}

