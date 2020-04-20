//
// Created by maty on 08/12/19.
//
#include <set>
#include <NTL/RR.h>
#include "microarray_game.h"
#include "math.h"

MicroarrayGame::MicroarrayGame(const vector<vector<int>> &mtx) :
    CoalGame(mtx[0].size()), checks(vector<std::set<int>>(mtx.size())) {
  for (size_t i = 0; i < mtx.size(); ++i) {
    for (size_t j = 0; j < mtx[i].size(); ++j) {
      if (mtx[i][j] == 1) checks[i].insert(j);
    }
  }
}

double MicroarrayGame::v(const vector<int> &coalition) {
  if (checks.empty()) return 0;
  std::set<int> coal(coalition.begin(), coalition.end());
  double res = 0;

  for (auto & check: checks) {
    if (check.size() > coal.size()) continue;
    bool pass = true;
    for (int i: check) {
      if (!coal.count(i)) {
        pass = false;
        break;
      }
    }
    res += pass;
  }

  return res / checks.size();
}

vector<double> MicroarrayGame::banzhaf() {
  vector<ZZ> sums(players);

  for (auto & check: checks) {
    for (auto & p: check) {
      sums[p] += power(ZZ(2), (players - check.size() + 1));
    }
  }
  setBanzhafDenominator(BANZHAF_DENOM_SUBSETS); // This is what they use in the relevant paper
  auto res = normalizeRawBanzhaf(sums);
  for (auto & i: res) i /= checks.size();
  return res;
}

vector<double> MicroarrayGame::shapley() {
  vector<double> res(players);
  for (const auto & check: checks) {
    if (!check.size()) continue;
    for (auto i: check) {
      res[i] += 1.0 / check.size();
    }
  }
  for (auto & i: res) i /= checks.size();
  return res;
}
