\name{system.flips}
\alias{system.flips}

\title{Measuring Floating Point Instructions}
\description{
  Measuring floating point instructions.
}
\usage{
  system.flips(expr, gcFirst=TRUE)
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
  \code{flpins} \tab FLoating Point INStructions (count) \cr
  \code{mflips} \tab Mflips (flpins per second) \cr
}
}
\examples{
\dontrun{
library(pbdPAPI)

system.flips(1+1)
}
}
\keyword{programming}

