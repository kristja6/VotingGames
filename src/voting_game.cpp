#include "voting_game.h"
#include "math.h"

long long int VotingGame::v(const vector<int> &coalition) {
  int sum = 0;
  for (int i: coalition) sum += weights[i];
  return sum >= quota;
}

vector<double> VotingGame::emptyColumn() {
  vector<double> res(quota, -INF);
  res[0] = 0;
  return res;
}

vector<double> VotingGame::addToColumn(vector<double> res, ll weight) {
  if (!weight) return res;
  for (int i = quota - 1; i >= weight; -- i) {
    res[i] = logAdd(res[i], res[i - weight]);
  }
  return res;
}

double VotingGame::countSwingsColumn(const vector<double> &a, ll weight) {
  double res = -INF;
  if (!weight) return res;
  for (int i = max(0ll, quota - weight); i < quota; ++ i) {
    res = logAdd(res, a[i]);
  }
  return res;
}

double VotingGame::countSwingsColumn(const vector<double> &a, const vector<double> &b, ll weight) {
  double res = -INF;
  if (!weight) return res;
  vector<double> prefixLogSum(quota);
  prefixLogSum[0] = b[0];
  for (int i = 1; i < quota; ++i) prefixLogSum[i] = logAdd(prefixLogSum[i - 1], b[i]);
  for (int i = 0; i < quota; ++i) {
    // a[i] * all those in b, that fall in interval [quota - weight, quota - 1]
    double intervalSum = prefixLogSum[quota - 1 - i];
    const int lowerEnd = quota - 1 - weight - i;
    if (lowerEnd >= 0) intervalSum = logSub(intervalSum, prefixLogSum[lowerEnd]);
    res = logAdd(res, a[i] + intervalSum);
  }
  return res;
}

vector<double> VotingGame::banzhafDpSlow() {
  vector<double> res(players);
  // count swings for player i
  for (int i = 0; i < players; ++ i) {
    vector<double> c = emptyColumn();
    for (int j = 0; j < players; ++ j) {
      if (i == j) continue;
      c = addToColumn(c, weights[j]);
    }
    res[i] = countSwingsColumn(c, weights[i]);
  }
  normalizeBanzhafLogSums(res);
  return res;
}

void VotingGame::bbRec(ll sum, int idx, vector<bool> has, bool checkSum) {
  if (sum >= quota && checkSum) {
    for (int i = 0; i < idx; ++ i) {
      if (has[i] && sum - weights[i] < quota) {
        bbSums[i] ++;
      }
    }
    checkSum = false;
  }
  if (idx == players) return;

  bbRec(sum, idx + 1, has, checkSum);
  has[idx] = true;
  bbRec(sum + weights[idx], idx + 1, has, true);
}

VotingGame::VotingGame(istream &in) {
  in >> players;
  in >> quota;
  weights = vector<ll>(players);
  for (int i = 0; i < players; ++i) {
    in >> weights[i];
  }
}

vector<double> VotingGame::banzhafDpFast() {
  logSumsRec = vector<double>(players, -INF);
  assert(players == weights.size());
  vector<double> right = emptyColumn();
  vector<double> left = emptyColumn();
  // prepare left
  for (int i = 0; i < players-1; ++i) {
    addToColumnInplace(left, weights[i]);
  }
  // get results for all players
  for (int i = players - 1; i >= 0; --i) {
    logSumsRec[i] = countSwingsColumn(right, left, weights[i]);
    if (i > 0) {
      addToColumnInplace(right, weights[i]);
      removeFromColumnInplace(left, weights[i - 1]);
    }
  }
  normalizeBanzhafLogSums(logSumsRec);
  return logSumsRec;
}

vector<double> VotingGame::banzhafBranchAndBound() {
  bbSums = vector<double>(players);
  bbRec(0, 0, vector<bool>(players, false), true);
  normalizeBanzhafSums(bbSums);
  return bbSums;
}

void VotingGame::addToColumnInplace(vector<double> &a, ll weight) {
  if (!weight) return;
  for (int i = a.size() - 1; i >= weight; --i) {
    logInc(a[i], a[i - weight]);
  }
}

void VotingGame::removeFromColumnInplace(vector<double> &a, ll weight) {
  if (!weight) return;
  for (size_t i = weight; i < a.size(); ++i) {
    logDec(a[i], a[i - weight]);
  }
}

vector<double> VotingGame::shapleyDpFast() {
  logSumsRec = vector<double>(players, -INF);

  matrix left = matrix(players, vector<double>(quota, -INF));
  left[0][0] = 0;
  matrix right = matrix(players, vector<double>(quota, -INF));
  matrix rightPf = matrix(players, vector<double>(quota));
  for (int i = 0; i < players; ++i) {
    right[i][0] = logFact(i) + logFact(players - i - 1);
  }
  // fill up left
  for (int i = 0; i < players - 1; ++i) {
    for (int j = players - 1; j >= 1; --j) {
      for (int k = quota - 1; k >= weights[i]; --k) {
        left[j][k] = logAdd(left[j][k], left[j - 1][k - weights[i]]);
      }
    }
  }

  for (int i = players - 1; i >= 0 ; --i) {
    // compute prefix for right
    for (int j = 0; j < players; ++ j) {
      rightPf[j][0] = right[j][0];
      for (int k = 1; k < quota; ++k) {
        rightPf[j][k] = logAdd(rightPf[j][k - 1], right[j][k]);
      }
    }

    // count result for player i
    for (int j = 0; j < players; ++j) {
      for (int k = 0; k < quota; ++k) {
        const int subIndex = quota - weights[i] - k - 1;
        const double subs = (subIndex < 0 ? 0 : rightPf[j][subIndex]);
        if (subIndex < 0) {
          logInc(logSumsRec[i], left[j][k] + rightPf[j][quota - 1 - k]);
        } else {
          logInc(logSumsRec[i], left[j][k] + logSub(rightPf[j][quota - 1 - k], subs));
        }
      }
    }

    if (i > 0) {
      for (int j = 1; j < players; ++j) {
        for (int k = weights[i - 1]; k < quota; ++k) {
          logDec(left[j][k], left[j - 1][k - weights[i - 1]]);
        }
      }

      // add to the right matrix
      for (int j = 0; j < players - 1; ++j) {
        for (int k = quota - 1; k >= weights[i]; --k) {
          logInc(right[j][k], right[j + 1][k - weights[i]]);
        }
      }
    }
  }
  for (int i = 0; i < players; ++i) {
    logSumsRec[i] = exp(logSumsRec[i] - logFact(players));
  }

  return logSumsRec;
}
