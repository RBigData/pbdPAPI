\name{hw.info}
\alias{hw.info}

\title{Hardware Information}
\description{
  This function returns some basic hardware information.  
  
  If you are submitting a bug to the pbdPAPI developers, please
  include the output of \code{hw.info()}.  You may also be asked to
  submit the output of \code{pbdPAPI:::hw.info.internal()}.
}
\usage{
  hw.info()
}
}
\value{
  The return is a list consisting of:
  \tabular{ll}{
  \code{vendor} \tab Hardware vendor. \cr
  \code{model} \tab CPU model. \cr
  \code{codename} \tab CPU codename. \cr
  \code{clockrate} \tab Clock rate in MHz. \cr
  \code{ncpus} \tab The number of "cores" (physical + logical) detected.
}
}
\examples{
\dontrun{
library(pbdPAPI)

hw.info()
}
}
