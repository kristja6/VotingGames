\name{shapleyTop}
\alias{shapleyTop}
\docType{package}
\title{
  Shapley values of the strongest players
}
\description{
  Returns Shapley values of at least topN strongest players. The rest are assigned the value of -infinity.
}
\usage{
  banzhafTop(game, topN)
  banzhafTop(game, topN)
}
\arguments{
\item{game}{A coalitional game}
\item{topN}{Number of top players for which we want the Shapley value}
}
\examples{
\dontrun{
game = votingGame(c(4, 12, 14, 22, 25), 30)
shapleyTop(game, 2)
}
}
