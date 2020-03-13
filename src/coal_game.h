#ifndef __COAL_GAME_H__
#define __COAL_GAME_H__
#include <vector>
#include <string>
#include <functional>
#include <set>
#include "math.h"
#define BANZHAF_DENOM_SUBSETS 0
#define BANZHAF_DENOM_WINNING 1
#define BANZHAF_DENOM_NONE 2

#define dbg cerr

using namespace std;
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
  vector<double> shapleyMonteCarlo(int iters);
  vector<double> banzhafMonteCarlo(int iters);

  // ------------ Enumeration ------------------
  vector<double> banzhafEnum();
  void banzhafEnumRec(int player, vector<int> & coal);
  vector<double> shapleyEnum();
  void shapleyEnumRec(int player, vector<int> & coal);
  virtual double banzhafInteraction(vector<int> inputSubset);

  // NOTE: currently normalized by the number of possible coalitions without inputSubset
  double banzhafInteractionEnum(vector<int> inputSubset);
  // interates over subsets of 2^players - input_subset
  ZZ banzhafInteractionEnumRec1(int player, const std::set<int> & forbidden, vector<int> & curSubset, const vector<int> & inputSubset);
  // iterates over subsets of the input subset
  ZZ banzhafInteractionEnumRec2(int player, vector<int> &curSubset, const vector<int> &subset, int added = 0);

  virtual void setBanzhafDenominator(int denom);
  virtual vector<int> optimalCoalitionOfFixedSize(int size);

  int players;
  int getPlayers() const;

protected:
  int banzhafDenominator = BANZHAF_DENOM_WINNING; // TODO: use winning by defualt
  //int banzhafDenominator = BANZHAF_DENOM_SUBSETS;

  vector<double> normalizeRawBanzhaf(const vector<ZZ> &sums);
  vector<double> normalizeRawShapley(const vector<ZZ> &sums);
  vector<double> normalizeRawShapley(const vector<ZZ> &sums, const ZZ & normalization);

  vector<int> bestCoal;
  double bestValue;

  void optimalCoalitionOfFixedSizeRec(int items, int start_item, int len, vector<int> & coal);

  bool outputLog = 0;

private:
  function<double(const vector<int> & players)> vFunc;
  vector<ZZ> sums;

};


#endif
