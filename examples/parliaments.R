library(VotingGames)
source("../R/main.R")
# European parliament by parties
EUParliamentByParties = data.frame(
  parties = c("EEP", "S&D", "RE", "ID", "Greens-EFA", "ECR", "GUE/NGL", rep("NI", 29)),
  seats = c(187, 147, 98, 76, 67, 61, 39, rep(1, 29))
  )

quota = floor(sum(EUParliamentByParties$seats)/2) + 1
game = votingGame(EUParliamentByParties$seats, quota)

EUParliamentByParties$fractionOfSeats = EUParliamentByParties$seats / sum(EUParliamentByParties$seats)
EUParliamentByParties$shapley = shapley(game)
EUParliamentByParties$banzhaf = banzhaf(game)

print(EUParliamentByParties)


# House of Commons in the United Kingdom with ruling requiring 2/3 of the seats
UKHouseOfCommons = data.frame(
  parties = c("Conservative Party", "Labour Party", "Scottish National Party", "Liberal Democrats",
                          "Democratic Unionist Party", "Plaid Cymru", "SDLP", "Alliance Party", "Green Party",
                          "Independent"),
              seats = c(365, 202, 47, 11, 8, 4, 2, 1, 1, 1)
)

quota = ceiling(2*sum(UKHouseOfCommons$seats)/3)
game = votingGame(UKHouseOfCommons$seats, quota)

UKHouseOfCommons$shapley = shapley(game)
UKHouseOfCommons$banzhaf = banzhaf(game)

print(UKHouseOfCommons)


# Czech parliament

CzechParliament = data.frame(
  parties = c("ANO", "ODS", "Pirates", "SPD", "KSČM", "ČSSD", "KDU-ČSL", "TOP 09", "STAN", rep("Independent", 5)),
  seats = c(78, 23, 22, 20, 15, 14, 10, 7, 6, rep(1, 5))
)

quota = floor(sum(CzechParliament$seats)/2) + 1
game = votingGame(CzechParliament$seats, quota)

CzechParliament$fractionOfSeats = CzechParliament$seats / sum(CzechParliament$seats)
CzechParliament$shapley = shapley(game)
CzechParliament$banzhaf = banzhaf(game)

print(CzechParliament)
