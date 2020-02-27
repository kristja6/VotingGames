\name{value}
\alias{value}
\docType{package}
\title{
  Value of coalition in a game.
}
\description{
  Returns value of coalition in a game.
}
\usage{
  value(game, coalition)
}
\arguments{
\item{game}{A coalitional game created by one of the functions (see TODO)}
\item{players}{A vector of indices of players for which we want the Banzhaf index computed. The rest will be set to -infinity. Index of the first player is 0.}
}
\examples{
\dontrun{
game = votingGame(c(4, 12, 14, 22, 25), 30)
value(game, c(0, 3))
}
}
