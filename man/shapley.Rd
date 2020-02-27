\name{shapley}
\alias{shapley}
\docType{package}
\title{
  Shapley value of all players
}
\description{
  Return shapley value for every player in the given game.
  Optionally, it is possible to select only a subset of players and skip the computation for the rest.
}
\usage{
  shapley(game)
  shapley(game, players)
}
\arguments{
\item{game}{A coalitional game created by one of the functions (see TODO)}
\item{players}{A vector of indices of players for which we want the Shapley value computed. The rest will be set to -infinity. Index of the first player is 0.}
}
\examples{
\dontrun{
game = votingGame(c(4, 12, 14, 22, 25), 30)
shapley(game)
shapley(game, c(0, 2))
}
}
