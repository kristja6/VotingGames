//
// Created by maty on 08/12/19.
//
#include "microarray_game.h"
#include "math.h"

MicroarrayGame::MicroarrayGame(istream &in, bool sparse) {
  if (sparse) {
    int checks_cnt;
    in >> players >> checks_cnt;
    checks = vector<set<int>>(checks_cnt);
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
  set<int> coal(coalition.begin(), coalition.end());
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
  vector<double> logSum(players, -INF);

  for (auto & check: checks) {
    for (auto & p: check) {
      logInc(logSum[p], (players - check.size()) / log(2));
    }
  }
  normalizeBanzhafLogSums(logSum);
  return logSum;
}

vector<double> MicroarrayGame::shapley() {
  vector<double> logSum(players, -INF);

  vector<double> precomp(players);

  for (auto & check: checks) {
    //cout << (cnt ++) << ' ' << check.size() << endl;
    for (auto & p: check)           {
      // sum over all sizes
      for (int ad = 0; ad <= players - (int)check.size(); ++ ad) {
        double subsets = logChoose(players - (int)check.size(), ad);
        logInc(logSum[p], subsets + logFact(check.size() + ad - 1) + logFact(players - check.size() - ad));
      }
    }
  }
  for (int i = 0; i < players; ++i) {
    logSum[i] -= log(checks.size());
  }
  normalizeShapleyLogSums(logSum);
  return logSum;
}

/*vector<vector<int>> MicroarrayGame::expressionsToFeaturesStd2Groups(const vector<vector<double>> & special, const vector<vector<double>> & control) {
  vector<vector<int>> res = vector<vector<int>>(special.size(), vector<int>(special[0].size(), 0));
  for (size_t i = 0; i < special[0].size(); ++i) {
    double u = mean(control[i]);
    double s = sd(control[i], u);
    for (size_t j = 0; j < control[i].size(); ++j) {
      if (fabs(special[i][j] - u) >= 2*s) res[i][j] = 1;
    }
  }
  return res;
}*/

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
    CoalGame(mtx.size()), checks(vector<set<int>>(mtx[0].size())) {
  for (size_t i = 0; i < mtx.size(); ++i) {
    for (size_t j = 0; j < mtx[i].size(); ++j) {
      if (mtx[i][j] == 1) checks[j].insert(i);
    }
  }
}
