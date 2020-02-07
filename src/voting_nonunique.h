//
// Created by maty on 07/02/20.
//

#ifndef COOPGAME_VOTING_NONUNIQUE_H
#define COOPGAME_VOTING_NONUNIQUE_H


#include "coal_game.h"

class VotingNonunique : public CoalGame<ll> {
public:
  VotingNonunique(vector<ll> weights, ll quota);

  ll v(const vector<int> &coalition) override;

  vector<double> emptyColumn();

  vector<double> columnWithOne(ll weight, ll count);
  vector<double> mergeColumns(vector<double> a, vector<double> b);
  vector<double> addToColumn(const vector<double> & a, double weight, ll count);
  // recover a in a*b = c
  vector<double> unmergeColumns(vector<double> c, vector<double> b);

  double countSwingsColumn(vector<double> a, vector<double> b, ll weight);

  // Time: O(n*q), Space: O(unique_weights * q)
  vector<double> banzhaf() override;

  vector<ll> origWeights;
  vector<ll> w;
  vector<ll> cnt;
  ll quota;
private:
  vector<double> fillLeft(int last);
};


#endif //COOPGAME_VOTING_NONUNIQUE_H
