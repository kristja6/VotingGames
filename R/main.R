microarrayGame <- function(special, control) {
  res <- structure(list(
      "special" = special
      , "control" = control
      , "checks" = expressionsToFeaturesStd(special, control)
    )
    , "players" = nrow(special)
    , class = "microarrayGame")
  return(res)
}

votingGame <- function(weights, quota) {
  res <- structure(list(
    "weights" = weights
    , "quota" = quota
    , "players" = length(weights)
  )
  , class = "votingGame")
  return(res)
}

sumOfVotingGames <- function(weights, quotas) {
  res <- structure(list(
    "weights" = weights
    , "quotas" = quotas
    , "players" = ncol(weights)
  )
  , class = "sumOfVotingGames")
  return(res)
}

banzhaf <- function(x, players = c(), denom = "swings") UseMethod("banzhaf")
shapley <- function(x, players = c()) UseMethod("shapley")
banzhafTop <- function(x, topN) UseMethod("banzhafTop")
shapleyTop <- function(x, topN) UseMethod("shapleyTop")
value <- function(x, coalition) UseMethod("value")

getPlayers <- function(game, players = c()) {
  if (length(players) == 0) return(1:game$players)
  else return(players)
}

banzhaf.microarrayGame <- function(game, players = c(), denom = "swings") {
  players = getPlayers(game, players)
  return(microarrayBanzhaf(game$checks))
}

shapley.microarrayGame <- function(game, players = c()) {
  players = getPlayers(game, players)
  return(microarrayShapley(game$checks))
}

value.microarrayGame <- function(game, coalition) {
  return(microarrayValue(game$checks, coalition))
}

banzhaf.votingGame <- function(game, players = c(), denom = "swings") {
  players = getPlayers(game, players)
  return(votingBanzhaf(game$weights, game$quota, players, denom))
}

shapley.votingGame <- function(game, players = c()) {
  players = getPlayers(game, players)
  return(votingShapley(game$weights, game$quota, players))
}

banzhafTop.votingGame <- function(game, topN, denom = "subsets") {
  return(votingBanzhafTop(game$weights, game$quota, topN, denom))
}

shapleyTop.votingGame <- function(game, topN) {
  return(votingShapleyTop(game$weights, game$quota, topN))
}

value.votingGame <- function(game, coalition) {
  return(votingVal(game$weights, game$quota, coalition))
}

banzhaf.sumOfVotingGames <- function(game, players = c(), denom = "swings") {
  players = getPlayers(game, players)
  return(sumOfVotingBanzhaf(game$weights, game$quotas, players, denom))
}

shapley.sumOfVotingGames <- function(game, players = c()) {
  players = getPlayers(game, players)
  return(sumOfVotingShapley(game$weights, game$quotas, players))
}

banzhafTop.sumOfVotingGames <- function(game, topN, denom = "subsets") {
  return(sumOfVotingBanzhafTop(game$weights, game$quotas, topN, denom))
}

shapleyTop.sumOfVotingGames <- function(game, topN) {
  return(sumOfVotingShapleyTop(game$weights, game$quotas, topN))
}

value.sumOfVotingGames <- function(game, coalition) {
  return(sumOfVotingValue(game$weights, game$quotas, coalition))
}
