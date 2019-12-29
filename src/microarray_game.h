//
// Created by maty on 08/12/19.
//

#ifndef COAL_GAME_MICROARRAY_GAME_H
#define COAL_GAME_MICROARRAY_GAME_H

#include "coal_game.h"
#include "math.h"

class MicroarrayGame : public CoalGame<double> {
public:
  // format of input:
  /* items checks
   * check_1_size check_1,1 check_1,2 check_1,3
   * check_2_size check_2,1 check_2,2
   */
  MicroarrayGame(istream &, bool sparse = true);
  MicroarrayGame(const vector<set<int>> & checks): CoalGame(checks.size()), checks(checks) {
  }
  // create from a boolean matrix, where each check is a column, each row a player.
  // 1 means that the player is required in the check
  MicroarrayGame(const vector<vector<int>> & mtx):
    CoalGame(mtx.size()), checks(vector<set<int>>(mtx[0].size())) {
    for (int i = 0; i < mtx.size(); ++i) {
      for (int j = 0; j < mtx[i].size(); ++j) {
        if (mtx[i][j] == 1) checks[j].insert(i);
      }
    }
  }
  virtual double v(const vector<int> & coalition);

  vector<double> banzhaf();
  vector<double> shapley();
private:
  vector<set<int>> checks;
};


#endif //COAL_GAME_MICROARRAY_GAME_H
