#include "coal_game.h"
#include <NTL/RR.h>
#include <algorithm>
#include <set>


double CoalGame::v(const vector<int> &coalition) {
  return vFunc(coalition);
}

vector<double> CoalGame::shapley() {
  return shapleyEnum();
}

double CoalGame::shapley(int player) {
  return shapley()[player];
}

vector<double> CoalGame::banzhaf() {
  return banzhafEnum();
}

double CoalGame::banzhaf(int player) {
  return banzhaf()[player];
}

vector<double> CoalGame::shapleyMonteCarlo(int iters) {
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
  for (int i = 0; i < players; ++ i) sh[i] /= iters; // TODO: make double here
  vector<double> res(players);
  for (int i = 0; i < players; ++ i) res[i] = conv<double>(sh[i]);
  return res;
}

vector<double> CoalGame::banzhafMonteCarlo(int iters) {
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

vector<double> CoalGame::banzhafEnum() {
  sums = vector<ZZ>(players);
  vector<int> coal;
  banzhafEnumRec(0, coal);
  return normalizeRawBanzhaf(sums);
}

void CoalGame::banzhafEnumRec(int player, vector<int> &coal) {
  if (player == players) {
    for (int i = (int)coal.size() - 1; i >= 0; --i) {
      const int pl = coal[i];
      sums[pl] += v(coal);
      swap(coal[i], coal.back());
      coal.pop_back();
      sums[pl] += v(coal);
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

vector<double> CoalGame::shapleyEnum() {
  sums = vector<ZZ>(players);
  vector<int> coal;
  shapleyEnumRec(0, coal);
  return normalizeRawShapley(sums);
}

void CoalGame::shapleyEnumRec(int player, vector<int> &coal) {
  if (player == players) {
    const double withAll = log(v(coal));
    for (int i = (int)coal.size() - 1; i >= 0; --i) {
      const int pl = coal[i];
      double incr = withAll;
      swap(coal[i], coal.back());
      coal.pop_back();
      incr -= v(coal);
      coal.push_back(pl);
      sums[pl] += incr * factorial(coal.size() - 1) * factorial(players - coal.size());
    }
    sort(coal.begin(), coal.end());
    return;
  }
  shapleyEnumRec(player + 1, coal);
  coal.push_back(player);
  shapleyEnumRec(player + 1, coal);
  coal.pop_back();
}

double CoalGame::banzhafInteraction(vector<int> inputSubset) {
  return banzhafInteraction(inputSubset);
}

double CoalGame::banzhafInteractionEnum(vector<int> inputSubset) {
  std::set<int> forbidden(inputSubset.begin(), inputSubset.end());
  vector<int> empty;
  ZZ res = banzhafInteractionEnumRec1(0, forbidden, empty, inputSubset);
  res /= power(ZZ(2), players - inputSubset.size());
  return 0; // TODO: return the real result
}

ZZ CoalGame::banzhafInteractionEnumRec1(int player, const std::set<int> &forbidden, vector<int> &curSubset,
                                               const vector<int> &inputSubset) {
  while (forbidden.count(player)) player ++;
  if (player == players) {
    return banzhafInteractionEnumRec2(0, curSubset, inputSubset);
  }
  ZZ res = banzhafInteractionEnumRec1(player + 1, forbidden, curSubset, inputSubset);
  curSubset.push_back(player);
  res += banzhafInteractionEnumRec1(player + 1, forbidden, curSubset, inputSubset);
  curSubset.pop_back();
  return res;
}

ZZ CoalGame::banzhafInteractionEnumRec2(int player, vector<int> &curSubset, const vector<int> &subset, int added) {
  if (player == (int)subset.size()) {
    ZZ res(v(curSubset));
    if ((subset.size() - added) % 2 == 1) res *= -1;
    return res;
  }

  ZZ res;
  res += banzhafInteractionEnumRec2(player + 1, curSubset, subset, added);
  curSubset.push_back(subset[player]);
  res += banzhafInteractionEnumRec2(player + 1, curSubset, subset, added + 1);
  curSubset.pop_back();
  return res;
}

void CoalGame::setBanzhafDenominator(int denom) {
  banzhafDenominator = denom;
}

vector<int> CoalGame::optimalCoalitionOfFixedSize(int size) {
  vector<int> coal;
  bestValue = numeric_limits<double>::min();
  optimalCoalitionOfFixedSizeRec(players, 0, size, coal);
  return bestCoal;
}

int CoalGame::getPlayers() const {
  return players;
}

vector<double> CoalGame::normalizeRawBanzhaf(const vector<ZZ> &sums) {
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

vector<double> CoalGame::normalizeRawShapley(const vector<ZZ> &sums) {
  vector<double> res(sums.size());
  for (size_t i = 0; i < sums.size(); ++i) {
    res[i] = conv<double>(conv<RR>(sums[i]) / conv<RR>(factorial(players)));
  }
  return res;
}

vector<double> CoalGame::normalizeShapleyCounts(const vector<ZZ> &sums) {
  vector<double> res(sums.size());
  for (size_t i = 0; i < sums.size(); ++i) {
    res[i] = conv<double>(conv<RR>(sums[i] * factorial(i) * factorial(players - 1 - i)) / conv<RR>(factorial(players)));
  }
  return res;
}

void CoalGame::optimalCoalitionOfFixedSizeRec(int items, int start_item, int len, vector<int> &coal) {
  if (!len) {
    double curVal = v(coal);
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
