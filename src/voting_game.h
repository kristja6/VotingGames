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

  vector<double> shapley() override; // Computes the Shapley value of all players
  vector<double> banzhaf() override; // Computes the Banzhaf index of all players
  vector<double> shapley(const vector<int> &); // Computes the Shapley value only for the players in the input
  vector<double> banzhaf(const vector<int> &); // Computes the Banzhaf index only for the players in the input

  virtual vector<double> banzhafTop(int); // Computer the Shapley value only for the topN strongest players
  virtual vector<double> shapleyTop(int); // Computer the Banzhaf index only for the topN strongest players

  const vector<int> & getWeights() const { return weights; }

private:
  vector<double> shapleyNewDp();
  vector<double> shapleyNewDp(const vector<int> &);

  vector<double> banzhafNewDp();
  vector<double> banzhafNewDp(const vector<int> &);


  // For Banzhaf index
  ZZX emptyColumn();
  ZZX columnWithOne(int weight, int count);
  ZZX mergeRecBanzhaf(int st, int en, int);

  // For Shapley value
  Polynomial2D emptyTable();
  Polynomial2D tableWithOne(int weight, int count);
  Polynomial2D mergeRecShapley(int st, int en, int keepForPlayers, int keepForQuota);

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
