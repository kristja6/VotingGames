#ifndef __ZERO_ONE_GAME__
#define __ZERO_ONE_GAME__

#include "coal_game.h"

class VotingGame : public CoalGame<ll> {
  public:
  using ll = long long;
  VotingGame(const vector<ll> &weights, long long int quota):
    CoalGame(weights.size()), weights(weights), quota(quota) {
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
  vector<double> shapleyLogNum();
  vector<double> shapleyLogNumHelp();
  ll v(const vector<int> & coalition) override;
  vector<double> shapleyNew();

  // todo: make protected
  vector<ll> weights;
  ll quota;

protected:

  // Banzhaf
  ZZX emptyColumn();
  Polynomial2D emptyTable();
  vector<LogNum> emptyColumnLogNum();
  vector<LogNum> addToColumn(vector<LogNum> a, ll weight); // TODO: make in-place
  void addToColumnInplace(vector<LogNum> & a, ll weight);
  void addToColumnInplace(ZZX &a, ll weight);
  void removeFromColumnInplace(vector<LogNum> & a, ll weight);
  LogNum countSwingsColumn(const vector<LogNum> &a, ll weight);
  LogNum countSwingsColumn(const vector<LogNum> &a, const vector<LogNum> &b, ll weight);
  ZZ countSwingsColumn(const ZZX & a, const ZZX & b, ll weight);
  bool hasAnySwings(const vector<LogNum> &a, const vector<LogNum> &b, ll weight);
  void removeFromColumnInplace(ZZX &a, ll weight);

  void bbRec(ll sum, int idx, vector<bool> has, bool checkSum);
  ll reduceDummyPlayers();

  virtual ZZX mergeRecBanzhaf(int st, int en);
  virtual Polynomial2D mergeRecShapley(int st, int en);

  ZZX columnWithOne(int weight);
  Polynomial2D tableWithOne(int weight);
  ZZ countSwingsTable(const Polynomial2D & a, int quota);

private:
  vector<double> bbSums;
  vector<double> logSumsRec;
  map<pair<int,int>,ZZX> cache;
};

#endif
