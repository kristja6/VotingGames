#ifndef __COAL_GAME_H__
#define __COAL_GAME_H__
#include <vector>
#include <string>
#include <functional>
#include <set>
#include "math.h"
#define BANZHAF_DENOM_SUBSETS 0
#define BANZHAF_DENOM_WINNING 1 // for normalized banzhaf index
#define BANZHAF_DENOM_NONE 2

#define dbg cerr

using namespace std;
using namespace NTL;

// type of weights
class CoalitionalGame {
public:
  CoalitionalGame() = default;
  CoalitionalGame(int players): players(players) {};
  CoalitionalGame(int players, const function<double(const vector<int> & players)> & fn):
    players(players), vFunc(fn) { };

  virtual double v(const vector<int> & coalition);

  /* Default exact methods, to be overwritten by more efficient methods
     in child classes */

  virtual vector<double> shapley();
  virtual double shapley(int player);
  virtual vector<double> banzhaf();
  virtual double banzhaf(int player);

  // ---------- Monte Carlo methods -----------
  vector<double> shapleyMonteCarlo(int iters);
  vector<double> banzhafMonteCarlo(int iters);

  // ------------ Enumeration ------------------
  vector<double> banzhafEnum();
  void banzhafEnumRec(int player, vector<int> & coal);
  vector<double> shapleyEnum();
  void shapleyEnumRec(int player, vector<int> & coal);

  virtual void setBanzhafDenominator(int denom);

  int players;

protected:
  int banzhafDenominator = BANZHAF_DENOM_WINNING;

  vector<double> normalizeRawBanzhaf(const vector<ZZ> &sums);
  vector<double> normalizeRawShapley(const vector<ZZ> &sums);
  vector<double> normalizeRawShapley(const vector<ZZ> &sums, const ZZ & normalization);

  vector<int> bestCoal;
  double bestValue;
  bool outputLog = 0;

private:
  function<double(const vector<int> & players)> vFunc;
  vector<double> sums;

};


#endif
