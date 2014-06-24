\name{system.cpuormem}
\alias{system.cpuormem}

\title{Categorizing Computation as CPU or RAM Bound}
\description{
  This attmpts to declare a computation as compute or memory
  bound.  Some simplifying assumptions are made.  See the details
  section for more information.
}
\usage{
  system.idle(expr, events="float", gcFirst=TRUE, burnin=TRUE)
}
\arguments{
  \item{expr}{A valid R expression to be profiled.}
  \item{gcFirst}{logical; determines if garbage collection should be
  called before profiling.}
  \item{burnin}{logical; determines if the function should first be
  evaluated with an empty expression.}
}
\details{
  To make our determination, we measure the total number of floating
  point instructions and the total number of L2 cache accesses.
  If the number of floating point instructions is greater, then
  we say the computation is compute bound, and otherwise we say
  the computation is memory bound.
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

