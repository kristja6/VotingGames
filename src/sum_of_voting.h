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
  SumOfVoting(const matrix & special, const matrix & control);
  virtual ll v(const vector<int> & coal);
  vector<double> banzhaf() override;
  vector<double> shapley() override;
  vector<int> expressionsToCoalition(const vector<double> & expressions);

  // true if special
  bool predict(const vector<int> & coal);

protected:
  vector<VotingGame> games;
  double avgMatrixVal(const matrix & mtx);
private:
  vector<double> avgSpecial, avgControl;
  matrix special, control;
};


#endif //COOPGAME_SUM_OF_VOTING_H
