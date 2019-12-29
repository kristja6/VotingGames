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
  MicroarrayGame(istream & in);
  virtual double v(const vector<int> & coalition);

  vector<double> banzhaf();
  vector<double> shapley();
private:
  vector<set<int>> checks;
};


#endif //COAL_GAME_MICROARRAY_GAME_H
