#ifndef __ZERO_ONE_GAME__
#define __ZERO_ONE_GAME__

#include "coal_game.h"
#include "math.h"

class VotingGame : public CoalGame<ll> {
  public:

  using ll = long long;
  VotingGame(const vector<ll> &weights, long long int quota):
    CoalGame(weights.size()), weights(weights), quota(quota) {
    cutoffDepth = getCutoffDepth();
    nonzeroPlayers = players;
    for (int i = 0; i < players; ++ i) if (!weights[i]) nonzeroPlayers --;
    precompMaxPlayers();
  }

  // read from stdin
  VotingGame(istream & in);
  virtual ~VotingGame() {
  }

  // TIME: O(n^2 * q), SPACE: O(n + q)
  vector<double> banzhafDpSlow();

  // TIME: O(n^2*q) SPACE: O(n + q)
  virtual vector<double> banzhaf() override;
  virtual double banzhaf(int player) override;
  //vector<double> banzhafFast();
  vector<double> banzhafBranchAndBound();

  // TIME: O(n^2*q), SPACE: O(nq)
  vector<double> shapleyLogNumHelp();
  vector<double> shapley() override;
  double shapley(int player);
  vector<double> shapley(bool useLog);
  vector<double> shapleyHelp();
  ll v(const vector<int> & coalition) override;
  vector<double> shapleyNew();

  // todo: make protected
  vector<ll> weights;
  ll quota;

protected:
  friend class VotingNonunique;

  static ZZX emptyColumn();
  static Polynomial2D emptyTable();
  static vector<LogNum> emptyColumnLogNum(int quota);
  static vector<LogNum> addToColumn(vector<LogNum> a, ll weight, int quota);
  static void addToColumnInplace(vector<LogNum> & a, ll weight);
  static void addToColumnInplace(ZZX &a, ll weight, int quota);
  static void removeFromColumnInplace(vector<LogNum> & a, ll weight);
  static LogNum countSwingsColumn(const vector<LogNum> &a, ll weight, int quota);
  static LogNum countSwingsColumn(const vector<LogNum> &a, const vector<LogNum> &b, ll weight, int quota);
  static ZZ countSwingsColumn(const ZZX & a, const ZZX & b, ll weight, int quota);
  static bool hasAnySwings(const vector<LogNum> &a, const vector<LogNum> &b, ll weight, int quota);
  static void removeFromColumnInplace(ZZX &a, ll weight);

  void bbRec(ll sum, int idx, vector<bool> has, bool checkSum);
  ll reduceDummyPlayers();

  virtual ZZX mergeRecBanzhaf(int st, int en);
  virtual Polynomial2D mergeRecShapleyDense(int st, int en, int maxPlayers, int depth = 1);
  unordered_map<pair<int,int>, ZZ, IntPairHash> mergeRecShapleySparse(int st, int en, int maxPlayers);

  ZZX columnWithOne(int weight);
  Polynomial2D tableWithOne(int weight);
  static ZZ countSwingsTable(const Polynomial2D & a, int weight, int quota, int players);
  static ZZ countSwingsTable(const SparsePolynomial2D & a, int weight, int quota, int players);

private:
  vector<double> bbSums;
  vector<double> logSumsRec;
  map<pair<int,int>,ZZX> cache;
  int cutoffDepth;
  int getCutoffDepth();
  int nonzeroPlayers;
  int maxPlayers;
  void precompMaxPlayers();
};

#endif
