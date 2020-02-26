//
// Created by maty on 07/02/20.
//

#ifndef COOPGAME_VOTING_NONUNIQUE_H
#define COOPGAME_VOTING_NONUNIQUE_H


#include "coal_game.h"
#include "voting_game.h"
#include <NTL/ZZX.h>
using namespace NTL;

class VotingNonunique : public VotingGame {
public:
  VotingNonunique(const vector<ll> &weights, ll quota);

  double v(const vector<int> &coalition) override;

  double shapley(int player) override;
  vector<double> shapley() override;
  vector<double> banzhaf() override;

  virtual vector<double> shapleyNew() override;
  vector<double> shapleyNewForEachPlayer();
  vector<double> shapleyNewDp() override;

  vector<double> banzhafNewOnlyConvolution();
  vector<double> banzhafNewWithDeconvolution(); // TODO: would be the best if I could find fast polynomial division
  vector<double> banzhafNewDp();

protected:
  // Banzhaf methods
  ZZX columnWithOne(ll weight, ll count);
  ZZX addToColumn(const ZZX & a, ll weight, ll count);
  void addToColumnInplace(ZZX & a, ll weight, ll count);

  void banzhafRec(int first, int last, ZZX pf);
  ZZX mergeRecBanzhaf(int st, int en) override;

  // Shapley methods
  Polynomial2D tableWithOne(ll weight, ll count);
  SparsePolynomial2D tableWithOneSparse(ll weight, ll count);
  void addToTableInplace(Polynomial2D & a, ll weight, ll count);
  void addToTableInplace(SparsePolynomial2D & a, ll weight, ll count);
  void removeFromColumn(ZZX &a, ll weight, ll count);

  Polynomial2D mergeRecShapley(int st, int en);
  SparsePolynomial2D mergeRecShapleySparse(int st, int en) override;
  SparsePolynomial2D sparseWithOne(int weight, int count);
  void shapleyMergeRec(int first, int last, const Polynomial2D &pf);


  // information about the game
  vector<ll> uniqueWeights;
  vector<ll> weightCount;

private:

  ZZX rollingBanzhaf;
  Polynomial2D rollingShapley;
  vector<ZZ> sums;
  ZZ sum;
  unordered_map<ll,ZZ> weightToRes;
  unordered_map<ll,double> shapleyCache;
};


#endif //COOPGAME_VOTING_NONUNIQUE_H
