//
// Created by maty on 05/02/20.
//

#ifndef COOPGAME_VOTING_MICROARRAY_H
#define COOPGAME_VOTING_MICROARRAY_H


#include "sum_of_voting.h"

class VotingMicroarray : public SumOfVoting {
public:
  
  VotingMicroarray(const matrix & special, const matrix & control);
  vector<int> expressionsToCoalition(const vector<double> & expressions);

  // true if special
  bool predict(const vector<int> & coal);
  
private:
  double avgMatrixVal(const matrix & mtx);
  vector<double> avgSpecial, avgControl;
  double avgSpecialVal, avgControlVal;
  matrix special, control;
};


#endif //COOPGAME_VOTING_MICROARRAY_H
