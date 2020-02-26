#ifndef __ZERO_ONE_GAME__
#define __ZERO_ONE_GAME__

#include "coal_game.h"
#include "math.h"

class VotingGame : public CoalGame {
  public:

  using ll = long long;
  VotingGame(const vector<ll> &weights, long long int quota);
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
  virtual vector<double> shapleyNew();
  virtual vector<double> shapleyNewForEach();
  virtual vector<double> shapleyNewDp();

  const vector<ll> & getWeights() const;
  ll getQuota() const;

protected:
  // Banzhaf methods
  ZZX emptyColumn();
  void addToColumnInplace(ZZX &a, ll weight);
  ZZ countSwingsColumn(const ZZX & a, const ZZX & b, ll weight);
  void removeFromColumnInplace(ZZX &a, ll weight);
  ZZX columnWithOne(int weight);
  ZZ countSwingsTable(const Polynomial2D & a, int weight);
  ZZ countSwingsTable(const SparsePolynomial2D & a, int weight);

  virtual ZZX mergeRecBanzhaf(int st, int en);

  // Shapley methods
  Polynomial2D emptyTable();
  Polynomial2D tableWithOne(int weight);
  vector<double> shapleyUnoDpHelper();

  Polynomial2D mergeRecShapleyDense(int st, int en, int depth = 1);
  virtual SparsePolynomial2D mergeRecShapleySparse(int st, int en);

  // Common methods
  void precompMaxPlayers();
  ll reduceDummyPlayers();

  int maxPlayers;
  int nonzeroPlayers;
  int maxWeight;

  // Defines the game
  vector<ll> weights;
  ll quota;


private:

  vector<double> bbSums;
  int cutoffDepth;
  int getCutoffDepth();
};

#endif
