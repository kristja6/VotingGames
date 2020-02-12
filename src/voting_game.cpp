#include "voting_game.h"
#include "math.h"
#include <NTL/RR.h>

long long int VotingGame::v(const vector<int> &coalition) {
  int sum = 0;
  for (int i: coalition) sum += weights[i];
  return sum >= quota;
}

ZZX VotingGame::emptyColumn() {
  ZZX res;
  SetCoeff(res, 0, 1);
  return res;
}

vector<double> VotingGame::emptyColumnDub() {
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

ZZ VotingGame::countSwingsColumn(const ZZX & a, const ZZX & b, ll weight) {
  ZZ res(0);
  if (!weight) return res;
  vector<ZZ> prefixSum(quota, ZZ(0));
  prefixSum[0] = coeff(b,0);
  for (int i = 1; i < quota; ++i) prefixSum[i] = prefixSum[i - 1] + coeff(b, i);

  for (int i = 0; i < quota; ++i) {
    // a[i] * all those in b, that fall in interval [quota - weight, quota - 1]
    ZZ intervalSum = prefixSum[quota - 1 - i];
    const int lowerEnd = quota - 1 - weight - i;
    if (lowerEnd >= 0) intervalSum = intervalSum - prefixSum[lowerEnd];
    res += coeff(a, i) * intervalSum;
  }
  return res;
}

bool VotingGame::hasAnySwings(const vector<double> &a, const vector<double> &b, ll weight) {
  if (!weight) return false;
  vector<double> prefixLogSum(quota);
  prefixLogSum[0] = b[0];
  for (int i = 1; i < quota; ++i) prefixLogSum[i] = logAdd(prefixLogSum[i - 1], b[i]);
  for (int i = 0; i < quota; ++i) {
    // a[i] * all those in b, that fall in interval [quota - weight, quota - 1]
    double intervalSum = prefixLogSum[quota - 1 - i];
    const int lowerEnd = quota - 1 - weight - i;
    if (lowerEnd >= 0) intervalSum = logSub(intervalSum, prefixLogSum[lowerEnd]);
    if(a[i] + intervalSum > -INF) return true;
  }
  return false;
}


vector<double> VotingGame::banzhafDpSlow() {
  vector<double> res(players);
  // count swings for player i
  for (int i = 0; i < players; ++ i) {
    vector<double> c = emptyColumnDub();
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

vector<double> VotingGame::banzhaf() {
  //logSumsRec = vector<double>(players, -INF);
  vector<ZZ> sums(players, ZZ(0));
  assert(players == (int)weights.size());
  ZZX right = emptyColumn();
  ZZX left = emptyColumn();
  ZZ sum(0);
  // prepare left
  /*for (int i = 0; i < players-1; ++i) {
    cout << i << ' ' << flush;
    addToColumnInplace(left, weights[i]);
  }*/
  left = mergeRec(0, players - 2);
  //exit(0);
  // get results for all players
  for (int i = players - 1; i >= 0; --i) {
    cout << i << ' ' << flush;
    sum += sums[i] = countSwingsColumn(right, left, weights[i]);
    if (i > 0) {
      addToColumnInplace(right, weights[i]);
      removeFromColumnInplace(left, weights[i - 1]);
      cout << right.rep.length() << ' ' << left.rep.length() << endl;
    }
  }
  vector<double> res(players);

  for (int i = 0; i < players; ++i) {
    RR temp = conv<RR>(sums[i]);
    temp /= conv<RR>(sum);
    res[i] = conv<double>(temp);
  }

  return res;
}
map<ll,ll> len;

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

void VotingGame::addToColumnInplace(ZZX &a, ll weight) {
  if (!weight) return;
  for (int i = min(quota, a.rep.length() - 1 + weight); i >= weight; --i) {
    //a[i], a[i - weight]);
    SetCoeff(a, i, coeff(a, i) + coeff(a, i - weight));
  }
}

/*void VotingGame::addToColumnMultiple(vector<double> &a, ll weight, ll count) {
  if (!weight) return;
  for (int i = a.size() - 1; i >= weight; --i) {
    logInc(a[i], a[i - weight]);
  }
}*/

void VotingGame::removeFromColumnInplace(vector<double> &a, ll weight) {
  if (!weight) return;
  for (size_t i = weight; i < a.size(); ++i) {
    logDec(a[i], a[i - weight]);
  }
}

void VotingGame::removeFromColumnInplace(ZZX &a, ll weight) {
  if (!weight) return;
  for (int i = weight; i < a.rep.length(); ++i) {
    //logDec(a[i], a[i - weight]);
    SetCoeff(a, i, coeff(a, i) - coeff(a, i - weight));
  }
  a.normalize();
}

vector<double> VotingGame::shapley() {
  vector<int> mapping;
  vector<ll> newWeights;
  ll newQuota = reduceDummyPlayers();
  for (int i = 0; i < players; ++i) {
    if (weights[i] == 0) continue;
    mapping.push_back(i);
    newWeights.push_back(weights[i]);
  }
  VotingGame reducedGame(newWeights, newQuota);
  auto reducedRes = reducedGame.shapleyHelp();
  vector<double> res(players, 0);
  for (size_t i = 0; i < mapping.size(); ++i) {
    res[mapping[i]] = reducedRes[i];
  }
  return res;
}

vector<double> VotingGame::shapleyHelp() {
  cout << "players: " << players << ", quota: " << quota << endl;
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

ll VotingGame::reduceDummyPlayers() {
  ll reducedQuota = quota;
  map<ll, vector<int>> w;
  for (int i = 0; i < players; ++i) {
    w[weights[i]].push_back(i);
  }

  vector<double> right = emptyColumnDub();
  vector<double> left = emptyColumnDub();
  // prepare left
  for (int i = 0; i < players-1; ++i) {
    addToColumnInplace(left, weights[i]);
  }

  // get results for all players
  for (int i = players - 1; i >= 0; --i) {
    if (!hasAnySwings(left, right, weights[i])) {
      const ll curW = weights[i];
      while (w[curW].size()) {
        weights[w[curW].back()] = 0;
        w[curW].pop_back();
        reducedQuota -= curW;
      }
    }
    if (i > 0) {
      addToColumnInplace(right, weights[i]);
      removeFromColumnInplace(left, weights[i - 1]);
    }
  }
  return reducedQuota;
}

ZZX VotingGame::mergeRec(int st, int en) {
  //cout << "rec: " << st << ' ' << en << endl;
  if (cache.find({st,en}) != cache.end()) return cache[{st,en}];
  if (en < 0 || st >= players) return emptyColumn();
  if (st == en) {
    ZZX res = columnWithOne(weights[st]);
    return res;
  }
  ZZX res = mergeRec(st, (st + en)/2) * mergeRec((st + en)/2 + 1, en);
  cutPolynom(res, quota);


  len[{en - st + 1}] ++;
  return cache[{st,en}] = res;
  //return res;
}

ZZX VotingGame::columnWithOne(int weight) {
  ZZX res;
  SetCoeff(res, 0, 1);
  SetCoeff(res, weight, 1);
  return res;
}
