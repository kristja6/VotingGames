#include "coalitional_game.h"
#include <NTL/RR.h>
#include <algorithm>
#include <set>


double CoalitionalGame::v(const vector<int> &coalition) {
  return vFunc(coalition);
}

vector<double> CoalitionalGame::shapley() {
  return shapleyEnum();
}

double CoalitionalGame::shapley(int player) {
  return shapley()[player];
}

vector<double> CoalitionalGame::banzhaf() {
  return banzhafEnum();
}

double CoalitionalGame::banzhaf(int player) {
  return banzhaf()[player];
}

vector<double> CoalitionalGame::shapleyMonteCarlo(int iters) {
  vector<RR> sh(players);
  for (int it = 0; it < iters; ++ it) {
    vector<int> pi = random_perm(players);
    vector<int> cur;
    for (int i = 0; i < players; ++ i) {
      sh[pi[i]] -= v(cur);
      cur.push_back(pi[i]);
      sh[pi[i]] += v(cur);
    }
  }
  for (int i = 0; i < players; ++ i) sh[i] /= iters;
  vector<double> res(players);
  for (int i = 0; i < players; ++ i) res[i] = conv<double>(sh[i]);
  return res;
}

vector<double> CoalitionalGame::banzhafMonteCarlo(int iters) {
  vector<ZZ> bz(players);
  vector<int> appears(players);
  double swingVotes = 0;

  for (int it = 0; it < iters; ++it) {
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
  return normalizeRawBanzhaf(bz);
}

vector<double> CoalitionalGame::banzhafEnum() {
  sums = vector<double>(players);
  vector<int> coal;
  banzhafEnumRec(0, coal);
  double norm = 0;
  if (banzhafDenominator == BANZHAF_DENOM_SUBSETS) norm = pow(2, players - 1);
  else if (banzhafDenominator == BANZHAF_DENOM_WINNING) {
    for (auto i: sums) norm += i;
  }
  for (auto & i: sums) i /= norm;
  return sums;
}

void CoalitionalGame::banzhafEnumRec(int player, vector<int> &coal) {
  if (player == players) {
    for (int i = (int)coal.size() - 1; i >= 0; --i) {
      const int pl = coal[i];
      sums[pl] += v(coal);
      swap(coal[i], coal.back());
      coal.pop_back();
      sums[pl] -= v(coal);
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

vector<double> CoalitionalGame::shapleyEnum() {
  sums = vector<double>(players);
  vector<int> coal;
  shapleyEnumRec(0, coal);
  for (auto & i: sums) i /= conv<double>(factorialCached(players));
  return sums;
}

void CoalitionalGame::shapleyEnumRec(int player, vector<int> &coal) {
  if (player == players) {
    const double withAll = v(coal);
    for (int i = (int)coal.size() - 1; i >= 0; --i) {
      const int pl = coal[i];
      double incr = withAll;
      swap(coal[i], coal.back());
      coal.pop_back();
      incr -= v(coal);
      coal.push_back(pl);
      sums[pl] += incr * conv<double>(factorialCached(coal.size() - 1) * factorialCached(players - coal.size()));
    }
    sort(coal.begin(), coal.end());
    return;
  }
  shapleyEnumRec(player + 1, coal);
  coal.push_back(player);
  shapleyEnumRec(player + 1, coal);
  coal.pop_back();
}

void CoalitionalGame::setBanzhafDenominator(int denom) {
  banzhafDenominator = denom;
}

vector<double> CoalitionalGame::normalizeRawBanzhaf(const vector<ZZ> &sums) {
  vector<double> res(sums.size());
  if (banzhafDenominator == BANZHAF_DENOM_WINNING) {
    ZZ swingVotes(0);
    vector<ZZ> cpy = sums;
    for (auto & i: sums) swingVotes += i;
    for (size_t i = 0; i < sums.size(); ++ i) {
      res[i] = conv<double>(conv<RR>(sums[i]) / conv<RR>(max(ZZ(1), swingVotes)));
    }
  } else if (banzhafDenominator == BANZHAF_DENOM_SUBSETS) {
    RR subsets = power(RR(2), players);
    for (size_t i = 0; i < sums.size(); ++ i) {
      res[i] = conv<double>(conv<RR>(sums[i]) / subsets);
    }
  }
  return res;
}

vector<double> CoalitionalGame::normalizeRawShapley(const vector<ZZ> &sums, const ZZ & normalization) {
  vector<double> res(sums.size());
  for (size_t i = 0; i < sums.size(); ++i) {
    res[i] = conv<double>(conv<RR>(sums[i]) / conv<RR>(normalization));
  }
  return res;
}


vector<double> CoalitionalGame::normalizeRawShapley(const vector<ZZ> &sums) {
  return normalizeRawShapley(sums, ZZ(factorialCached(players)));
}
