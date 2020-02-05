//
// Created by maty on 04/01/20.
//

#ifndef COOPGAME_SUM_OF_VOTING_H
#define COOPGAME_SUM_OF_VOTING_H

#include <bits/stdc++.h>
#include "voting_game.h"
using namespace std;

class SumOfVoting: public CoalGame<ll> {
public:
  SumOfVoting(vector<vector<ll>> & weights, vector<ll> & quotas): CoalGame(weights[0].size()) {
    assert(weights.size() == quotas.size());
    for (int i = 0; i < weights.size(); ++i) {
      games.push_back(VotingGame(weights[i], quotas[i]));
    }
  }
  SumOfVoting(const vector<VotingGame> & games): CoalGame(games[0].players), games(games) {
  }
  SumOfVoting(int players): CoalGame(players) {
  }
  virtual ll v(const vector<int> & coal);
  vector<double> banzhaf() override;
  vector<double> shapley() override;

protected:
  vector<VotingGame> games;
};


#endif //COOPGAME_SUM_OF_VOTING_H
