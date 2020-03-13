library(VotingGames)

#### Weighted voting games #####
wvg = votingGame(c(4, 5, 6, 14, 16, 17, 25, 26, 35, 36), 55)
shapley(wvg)
shapleyTop(wvg, 4)
banzhaf(wvg)
banzhafTop(wvg, 4)


#### Sum of voting games ####
game1 = 1:5
game2 = c(1,1,1,1,1)
game3 = c(4, 1, 1, 1, 1)
svg = sumOfVotingGames(rbind(game1, game2, game3), c(7, 5, 4))
shapley(svg)
shapleyTop(svg, 1)
banzhaf(svg, denom = "subsets")
banzhafTop(svg, 1)


#### Microarray games ####
checks = rbind(c(1, 0, 1, 1), c(0, 1, 0, 0))
mg = microarrayGame(checks)
shapley(mg)
banzhaf(mg)
