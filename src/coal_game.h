#ifndef __COAL_GAME_H__
#define __COAL_GAME_H__

#include <bits/stdc++.h>
#include "math.h"
#include "types.h"
//#define ll long long
#define BANZHAF_DENOM_SUBSETS 0
#define BANZHAF_DENOM_WINNING 1
#define BANZHAF_DENOM_NONE 2

using namespace std;
using ll = long long;
using namespace NTL;

// type of weights
template<class CoalValue>
class CoalGame {
public:
  CoalGame() = default;
  CoalGame(int players): players(players) {};
  CoalGame(int players, const function<CoalValue(const vector<int> & players)> & fn):
    players(players), vFunc(fn) { };

  virtual CoalValue v(const vector<int> & coalition) {
    return vFunc(coalition);
  }

  /* Default exact methods, to be overwritten by more efficient methods
     in child classes */

  virtual vector<double> shapley() {
    return shapleyEnum();
  }
  virtual double shapley(int player) {
    return shapley()[player];
  }
  virtual vector<double> banzhaf() {
    return banzhafEnum();
  }
  virtual double banzhaf(int player) {
    return banzhaf()[player];
  }

  // ---------- Monte Carlo methods -----------
  vector<double> shapleyMonteCarlo(ll iters) {
    vector<LogNum> sh(players);
    for (ll it = 0; it < iters; ++ it) {
      vector<int> pi = random_perm(players);
      vector<int> cur;
      for (int i = 0; i < players; ++ i) {
        sh[pi[i]] -= v(cur);
        cur.push_back(pi[i]);
        sh[pi[i]] += v(cur);
      }
    }
    for (int i = 0; i < players; ++ i) sh[i] /= iters;
    return toNormal(sh);
  }

  vector<double> banzhafMonteCarlo(ll iters) {
    vector<LogNum> bz(players);
    vector<int> appears(players);
    double swingVotes = 0;

    for (ll it = 0; it < iters; ++it) {
      vector<int> s = random_subset(players);
      vector<int> cur = s;
      for (int i = s.size() - 1; i >= 0; --i) {
        appears[s[i]]++;
        bz[s[i]] += v(cur);
        swingVotes += v(cur);
        swap(cur[i], cur.back());
        int deleted = cur.back();
        cur.pop_back();
        bz[s[i]] -= v(cur);
        swingVotes -= v(cur);
        cur.push_back(deleted);
      }
    }
    normalizeBanzhafLogSums(bz, iters, swingVotes);
    return toNormal(bz);
  }

  // ------------ Enumeration ------------------
  vector<double> banzhafEnum() {
    logSums = vector<LogNum>(players);
    vector<int> coal;
    banzhafEnumRec(0, coal);
    normalizeBanzhafLogSums(logSums);
    return toNormal(logSums);
  }

  void banzhafEnumRec(int player, vector<int> & coal) {
    if (player == players) {
      for (int i = (int)coal.size() - 1; i >= 0; --i) {
        const int pl = coal[i];
        logSums[pl] += v(coal);
        swap(coal[i], coal.back());
        coal.pop_back();
        logSums[pl] += v(coal);
        coal.push_back(pl);
      }
      sort(coal.begin(), coal.end());
      return;
    }
    banzhafEnumRec(player + 1, coal);
    coal.push_back(player);
    banzhafEnumRec(player + 1, coal);
    coal.pop_back();
  }

  vector<double> shapleyEnum() {
    logSums = vector<LogNum>(players);
    vector<int> coal;
    shapleyEnumRec(0, coal);
    normalizeShapleyLogSums(logSums);
    return toNormal(logSums);
  }

  void shapleyEnumRec(int player, vector<int> & coal) {
    if (player == players) {
      const double withAll = log(v(coal));
      for (int i = (int)coal.size() - 1; i >= 0; --i) {
        const int pl = coal[i];
        double incr = withAll;
        swap(coal[i], coal.back());
        coal.pop_back();
        logDec(incr, log(v(coal)));
        coal.push_back(pl);
        logSums[pl] += LogNum(incr + logFact(coal.size() - 1) + logFact(players - coal.size()), true);
      }
      sort(coal.begin(), coal.end());
      return;
    }
    shapleyEnumRec(player + 1, coal);
    coal.push_back(player);
    shapleyEnumRec(player + 1, coal);
    coal.pop_back();
  }

  virtual double banzhafInteraction(vector<int> inputSubset) {
    return banzhafInteraction(inputSubset);
  }

  // NOTE: currently normalized by the number of possible coalitions without inputSubset
  double banzhafInteractionEnum(vector<int> inputSubset) {
    std::set<int> forbidden(inputSubset.begin(), inputSubset.end());
    vector<int> empty;
    ExtLogNum res = banzhafInteractionEnumRec1(0, forbidden, empty, inputSubset);
    res /= LogNum((players - inputSubset.size()) / log(2), true);
    return res.norm();
  }

