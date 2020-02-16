microarrayGame <- function(special, control) {
  res <- structure(list(
    "special" = special
    , "control" = control
    , "checks" = expressionsToFeaturesStd(special, control))
    , class = "microarrayGame")
  return(res)
}

votingMicroarray <- function(special, control) {
  res <- structure(list(
    "special" = special
    , "control" = control
    )
  , class = "votingMicroarray")
  return(res)
}

votingGame <- function(weights, quota) {
  res <- structure(list(
    "weights" = weights
    , "quota" = quota
    )
  , class = "votingGame")
  return(res)
}

sumOfVotingGames <- function(weights, quotas) {
  res <- structure(list(
    "weights" = weights
    , "quotas" = quotas
    )
  , class = "sumOfVotingGames")
  return(res)
}

banzhaf <- function(x) UseMethod("banzhaf")
shapley <- function(x) UseMethod("shapley")
banzhafTop <- function(x, topN) UseMethod("banzhafTop")
shapleyTop <- function(x, topN) UseMethod("shapleyTop")
value <- function(x, coalition) UseMethod("value")

banzhaf.microarrayGame <- function(game) {
  return(microarrayBanzhaf(game$checks))
}

shapley.microarrayGame <- function(game) {
  return(microarrayShapley(game$checks))
}

value.microarrayGame <- function(game, coalition) {
  return(microarrayValue(game$checks, coalition))
}

banzhaf.votingMicroarray <- function(game) {
  return(votingMicroarrayBanzhaf(game$special, game$control))
}

shapley.votingMicroarray <- function(game) {
  return(votingMicroarrayShapley(game$special, game$control))
}

banzhafTop.votingMicroarray <- function(game, topN) {
  return(votingMicroarrayBanzhafTop(game$special, game$control, topN))
}

shapleyTop.votingMicroarray <- function(game, topN) {
  return(votingMicroarrayShapleyTop(game$special, game$control, topN))
}

value.votingMicroarray <- function(game, coalition) {
  return(votingMicroarrayValue(game$special, game$control, coalition))
}

predict.votingMicroarray <- function(game, expressions) {
  return(votingMicroarrayPredict(game$special, game$control, expressions))
}

banzhaf.votingGame <- function(game) {
  return(votingBanzhaf(game$weights, game$quota))
}

shapley.votingGame <- function(game) {
  return(votingShapley(game$weights, game$quota))
}

value.votingGame <- function(game, coalition) {
  return(votingVal(game$weights, game$quota, coalition))
}

banzhaf.sumOfVotingGames <- function(game) {
  return(sumOfVotingBanzhaf(game$weights, game$quotas))
}

shapley.sumOfVotingGames <- function(game) {
  return(sumOfVotingShapley(game$weights, game$quotas))
}

value.sumOfVotingGames <- function(game, coalition) {
  return(sumOfVotingValue(game$weights, game$quotas, coalition))
}
