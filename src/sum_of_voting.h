//
// Created by maty on 04/01/20.
//

#ifndef COOPGAME_SUM_OF_VOTING_H
#define COOPGAME_SUM_OF_VOTING_H

#include <bits/stdc++.h>
#include "voting_game.h"
#include "voting_nonunique.h"

using namespace std;

struct PlayerWeights {
  PlayerWeights(const vector<ll> &weights, int idx);
  PlayerWeights() = default;
  PlayerWeights(PlayerWeights &&) = default;
  PlayerWeights(const PlayerWeights &) = default;
  PlayerWeights & operator = (const PlayerWeights &) = default;

  vector<ll> weights; // TODO: optimize this with a reference
  int idx;

  bool operator < (const PlayerWeights & o) const;
};


class SumOfVoting: public CoalGame {
public:
  SumOfVoting(const vector<vector<ll>> & weights, const vector<ll> & quotas);
  /*SumOfVoting(const vector<VotingNonunique> & games): CoalGame(games[0].players), gamesNonunique(games) {
  }*/
  SumOfVoting(int players): CoalGame(players) {
  }
  virtual double v(const vector<int> & coal);
  vector<double> banzhaf() override;
  double banzhaf(int player);
  vector<double> shapley() override;
  double shapley(int player);
  vector<double> shapleyTop(int topN);
  vector<double> banzhafTop(int topN);

  // doesn't return less than numberOfTopPlayers
  vector<int> getTopPlayers(const vector<vector<ll>> & weights, int numberOfTopPlayers);

protected:
  vector<VotingNonunique> gamesNonunique;
  vector<VotingGame> gamesUnique;
  vector<vector<ll>> getWeights();
};


#endif //COOPGAME_SUM_OF_VOTING_H
