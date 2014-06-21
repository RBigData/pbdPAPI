\name{system.flops}
\alias{system.flops}

\title{flops}
\description{
  Measuring floating point operations.
}
\usage{
  system.flops(expr, gcFirst=TRUE, burnin=TRUE)
}
\arguments{
  \item{expr}{A valid R expression to be profiled.}
  \item{gcFirst}{logical; determines if garbage collection should be
  called before profiling.}
  \item{burnin}{logical; determines if the function should first be
  evaluated with an empty expression.}
}
\details{
  This function measures the evaluation of the provided expression,
  expr, for real time, process time, number of floating point
  operations, and floating point operation rate. The additional
  arguments, gcFirst and burnin, can be toggled to better simulate
  the target context.
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