  // interates over subsets of 2^players - input_subset
  ExtLogNum banzhafInteractionEnumRec1(int player, const std::set<int> & forbidden, vector<int> & curSubset, const vector<int> & inputSubset) {
    while (forbidden.count(player)) player ++;
    if (player == players) {
      return banzhafInteractionEnumRec2(0, curSubset, inputSubset);
    }
    ExtLogNum res = banzhafInteractionEnumRec1(player + 1, forbidden, curSubset, inputSubset);
    curSubset.push_back(player);
    res += banzhafInteractionEnumRec1(player + 1, forbidden, curSubset, inputSubset);
    curSubset.pop_back();
    return res;
  }

  // iterates over subsets of the input subset
  ExtLogNum banzhafInteractionEnumRec2(int player, vector<int> &curSubset, const vector<int> &subset, int added = 0) {
    if (player == (int)subset.size()) {
      ExtLogNum res(v(curSubset));
      if ((subset.size() - added) % 2 == 1) res *= -1;
      return res;
    }

    ExtLogNum res;
    res += banzhafInteractionEnumRec2(player + 1, curSubset, subset, added);
    curSubset.push_back(subset[player]);
    res += banzhafInteractionEnumRec2(player + 1, curSubset, subset, added + 1);
    curSubset.pop_back();
    return res;
  }

  void setBanzhafDenominator(int denom) {
    banzhafDenominator = denom;
  }

  virtual vector<int> optimalCoalitionOfFixedSize(int size) {
    vector<int> coal;
    bestValue = numeric_limits<CoalValue>::min();
    optimalCoalitionOfFixedSizeRec(players, 0, size, coal);
    return bestCoal;
  }

  int players;

protected:
  int banzhafDenominator = BANZHAF_DENOM_WINNING; // TODO: use winning by defualt
  //int banzhafDenominator = BANZHAF_DENOM_SUBSETS;

  void normalizeBanzhafSums(vector<double> & sums, double subsets, double swingVotes) {
    switch (banzhafDenominator) {
      case BANZHAF_DENOM_SUBSETS:
        for (int i = 0; i < players; ++ i) sums[i] /= subsets;
        break;
      case BANZHAF_DENOM_WINNING:
        for (int i = 0; i < players; ++ i) sums[i] /= swingVotes;
        break;
    }
  }

  void normalizeBanzhafSums(vector<double> & sums) {
    normalizeBanzhafSums(sums, pow(2, players - 1), accumulate(sums.begin(), sums.end(), 0.0));
  }

  // For LogNum
  void normalizeBanzhafLogSums(vector<LogNum> & sum, LogNum subsets, LogNum swingVotes) {
    if (banzhafDenominator == BANZHAF_DENOM_SUBSETS) for (auto & i: sum) i -= subsets;
    else if (banzhafDenominator == BANZHAF_DENOM_WINNING) for (auto & i: sum) i -= swingVotes;
  }

  void normalizeBanzhafLogSums(vector<LogNum> & sums){
    double subsets = (players - 1) / log(2);
    LogNum swingVotes;
    if (banzhafDenominator == BANZHAF_DENOM_WINNING) {
      //for (auto & i: sums) swingVotes = logAdd(swingVotes, i);
      for (auto & i: sums) swingVotes += i;
    }
    normalizeBanzhafLogSums(sums, subsets, swingVotes);
  }

  // For BigNum
  void normalizeBanzhafLogSums(vector<BigNum> & sum, const BigNum & subsets, const BigNum & swingVotes) {
    if (banzhafDenominator == BANZHAF_DENOM_SUBSETS) for (auto & i: sum) i -= subsets;
    else if (banzhafDenominator == BANZHAF_DENOM_WINNING) for (auto & i: sum) i -= swingVotes;
  }

  void normalizeBanzhafLogSums(vector<BigNum> & sums){
    BigNum subsets = power(ZZ(2), players - 1);
    BigNum swingVotes(0);
    if (banzhafDenominator == BANZHAF_DENOM_WINNING) {
      for (auto & i: sums) swingVotes += i;
    }
    normalizeBanzhafLogSums(sums, subsets, swingVotes);
  }

  void normalizeShapleyLogSums(vector<double> & sums) {
    for (auto & i: sums) i -= logFact(players);
    if (!outputLog) logToNorm(sums);
  }

  void normalizeShapleyLogSums(vector<LogNum> & sums) {
    for (auto & i: sums) i /= LogNum(logFact(players), true);
  }

  vector<int> bestCoal;
  CoalValue bestValue;

  void optimalCoalitionOfFixedSizeRec(int items, int start_item, int len, vector<int> & coal) {
    if (!len) {
      CoalValue curVal = v(coal);
      if (curVal > bestValue) {
        bestCoal = coal;
        bestValue = curVal;
      }
      return;
    }

    for (int i = start_item; i < items; ++i) {
      coal.push_back(i);
      optimalCoalitionOfFixedSizeRec(items, i + 1, len - 1, coal);
      coal.pop_back();
    }
  }

  bool outputLog = 0;

private:
  function<CoalValue(const vector<int> & players)> vFunc;
  vector<LogNum> logSums;

};


#endif
