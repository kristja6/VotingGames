#include "voting_game.h"
#include "math.h"
#include "types.h"
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

vector<LogNum> VotingGame::emptyColumnLogNum(int quota) {
  vector<LogNum> res(quota);
  res[0] = 1;
  return res;
}

vector<LogNum> VotingGame::addToColumn(vector<LogNum> res, ll weight, int quota) {
  if (!weight) return res;
  for (int i = quota - 1; i >= weight; -- i) {
    res[i] += res[i - weight];
  }
  return res;
}

LogNum VotingGame::countSwingsColumn(const vector<LogNum> &a, ll weight, int quota) {
  LogNum res;
  if (!weight) return res;
  for (int i = max(0ll, quota - weight); i < quota; ++ i) {
    res += a[i];
  }
  return res;
}

LogNum VotingGame::countSwingsColumn(const vector<LogNum> &a, const vector<LogNum> &b, ll weight, int quota) {
  LogNum res;
  if (!weight) return res;
  vector<LogNum> prefixLogSum(quota);
  prefixLogSum[0] = b[0];
  for (int i = 1; i < quota; ++i) prefixLogSum[i] = prefixLogSum[i - 1] + b[i];
  for (int i = 0; i < quota; ++i) {
    // a[i] * all those in b, that fall in interval [quota - weight, quota - 1]
    LogNum intervalSum = prefixLogSum[quota - 1 - i];
    const int lowerEnd = quota - 1 - weight - i;
    if (lowerEnd >= 0) intervalSum -= prefixLogSum[lowerEnd];
    res += a[i] + intervalSum;
  }
  return res;
}

