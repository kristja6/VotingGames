\name{votingGame}
\alias{votingGame}
\docType{package}
\title{
  Creates a weighted voting game.
}
\description{
  Weighted voting game is a ... (TODO)
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
