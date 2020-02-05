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

banzhaf <- function(x) UseMethod("banzhaf")
shapley <- function(x) UseMethod("shapley")
value <- function(x) UseMethod("value")

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

value.votingMicroarray <- function(game, coalition) {
  return(votingMicroarrayValue(game$special, game$control, coalition))
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