ZZ VotingGame::countSwingsColumn(const ZZX & a, const ZZX & b, ll weight, int quota) {
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

bool VotingGame::hasAnySwings(const vector<LogNum> &a, const vector<LogNum> &b, ll weight, int quota) {
  if (!weight) return false;
  vector<LogNum> prefixLogSum(quota);
  prefixLogSum[0] = b[0];
  for (int i = 1; i < quota; ++i) prefixLogSum[i] = prefixLogSum[i - 1] + b[i];
  for (int i = 0; i < quota; ++i) {
    // a[i] * all those in b, that fall in interval [quota - weight, quota - 1]
    LogNum intervalSum = prefixLogSum[quota - 1 - i];
    const int lowerEnd = quota - 1 - weight - i;
    if (lowerEnd >= 0) intervalSum -= prefixLogSum[lowerEnd];
    if(a[i] + intervalSum > 0) return true;
  }
  return false;
}


vector<double> VotingGame::banzhafDpSlow() {
  vector<LogNum> res(players);
  // count swings for player i
  for (int i = 0; i < players; ++ i) {
    vector<LogNum> c = emptyColumnLogNum(quota);
    for (int j = 0; j < players; ++ j) {
      if (i == j) continue;
      c = addToColumn(c, weights[j], quota);
    }
    res[i] = countSwingsColumn(c, weights[i], quota);
  }
  normalizeBanzhafLogSums(res);
  return toNormal(res);
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
 // todo: use max players her
VotingGame::VotingGame(istream &in) {
  in >> players;
  in >> quota;
  weights = vector<ll>(players);
  nonzeroPlayers = players;
  for (int i = 0; i < players; ++i) {
    in >> weights[i];
    if (!weights[i]) nonzeroPlayers --;
  }
  cutoffDepth = getCutoffDepth();
  precompMaxPlayers();
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
  left = mergeRecBanzhaf(0, players - 2);
  //exit(0);
  // get results for all players
  for (int i = players - 1; i >= 0; --i) {
    cout << i << ' ' << flush;
    sum += sums[i] = countSwingsColumn(right, left, weights[i], quota);
    if (i > 0) {
      addToColumnInplace(right, weights[i], quota);
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

void VotingGame::addToColumnInplace(vector<LogNum> &a, ll weight) {
  if (!weight) return;
  for (int i = a.size() - 1; i >= weight; --i) {
    a[i] += a[i - weight];
  }
}

void VotingGame::addToColumnInplace(ZZX &a, ll weight, int quota) {
  if (!weight) return;
  for (int i = min(quota, (int)(a.rep.length() + weight - 1)); i >= weight; --i) {
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

void VotingGame::removeFromColumnInplace(vector<LogNum> &a, ll weight) {
  if (!weight) return;
  for (size_t i = weight; i < a.size(); ++i) {
    a[i] -= a[i - weight];
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
  return shapley(false);
}

double VotingGame::shapley(int player) {

  Polynomial2D tab = mergeRecShapleyDense(0, players - 1, maxPlayers) * mergeRecShapleyDense(player + 1, players - 1, maxPlayers);
  tab.shrink(quota, maxPlayers);
  ZZ swings = countSwingsTable(tab, weights[player], quota, players);
  RR temp = conv<RR>(swings);
  temp /= conv<RR>(factorial(players));
  return conv<double>(temp);
}

vector<double> VotingGame::shapley(bool logNum) {
  vector<int> mapping;
  vector<ll> newWeights;
  ll newQuota = reduceDummyPlayers();
  for (int i = 0; i < players; ++i) {
    if (weights[i] == 0) continue;
    mapping.push_back(i);
    newWeights.push_back(weights[i]);
  }
  cout << "after reducing: " << newWeights.size() << ' ' << newQuota << endl;
  VotingGame reducedGame(newWeights, newQuota);
  vector<double> reducedRes;
  if(logNum) reducedRes = reducedGame.shapleyLogNumHelp();
  else reducedRes = reducedGame.shapleyHelp();
  vector<double> res(players, 0);
  for (size_t i = 0; i < mapping.size(); ++i) {
    res[mapping[i]] = reducedRes[i];
  }
  return res;
}

vector<double> VotingGame::shapleyLogNumHelp() {
  cout << "players: " << players << ", quota: " << quota << endl;
  cout << "using log num" << endl;
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
    cout << i << ' ' << flush;
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
  for (int i = 0; i < left.size(); ++i) {
    for (int j = 0; j < left[i].size(); ++j) {
      cout << left[i][j] << ' ';
    }
    cout << endl;
  }
  cout << endl;
  cout << endl;
  for (int i = 0; i < players; ++i) {
    logSumsRec[i] = exp(logSumsRec[i] - logFact(players));
  }

  return logSumsRec;
}

vector<double> VotingGame::shapleyHelp() {
  cout << "players: " << players << ", quota: " << quota << endl;
  vector<ZZ> sums = vector<ZZ>(players, ZZ(0));

  vector<vector<ZZ>> left = vector<vector<ZZ>>(players, vector<ZZ>(quota, ZZ(0)));
  left[0][0] = 1;
  vector<vector<ZZ>> right = vector<vector<ZZ>>(players, vector<ZZ>(quota, ZZ(0)));
  vector<vector<ZZ>> rightPf = vector<vector<ZZ>>(players, vector<ZZ>(quota, ZZ(0)));


  for (int i = 0; i < players; ++i) {
    right[i][0] = factorial(i) * factorial(players - i - 1);
  }

  // fill up left
  /*for (int i = 0; i < players - 1; ++i) {
    for (int j = players - 1; j >= 1; --j) {
      for (int k = quota - 1; k >= weights[i]; --k) {
        left[j][k] += left[j - 1][k - weights[i]];
      }
    }
  }*/
  auto temp = mergeRecShapleyDense(0, players - 2, players);
  for (int i = 0; i < players - 1; ++i) {
    for (int j = 0; j < quota; ++j) {
      left[i][j] = temp.get(j, i);
    }
  }

  for (int i = players - 1; i >= 0 ; --i) {
    cout << i << ' ' << flush;

    // compute prefix for right
    for (int j = 0; j < players; ++ j) {
      rightPf[j][0] = right[j][0];
      for (int k = 1; k < quota; ++k) {
        rightPf[j][k] = rightPf[j][k - 1] + right[j][k];
      }
    }

    // count result for player i
    for (int j = 0; j < players; ++j) {
      for (int k = 0; k < quota; ++k) {
        const int subIndex = quota - weights[i] - k - 1;
        const ZZ subs = (subIndex < 0 ? ZZ(0) : rightPf[j][subIndex]);
        if (subIndex < 0) {
          sums[i] += left[j][k] * rightPf[j][quota - 1 - k];
        } else {
          sums[i] += left[j][k] * (rightPf[j][quota - 1 - k] - subs);
        }
      }
    }

    if (i > 0) {
      for (int j = 1; j < players; ++j) {
        for (int k = weights[i - 1]; k < quota; ++k) {
          left[j][k] -= left[j - 1][k - weights[i - 1]];
        }
      }

      // add to the right matrix
      for (int j = 0; j < players - 1; ++j) {
        for (int k = quota - 1; k >= weights[i]; --k) {
          right[j][k] += right[j + 1][k - weights[i]];
        }
      }
    }
  }
  cout << endl;

  vector<double> res(players);
  for (int i = 0; i < players; ++i) {
    RR temp = conv<RR>(sums[i]);
    temp /= conv<RR>(factorial(players));
    res[i] = conv<double>(temp);
  }

  return res;
}

ll VotingGame::reduceDummyPlayers() {
  ll reducedQuota = quota;
  map<ll, vector<int>> w;
  for (int i = 0; i < players; ++i) {
    w[weights[i]].push_back(i);
  }

  vector<LogNum> right = emptyColumnLogNum(quota);
  vector<LogNum> left = emptyColumnLogNum(quota);
  // prepare left
  for (int i = 0; i < players-1; ++i) {
    addToColumnInplace(left, weights[i]);
  }

  // get results for all players
  for (int i = players - 1; i >= 0; --i) {
    if (!hasAnySwings(left, right, weights[i], quota)) {
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

ZZX VotingGame::mergeRecBanzhaf(int st, int en) {
  if (en < 0 || st >= players) return emptyColumn();
  if (st == en) {
    ZZX res = columnWithOne(weights[st]);
    return res;
  }
  ZZX res = mergeRecBanzhaf(st, (st + en) / 2) * mergeRecBanzhaf((st + en) / 2 + 1, en);
  cutPolynom(res, quota);

  return res;
}

ZZX VotingGame::columnWithOne(int weight) {
  ZZX res;
  SetCoeff(res, 0, 1);
  SetCoeff(res, weight, 1);
  return res;
}

double VotingGame::banzhaf(int player) {
  if (!weights[player]) return 0;
  /*if (banzhafDenominator == BANZHAF_DENOM_WINNING) {
    throw "can't compute just one!";
  }*/
  // TODO: must use number of subsets as the denominator
  BigNum swings = countSwingsColumn(mergeRecBanzhaf(0, player - 1), mergeRecBanzhaf(player + 1, players - 1), weights[player], quota);
  RR temp = conv<RR>(swings);
  temp /= conv<RR>(power(ZZ(2), nonzeroPlayers - 1));
  return conv<double>(temp);
}


Polynomial2D VotingGame::emptyTable() {
  Polynomial2D res(1, 1);
  res.set(0, 0, 1);
  return res;
}

Polynomial2D VotingGame::tableWithOne(int weight) {
  Polynomial2D res(weight + 1, 2);
  res.set(0, 0, 1);
  res.set(weight, 1, 1);
  return res;
}

vector<double> VotingGame::shapleyNew() {
  vector<double> res(players);
  ZZ f = factorial(players);
  for (int i = 0; i < players; ++i) {
    cout << i << ' ' << flush; // todo: use max players here
    res[i] = shapley(i);
  }
  cout << endl;
  return res;
}

ZZ VotingGame::countSwingsTable(const Polynomial2D & a, int weight, int quota, int players) {
  ZZ res(0);
  for (int i = 0; i < a.columns; ++i) {
    ZZ cur(0);
    for (int j = quota - weight; j < quota; ++j) {
      cur += a.get(j, i);
    }
    res += cur*factorial(i) * factorial(players - i - 1);
    //cout << i << ": " << log(max(ZZ(1), cur*factorial(i) * factorial(players - i - 1))) << endl;
  }
  return res;
}

Polynomial2D VotingGame::mergeRecShapleyDense(int st, int en, int maxPlayers, int depth) {
  if (en < 0 || st > en) return emptyTable();
  if (depth < cutoffDepth) { // DENSE
    if (st == en) {
      Polynomial2D res = tableWithOne(weights[st]);
      return res;
    }

    auto res = mergeRecShapleyDense(st, (st + en) / 2, maxPlayers, depth + 1);
    res *= mergeRecShapleyDense((st + en) / 2 + 1, en, maxPlayers, depth + 1);
    //res.cutRows(quota);
    //res.cutColumns(maxPlayers);
    res.shrink(quota, maxPlayers);

    return res;

  } else { // SPARSE
    auto ret = mergeRecShapleySparse(st, en, maxPlayers);
    Polynomial2D res(quota, en - st + 2);
    for (const auto &i: ret) {
      res.set(i.first.first, i.first.second, i.second);
    }
    return res;
  }
}

unordered_map<pair<int, int>, ZZ, IntPairHash> VotingGame::mergeRecShapleySparse(int st, int en, int maxPlayers) {
  //cout << st << ' ' << en << endl;
  if (en < 0 || st > en) {
    unordered_map<pair<int, int>, ZZ, IntPairHash> res;
    res[{0, 0}] = 1;
  } else if (st == en) {
    unordered_map<pair<int, int>, ZZ, IntPairHash> res;
    res[{0, 0}] = 1;
    res[{weights[st], 1}] = 1;
    return res;
  }
  auto r1 = mergeRecShapleySparse(st, (st + en) / 2, maxPlayers);
  auto r2 = mergeRecShapleySparse((st + en) / 2 + 1, en, maxPlayers);
  // merge
  unordered_map<pair<int, int>, ZZ, IntPairHash> res;
  for (const auto &i: r1) {
    for (const auto &j: r2) {
      ZZ r = i.second * j.second;
      if (r < quota && i.second + j.second <= maxPlayers)
        res[{i.first.first + j.first.first, i.first.second + j.first.second}] += r;
    }
  }
  return res;
}

int VotingGame::getCutoffDepth() {
  return round(log2(pow(players, 2.0/3.0)));
}

void VotingGame::precompMaxPlayers() {
  auto wc = weights;
  sort(wc.begin(), wc.end());
  maxPlayers = 0;
  ll cumSum = 0;
  for (int i = 0; i < wc.size(); ++ i) {
    cumSum += wc[i];
    if (cumSum > quota - 1) break;
    else maxPlayers ++;
  }
  //maxPlayers = players - 1;
  cout << "maxPlayers: " << maxPlayers << endl;
}
