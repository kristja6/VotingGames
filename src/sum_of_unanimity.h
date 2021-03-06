//
// Created by maty on 08/12/19.
//

#ifndef COAL_GAME_MICROARRAY_GAME_H
#define COAL_GAME_MICROARRAY_GAME_H

#include "coalitional_game.h"
#include "math.h"


class SumOfUnanimity : public CoalitionalGame {
public:
  SumOfUnanimity(const vector<std::set<int>> & checks): CoalitionalGame(checks.size()), checks(checks) {
  }
  // create from a boolean matrix, where each check is a row, each column a player.
  // 1 means that the player is required in the check
  SumOfUnanimity(const vector<vector<int>> & mtx);
  virtual double v(const vector<int> & coalition);

  vector<double> banzhaf() override ;
  vector<double> shapley() override ;
private:
  vector<std::set<int>> checks;
};


#endif //COAL_GAME_MICROARRAY_GAME_H
