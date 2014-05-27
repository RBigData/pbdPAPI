\name{system.flops}
\alias{system.flops}

\title{flops}
\description{
  Measuring floating point operations.
}
\usage{
  system.flops(expr, gcFirst=TRUE)
}
\arguments{
  \item{expr}{A valid R expression to be profiled.}
  \item{gcFirst}{logical; determines if garbage collection should be
  called before profiling.}
}
\details{
  .
}
\value{
  The return is a list consisting of:
  \tabular{ll}{
  \code{real_time} \tab real time spent evaluating expression \cr
  \code{proc_time} \tab total process time spent evaluating expression \cr
  \code{flpops} \tab FLoating Point OPerationS (count) \cr
  \code{mflops} \tab Mflops (flpops per second) \cr
}
}
\examples{
\dontrun{
library(pbdPAPI)

system.flops(1+1)
}
}
\keyword{programming}

