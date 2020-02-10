//
// Created by maty on 07/02/20.
//

#ifndef COOPGAME_VOTING_NONUNIQUE_H
#define COOPGAME_VOTING_NONUNIQUE_H


#include "coal_game.h"
#include <NTL/ZZX.h>
using namespace NTL;

class VotingNonunique : public CoalGame<ll> {
public:
  VotingNonunique(vector<ll> weights, ll quota);

  ll v(const vector<int> &coalition) override;

  ZZX emptyColumn();

  ZZX columnWithOne(ll weight, ll count);
  ZZ countSwingsColumn(const ZZX & a, const ZZX & b, ll weight);
  ZZX addToColumn(const ZZX & a, ll weight, ll count);
  void addToColumnInplace(ZZX & a, ll weight, ll count);
  // recover a in a*b = c
  //ZZX unmergeColumns(vector<double> c, vector<double> b);
  void removeFromColumn(ZZX &a, ll weight, ll count);

  // Time: O(n*q), Space: O(unique_weights * q)
  vector<double> banzhaf() override;

  vector<ll> origWeights;
  vector<ll> w;
  vector<ll> cnt;
  ll quota;
private:
};


#endif //COOPGAME_VOTING_NONUNIQUE_H
