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
\item{game}{A coalitional game}
\item{coalition}{A vector of indices of players}
}
\examples{
\dontrun{
  weights = c(4, 5, 6, 14, 16, 17, 25, 26, 35, 36)
  quota = 55

  wvg = votingGame(weights, quota)
  value(wvg, c(0, 3, 4, 5, 8))
}
}
