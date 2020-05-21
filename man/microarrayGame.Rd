\name{microarrayGame}
\alias{microarrayGame}
\docType{package}
\title{
  Create a microarray game
}
\description{
  Microarray as described by Moretti and other.
}
\usage{
  microarrayGame(checks)
}
\arguments{
\item{checks}{Set of players in each check. Alternatively, it can be understood as the set of players defining each individual unanimity game.}
}
\examples{
\dontrun{
  checks = rbind(c(1, 0, 1, 1), c(0, 1, 0, 0))
  game = microarrayGame(checks)
  shapley(game)
}
}
