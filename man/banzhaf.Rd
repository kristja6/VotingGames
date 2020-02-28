\name{banzhaf}
\alias{banzhaf}
\docType{package}
\title{
  Banzhaf index of all players
}
\description{
  Return banzhaf index for every player in the given game.
  Optionally, it is possible to select only a subset of players and skip the computation for the rest.
}
\usage{
  banzhaf(game)
  banzhaf(game, players)
  banzhaf(game, players, denom)
}
\arguments{
\item{game}{A coalitional game created by one of the functions (see TODO)}
\item{players}{A vector of indices of players for which we want the Banzhaf index computed. The rest will be set to -infinity. Index of the first player is 0.}
\item{denom}{Must equal to 'swings' or 'subsets'.
Banzhaf index is essentially the number of swinging coalitions normalized by a denominator common for all players.
The default one is the total number of swings over all players (swings).
The alternative is the number of subsets of all players, that is 2^players.
}
}
\examples{
\dontrun{
game = votingGame(c(4, 12, 14, 22, 25), 30)
banzhaf(game)
banzhaf(game, c(0, 2))
}
}
