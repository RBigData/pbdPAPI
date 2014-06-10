\name{system.mips}
\alias{system.mips}

\title{flops}
\description{
  Instructions per cycle.
}
\usage{
  system.ipc(expr, gcFirst=TRUE, burnin=TRUE)
}
\arguments{
  \item{expr}{A valid R expression to be profiled.}
  \item{gcFirst}{logical; determines if garbage collection should be
  called before profiling.}
  \item{burnin}{logical; determines if the function should first be
  evaluated with an empty expression.}
}
\details{
  .
}
\value{
  The return is a list consisting of:
  \tabular{ll}{
  \code{real_time} \tab real time spent evaluating expression \cr
  \code{proc_time} \tab total process time spent evaluating expression \cr
  \code{ins} \tab Number of instructions. \cr
  \code{ipc} \tab Instructions per cycle. \cr
}
}
\examples{
\dontrun{
library(pbdPAPI)

system.flops(1+1)
}
}
\keyword{programming}

