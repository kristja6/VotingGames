#ifndef __COAL_GAME_H__
#define __COAL_GAME_H__

#include <bits/stdc++.h>
#include "random.h"
#include "math.h"
//#define ll long long
#define BANZHAF_DENOM_SUBSETS 0
#define BANZHAF_DENOM_WINNING 1
#define BANZHAF_DENOM_NONE 2

using namespace std;
using ll = long long;

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

  // ---------- Monte Carlo methods -----------
  vector<double> shapleyMonteCarlo(ll iters) {
    vector<double> sh(players);
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
    return sh;
  }

  // TODO: make sure this is correct
  vector<double> banzhafMonteCarlo(ll iters) {
    vector<double> bz(players);
    vector<int> appears(players);
    double swingVotes = 0;

    for (ll it = 0; it < iters; ++ it) {
      vector<int> s = random_subset(players);
      vector<int> cur = s;
      for (int i = s.size() - 1; i >= 0; -- i) {
        appears[s[i]] ++;
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

    normalizeBanzhafSums(bz, iters, swingVotes);
    return bz;
  }

  void setBanzhafDenominator(int denom) {
    banzhafDenominator = denom;
  }

protected:
  int players;
  int banzhafDenominator = BANZHAF_DENOM_WINNING;

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

  // all are log values
  void normalizeBanzhafLogSums(vector<double> & sum, double subsets, double swingVotes) {
    if (banzhafDenominator == BANZHAF_DENOM_SUBSETS) for (auto & i: sum) i -= subsets;
    else if (banzhafDenominator == BANZHAF_DENOM_WINNING) for (auto & i: sum) i -= swingVotes;
    if (!outputLog) logToNorm(sum);
  }

  void normalizeBanzhafLogSums(vector<double> & sums){
    double subsets = (players - 1) / log(2);
    double swingVotes = -INF;
    if (banzhafDenominator == BANZHAF_DENOM_WINNING) {
      for (auto & i: sums) swingVotes = logAdd(swingVotes, i);
    }
    normalizeBanzhafLogSums(sums, subsets, swingVotes);
  }

  void normalizeShapleyLogSums(vector<double> & sums) {
    for (auto & i: sums) i -= logFact(players);
    if (!outputLog) logToNorm(sums);
  }

  bool outputLog = 0;

private:
  function<CoalValue(const vector<int> & players)> vFunc;

};


#endif
