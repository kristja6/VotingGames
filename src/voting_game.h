//
// Created by maty on 07/02/20.
//

#ifndef COOPGAME_VOTING_NONUNIQUE_H
#define COOPGAME_VOTING_NONUNIQUE_H


#include "coalitional_game.h"
#include <NTL/ZZX.h>
#include <unordered_map>

using namespace NTL;

class VotingGame : public CoalitionalGame {
public:
  VotingGame(const vector<int> &weights, int quota);

  double v(const vector<int> &coalition) override;

  vector<double> shapley() override;
  vector<double> banzhaf() override;
  vector<double> banzhaf(const vector<int> &);
  vector<double> shapley(const vector<int> &);

  vector<double> shapleyNewDp();
  vector<double> shapleyNewDp(const vector<int> &);

  vector<double> banzhafNewDp();
  vector<double> banzhafNewDp(const vector<int> &);

  virtual vector<double> banzhafTop(int topN);
  virtual vector<double> shapleyTop(int);

  const vector<int> & getWeights() const { return weights; }

private:

  // Banzhaf methods
  ZZX emptyColumn();
  ZZX columnWithOne(int weight, int count);
  ZZX columnWithOne(int weight, int count, int maxPlayers, int quota);
  ZZX addToColumn(const ZZX & a, int weight, int count);
  void addToColumnInplace(ZZX & a, int weight, int count);

  void banzhafRec(int first, int last, ZZX pf);
  ZZX mergeRecBanzhaf(int st, int en);
  ZZX mergeRecBanzhaf(int st, int en, int);

  // Shapley methods
  Polynomial2D emptyTable();
  Polynomial2D tableWithOne(int weight, int count);
  Polynomial2D tableWithOne(int weight, int count, int, int);
  void addToTableInplace(Polynomial2D & a, int weight, int count);
  void removeFromColumn(ZZX &a, int weight, int count);
  ZZ countSwingsTable(const Polynomial2D & a, int weight);

  Polynomial2D mergeRecShapley(int st, int en);
  Polynomial2D mergeRecShapley(int st, int en, int maxPlayers, int quota);
  void shapleyMergeRec(int first, int last, const Polynomial2D &pf);

  vector<int> getTopPlayers(int topN);
  void precompMaxPlayers();

  // information about the game
  vector<int> uniqueWeights;
  vector<int> weightCount;

  int maxPlayers;
  int maxPlayersAll;
  int nonzeroPlayers;

  int quota;
  vector<int> weights;
  int maxWeight;

  vector<ZZ> sums;
  ZZ sum;
  unordered_map<int,ZZ> weightToRes;
  unordered_map<int,double> shapleyCache;
};


#endif //COOPGAME_VOTING_NONUNIQUE_H
