//
// Created by maty on 07/02/20.
//

#ifndef COOPGAME_VOTING_NONUNIQUE_H
#define COOPGAME_VOTING_NONUNIQUE_H


#include "coal_game.h"
#include "voting_game.h"
#include <NTL/ZZX.h>
using namespace NTL;

class VotingNonunique : public CoalGame<ll> {
public:
  VotingNonunique(vector<ll> weights, ll quota);

  ll v(const vector<int> &coalition) override;

  ZZX columnWithOne(ll weight, ll count);
  ZZX addToColumn(const ZZX & a, ll weight, ll count);
  ZZX emptyColumn(); // TODO: duplicite with voting_game.h
  ZZ countSwingsColumn(const ZZX & a, const ZZX & b, ll weight);
  void addToColumnInplace(ZZX & a, ll weight, ll count);
  // recover a in a*b = c
  void removeFromColumn(ZZX &a, ll weight, ll count);

  // Time: O(n*q), Space: O(unique_weights * q)
  vector<double> banzhaf() override;
  void banzhafRec(int first, int last, ZZX pf);
  vector<double> banzhafSlow();
  vector<double> banzhafSlowUnmerge(); // TOOD: would be the best if I could find fast polynomial division

  ZZX mergeRec(int st, int en);

  vector<ll> origWeights;
  vector<ll> w;
  vector<ll> cnt;
  ll quota;
private:
  ZZX rolling;
  vector<ZZ> sums;
  ZZ sum;
  map<ll,ZZ> weightToRes;
};


#endif //COOPGAME_VOTING_NONUNIQUE_H
