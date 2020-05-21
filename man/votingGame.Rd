\name{votingGame}
\alias{votingGame}
\docType{package}
\title{
  Creates a weighted voting game.
}
\description{
  Weighted voting game defined by assignment of a weight to each player and a quota.
  The value of a coalition is 1 if the total weight of the coalition is at least the quota.
  Otherwise it is zero.
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
