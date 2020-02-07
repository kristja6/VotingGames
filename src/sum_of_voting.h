//
// Created by maty on 04/01/20.
//

#ifndef COOPGAME_SUM_OF_VOTING_H
#define COOPGAME_SUM_OF_VOTING_H

#include <bits/stdc++.h>
#include "voting_game.h"
#include "voting_nonunique.h"

using namespace std;

class SumOfVoting: public CoalGame<ll> {
public:
  SumOfVoting(const vector<vector<ll>> & weights, const vector<ll> & quotas);
  SumOfVoting(const vector<VotingNonunique> & games): CoalGame(games[0].players), games(games) {
  }
  SumOfVoting(int players): CoalGame(players) {
  }
  virtual ll v(const vector<int> & coal);
  vector<double> banzhaf() override;
  vector<double> shapley() override;

protected:
  vector<VotingNonunique> games;
};


#endif //COOPGAME_SUM_OF_VOTING_H
