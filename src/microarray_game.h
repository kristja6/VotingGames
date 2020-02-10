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
  MicroarrayGame(const vector<std::set<int>> & checks): CoalGame(checks.size()), checks(checks) {
  }
  // create from a boolean matrix, where each check is a column, each row a player.
  // 1 means that the player is required in the check
  MicroarrayGame(const vector<vector<int>> & mtx);
  virtual double v(const vector<int> & coalition);

  vector<double> banzhaf() override ;
  vector<double> shapley() override ;
  double banzhafInteraction();

  // assume group 0 is control and use the mean and sd of that. All groups 1 ... k are the studied groups
  /*static vector<vector<vector<int>>> expressionsToFeaturesStd2Groups(const vector<vector<double>>> & mtx) {
    vector<vector<vector<int>>> res(mtx.size() - 1);
    for (int k = 1; k < mtx.size(); ++k) {
      res[k-1] = vector<vector<int>>(mtx[k].size(), vector<int>(mtx[k][0].size(), 0));
    }
    for (int i = 0; i < mtx[0].size(); ++i) {
      double u = mean(mtx[0][i]);
      double s = sd(mtx[0][i], u);
      for (int k = 1; k < mtx.size(); ++k) {
        for (int j = 0; j < mtx[k][i].size(); ++j) {
          if (fabs(mtx[k][i][j] - u) >= 2*s) res[k-1][i][j] = 1;
        }
      }
    }
    return res;
  }*/
  static vector<vector<int>> expressionsToFeaturesStd2Groups(const vector<vector<double>> &, const vector<vector<double>> &);
private:
  vector<std::set<int>> checks;
};


#endif //COAL_GAME_MICROARRAY_GAME_H
