library(VotingGames)

#### Weighted voting games #####
weights = c(4, 5, 6, 14, 16, 17, 25, 26, 35, 36)
quota = 55

wvg = votingGame(weights, quota)

shapley(wvg)
banzhaf(wvg, denom = "subsets")
banzhaf(wvg, denom = "swings")


#### Sum of voting games ####
# the weights in the individual voting games
game1 = 1:5
game2 = c(1,1,1,1,1)
game3 = c(4, 1, 1, 1, 1)
quotas = c(7, 5, 4) # quota for each of the voting games

svg = sumOfVotingGames(rbind(game1, game2, game3), quotas)

shapley(svg)
banzhaf(svg)


#### Microarray games ####
#checks = rbind(c(1, 0, 1, 1), c(0, 1, 0, 0))
checks = rbind(c(1, 0, 1, 1), c(0, 1, 0, 0))

mg = microarrayGame(checks)

shapley(mg)
banzhaf(mg)
