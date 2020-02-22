#ifndef __COAL_GAME_H__
#define __COAL_GAME_H__

#include <bits/stdc++.h>
#include "math.h"
#include "types.h"
//#define ll long long
#define BANZHAF_DENOM_SUBSETS 0
#define BANZHAF_DENOM_WINNING 1
#define BANZHAF_DENOM_NONE 2

#define dbg cerr

using namespace std;
using ll = long long;
using namespace NTL;

// type of weights
class CoalGame {
public:
  CoalGame() = default;
  CoalGame(int players): players(players) {};
  CoalGame(int players, const function<double(const vector<int> & players)> & fn):
    players(players), vFunc(fn) { };

  virtual double v(const vector<int> & coalition);

  /* Default exact methods, to be overwritten by more efficient methods
     in child classes */

  virtual vector<double> shapley();
  virtual double shapley(int player);
  virtual vector<double> banzhaf();
  virtual double banzhaf(int player);

  // ---------- Monte Carlo methods -----------
  vector<double> shapleyMonteCarlo(ll iters);
  vector<double> banzhafMonteCarlo(ll iters);

  // ------------ Enumeration ------------------
  vector<double> banzhafEnum();
  void banzhafEnumRec(int player, vector<int> & coal);
  vector<double> shapleyEnum();
  void shapleyEnumRec(int player, vector<int> & coal);
  virtual double banzhafInteraction(vector<int> inputSubset);

  // NOTE: currently normalized by the number of possible coalitions without inputSubset
  double banzhafInteractionEnum(vector<int> inputSubset);
  // interates over subsets of 2^players - input_subset
  ExtLogNum banzhafInteractionEnumRec1(int player, const std::set<int> & forbidden, vector<int> & curSubset, const vector<int> & inputSubset);
  // iterates over subsets of the input subset
  ExtLogNum banzhafInteractionEnumRec2(int player, vector<int> &curSubset, const vector<int> &subset, int added = 0);

  void setBanzhafDenominator(int denom);
  virtual vector<int> optimalCoalitionOfFixedSize(int size);

  int players;
  int getPlayers() const;

protected:
  int banzhafDenominator = BANZHAF_DENOM_WINNING; // TODO: use winning by defualt
  //int banzhafDenominator = BANZHAF_DENOM_SUBSETS;

  vector<double> normalizeRawBanzhaf(const vector<BigNum> &sums);
  vector<double> normalizeRawShapley(const vector<BigNum> &sums);
  vector<double> normalizeShapleyCounts(const vector<BigNum> &sums);

  vector<int> bestCoal;
  double bestValue;

  void optimalCoalitionOfFixedSizeRec(int items, int start_item, int len, vector<int> & coal);

  bool outputLog = 0;

private:
  function<double(const vector<int> & players)> vFunc;
  vector<ZZ> sums;

};


#endif
