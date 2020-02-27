\name{microarrayGame}
\alias{microarrayGame}
\docType{package}
\title{
  Create a microarray game
}
\description{
  Microarray as described by Moretti and other in [...] (TODO)
  (TODO: zjistit jak ze to vlastne funguje)
}
\usage{
  votingGame(weights, quota)
}
\arguments{
\item{weights}{A vector of integer weights for each player}
\item{quota}{Quota which must be surpassed for a motion to pass}
}
\examples{
\dontrun{
game = votingGame(c(4, 12, 14, 22, 25), 30)
}
}
