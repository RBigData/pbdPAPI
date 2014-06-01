\name{system.idle}
\alias{system.idle}

\title{Idle}
\description{
  Measuring idle cycles.
}
\usage{
  system.idle(expr, events="float", gcFirst=TRUE)
}
\arguments{
  \item{expr}{A valid R expression to be profiled.}
  \item{events}{The class of cache events to profile; options are
  "float", "int", "numeric", "branch", "load", and "all".}
  \item{gcFirst}{logical; determines if garbage collection should be
  called before profiling.}
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

system.idle(1+1, events="float")
}
}
\keyword{programming}

