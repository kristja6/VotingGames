#ifndef __ZERO_ONE_GAME__
#define __ZERO_ONE_GAME__

#include "coal_game.h"

class VotingGame : public CoalGame<ll> {
  using ll = long long;
  public:
  VotingGame(int players, const vector<ll> &weights, long long int quota) :
      CoalGame(players), weights(weights), quota(quota) {
  }
  VotingGame(const vector<ll> &weights, long long int quota):
    VotingGame(weights.size(), weights, quota) {
  }

  // read from stdin
  VotingGame(istream & in);
  virtual ~VotingGame() {
  }

  // TIME: O(n^2 * q), SPACE: O(n + q)
  vector<double> banzhafDpSlow();

  // TIME: O(n*q) SPACE: O(n + q)
  vector<double> banzhaf() override;
  vector<double> banzhafBranchAndBound();

  // TIME: O(n^2*q), SPACE: O(nq)
  vector<double> shapley() override;
  vector<double> shapleyHelp();
  ll v(const vector<int> & coalition) override;

  // todo: make protected
  vector<ll> weights;
  ll quota;

protected:

  // Banzhaf
  ZZX emptyColumn();
  vector<double> emptyColumnDub();
  vector<double> addToColumn(vector<double> a, ll weight); // TODO: make in-place
  void addToColumnInplace(vector<double> & a, ll weight);
  void addToColumnInplace(ZZX &a, ll weight);
  void removeFromColumnInplace(vector<double> & a, ll weight);
  double countSwingsColumn(const vector<double> &a, ll weight);
  double countSwingsColumn(const vector<double> &a, const vector<double> &b, ll weight);
  ZZ countSwingsColumn(const ZZX & a, const ZZX & b, ll weight);
  bool hasAnySwings(const vector<double> &a, const vector<double> &b, ll weight);
  void removeFromColumnInplace(ZZX &a, ll weight);

  void bbRec(ll sum, int idx, vector<bool> has, bool checkSum);
  ll reduceDummyPlayers();

private:
  vector<double> bbSums;
  vector<double> logSumsRec;
};

#endif
