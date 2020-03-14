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
      sums[p] += power(ZZ(2), (players - check.size()));
    }
  }
  return normalizeRawBanzhaf(sums);
}

vector<double> MicroarrayGame::shapley() {
  vector<double> res(players);
  RR factPlayers = conv<RR>(factorialNoCache(players));

  for (auto & check: checks) {
    // sum over all sizes
    ZZ swings;
    ZZ cnk(1);
    ZZ firstFact = factorialNoCache(check.size() - 1);
    ZZ secondFact = factorialNoCache(players - check.size());
    for (int ad = 0; ad <= players - (int)check.size(); ++ ad) {
      swings += cnk * firstFact * secondFact;

      if (ad == players - (int)check.size()) break;
      firstFact *= check.size() - 1 + (ad + 1);
      secondFact /= players - check.size() - ad;
      cnk *= (players - (int)check.size() - ad);
      cnk /= (ad + 1);
    }
    const auto sp = conv<double>(conv<RR>(swings) / factPlayers);
    for (auto & p: check) {
      res[p] += sp;
    }
  }
  return res;
}
