//
// Created by maty on 08/12/19.
//
#include "microarray_game.h"
#include "math.h"

MicroarrayGame::MicroarrayGame(istream &in, bool sparse) {
  if (sparse) {
    int checks_cnt;
    in >> players >> checks_cnt;
    checks = vector<std::set<int>>(checks_cnt);
    for (int i = 0; i < checks_cnt; ++i) {
      int check_size;
      in >> check_size;
      for (int j = 0; j < check_size; ++j) {
        int t;
        in >> t;
        checks[i].insert(t);
      }
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
  vector<ZZ> sums(players);

  vector<ZZ> precomp(players);

  for (auto & check: checks) {
    for (auto & p: check)           {
      // sum over all sizes
      for (int ad = 0; ad <= players - (int)check.size(); ++ ad) {
        ZZ subsets = nChooseK(players - (int)check.size(), ad); // todo: optimize this (rolling n choose k)
        sums[p] += subsets * factorial(check.size() + ad - 1) * factorial(players - check.size() - ad);
      }
    }
  }
  for (int i = 0; i < players; ++i) {
    sums[i] /= checks.size(); // TODO: needs to be RR?
  }
  return normalizeRawShapley(sums);
}

vector<vector<int>> MicroarrayGame::expressionsToFeaturesStd2Groups(const vector<vector<double>> & special, const vector<vector<double>> & control) {
  vector<vector<int>> res = vector<vector<int>>(special.size(), vector<int>(special[0].size(), 0));
  for (size_t i = 0; i < special.size(); ++i) {
    double u = mean(control[i]);
    double s = sd(control[i], u);
    for (size_t j = 0; j < special[i].size(); ++j) {
      if (fabs(special[i][j] - u) >= 6*s) res[i][j] = 1;
    }
  }
  return res;
}

MicroarrayGame::MicroarrayGame(const vector<vector<int>> &mtx) :
    CoalGame(mtx.size()), checks(vector<std::set<int>>(mtx[0].size())) {
  for (size_t i = 0; i < mtx.size(); ++i) {
    for (size_t j = 0; j < mtx[i].size(); ++j) {
      if (mtx[i][j] == 1) checks[j].insert(i);
    }
  }
}

double MicroarrayGame::banzhafInteraction(const vector<int> & inputSubset) {
  ZZ res;
  for (auto & check: checks) {
    // zero if none in the subset is in a check
    std::set<int> freeToSwitch;
    std::set<int> freeToSwitchInSubset = std::set<int>(inputSubset.begin(), inputSubset.end());
    for (int i = 0; i < players; ++i) freeToSwitch.insert(i);
    for (auto i: check) {
      freeToSwitch.erase(i);
      freeToSwitchInSubset.erase(i);
    }
    for (auto i: inputSubset) freeToSwitch.erase(i);

    ZZ cur = power(ZZ(2), freeToSwitch.size());
    ZZ even, odd;
    for (size_t j = 0; j < freeToSwitchInSubset.size(); ++j) {
      if (j%2 == 0) even += nChooseK(freeToSwitchInSubset.size(), j);
      else odd += nChooseK(freeToSwitchInSubset.size(), j);
    }
    if (inputSubset.size()%2) cur *= even - odd;
    else cur *= odd - even;
    res += cur;
  }
  return normalizeRawBanzhaf(vector<ZZ>{res})[0];
}
