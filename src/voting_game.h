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

  vector<int> getTopPlayers(int topN); // get the strongest players
private:
  vector<double> shapleyNewDp(); // the new algorithm
  vector<double> shapleyNewDp(const vector<int> &); // the new algorithm with just a subset of players

  vector<double> banzhafNewDp(); // the new algorithm
  vector<double> banzhafNewDp(const vector<int> &); // the new algorithm with just a subset of players


  // For Banzhaf index
  ZZX emptyColumn();
  ZZX columnWithOne(int weight, int count);
  ZZX mergeRecBanzhaf(int st, int en, int);

  // For Shapley value
  Polynomial2D emptyTable();
  Polynomial2D tableWithOne(int weight, int count);
  Polynomial2D mergeRecShapley(int st, int en, int keepForPlayers, int keepForQuota);

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
};


#endif //COOPGAME_VOTING_NONUNIQUE_H
