#ifndef __ZERO_ONE_GAME__
#define __ZERO_ONE_GAME__

#include "coal_game.h"
#include "math.h"

class VotingGame : public CoalGame {
  public:

  VotingGame(const vector<int> &weights, int quota);
  double v(const vector<int> & coalition) override;

  virtual vector<double> banzhaf() override;
  virtual double banzhaf(int player) override;
  virtual vector<double> shapley() override;
  virtual double shapley(int player);

  // TIME: O(n^3 * q), SPACE: O(n^2 + q)
  vector<double> banzhafNaiveDp();
  // TODO: TIME
  vector<double> banzhafUnoDp();
  // TIME: O(n^2*q), SPACE: O(nq)
  vector<double> shapleyUnoDp();
  // TODO: TIME

  const vector<int> & getWeights() const;
  int getQuota() const;

protected:
  // Banzhaf methods
  ZZX emptyColumn();
  void addToColumnInplace(ZZX &a, int weight);
  ZZ countSwingsColumn(const ZZX & a, const ZZX & b, int weight);
  void removeFromColumnInplace(ZZX &a, int weight);
  ZZX columnWithOne(int weight);
  ZZ countSwingsTable(const Polynomial2D & a, int weight);
  vector<int> getTopPlayers(int topN);

  virtual ZZX mergeRecBanzhaf(int st, int en);

  // Shapley methods
  Polynomial2D emptyTable();
  Polynomial2D tableWithOne(int weight);
  vector<double> shapleyUnoDpHelper();

  Polynomial2D mergeRecShapleyDense(int st, int en, int depth = 1);

  // Common methods
  void precompMaxPlayers();
  int reduceDummyPlayers();

  int maxPlayers;
  int maxPlayersAll;
  int nonzeroPlayers;
  int maxWeight;

  // Defines the game
  vector<int> weights;
  int quota;


private:
};

#endif
