//
// Created by maty on 04/01/20.
//

#ifndef COOPGAME_SUM_OF_VOTING_H
#define COOPGAME_SUM_OF_VOTING_H

#include <vector>
#include "voting_game.h"

using namespace std;

struct PlayerWeights {
  PlayerWeights(const vector<int> &weights, int idx);
  PlayerWeights() = default;
  PlayerWeights(PlayerWeights &&) = default;
  PlayerWeights(const PlayerWeights &) = default;
  PlayerWeights & operator = (const PlayerWeights &) = default;

  vector<int> weights; // TODO: optimize this with a reference
  int idx;

  bool operator < (const PlayerWeights & o) const;
};

class SumOfVoting: public CoalitionalGame {
public:
  SumOfVoting(const vector<vector<int>> & weights, const vector<int> & quotas);
  SumOfVoting(int players): CoalitionalGame(players) {
  }
  virtual double v(const vector<int> & coal);
  vector<double> banzhaf() override;
  double banzhaf(int player);
  vector<double> shapley() override;
  double shapley(int player);
  vector<double> shapleyTop(int topN);
  vector<double> banzhafTop(int topN);
  void setBanzhafDenominator(int denom) override;

  // doesn't return less than numberOfTopPlayers
  vector<int> getTopPlayers(const vector<vector<int>> & weights, int numberOfTopPlayers);

protected:
  vector<VotingGame> gamesNonunique;
  vector<vector<int>> getWeights();
};


#endif //COOPGAME_SUM_OF_VOTING_H
