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
  vector<double> banzhaf();

protected:
  vector<VotingGame> games;
};


#endif //COOPGAME_SUM_OF_VOTING_H
