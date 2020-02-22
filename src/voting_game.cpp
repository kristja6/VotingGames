#include "voting_game.h"
#include "math.h"
#include "types.h"
#include <NTL/RR.h>

VotingGame::VotingGame(const vector<ll> &weights, long long int quota) :
    CoalGame(weights.size()), weights(weights), quota(quota) {
  cutoffDepth = getCutoffDepth();
  nonzeroPlayers = players;
  for (int i = 0; i < players; ++ i) if (!weights[i]) nonzeroPlayers --;
  precompMaxPlayers();
}

double VotingGame::v(const vector<int> &coalition) {
  int sum = 0;
  for (int i: coalition) sum += weights[i];
  return sum >= quota;
}

ZZX VotingGame::emptyColumn() {
  ZZX res;
  SetCoeff(res, 0, 1);
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

vector<double> VotingGame::banzhafNaiveDp() {
  vector<ZZ> res(players);
  // count swings for player i
  for (int i = 0; i < players; ++ i) {
   ZZX c = emptyColumn();
    for (int j = 0; j < players; ++ j) {
      if (i == j) continue;
      addToColumnInplace(c, weights[j]);
    }
    res[i] = countSwingsColumn(c, emptyColumn(), weights[i]);
  }
  return normalizeRawBanzhaf(res);
}

vector<double> VotingGame::banzhafUnoDp() {
  vector<ZZ> sums(players, ZZ(0));
  assert(players == (int)weights.size());
  ZZX right = emptyColumn();
  ZZX left = mergeRecBanzhaf(0, players - 2);
  ZZ sum(0);

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

  return normalizeRawBanzhaf(sums);
}

vector<double> VotingGame::banzhaf() {
  return banzhafUnoDp();
}

void VotingGame::addToColumnInplace(ZZX &a, ll weight) {
  if (!weight) return;
  for (int i = min(quota, (ll)(a.rep.length() + weight - 1)); i >= weight; --i) {
    SetCoeff(a, i, coeff(a, i) + coeff(a, i - weight));
  }
}

void VotingGame::removeFromColumnInplace(ZZX &a, ll weight) {
  if (!weight) return;
  for (int i = weight; i < a.rep.length(); ++i) {
    SetCoeff(a, i, coeff(a, i) - coeff(a, i - weight));
  }
  a.normalize();
}

vector<double> VotingGame::shapley() {
  return shapleyUnoDp();
}

double VotingGame::shapley(int player) {
  Polynomial2D tab = mergeRecShapleyDense(0, players - 1, maxPlayers) * mergeRecShapleyDense(player + 1, players - 1, maxPlayers);
  tab.shrink(quota, maxPlayers);
  ZZ swings = countSwingsTable(tab, weights[player]);
  return normalizeRawShapley(vector<ZZ>{swings})[0];
}

vector<double> VotingGame::shapleyUnoDp() {
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
  reducedRes = reducedGame.shapleyUnoDpHelper();
  vector<double> res(players, 0);
  for (size_t i = 0; i < mapping.size(); ++i) {
    res[mapping[i]] = reducedRes[i];
  }
  return res;
}

vector<double> VotingGame::shapleyUnoDpHelper() {
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

  return normalizeRawShapley(sums);
}

ll VotingGame::reduceDummyPlayers() {
  ll reducedQuota = quota;
  map<ll, vector<int>> w;
  for (int i = 0; i < players; ++i) {
    w[weights[i]].push_back(i);
  }

  auto right = emptyColumn();
  auto left = emptyColumn();
  // prepare left
  for (int i = 0; i < players-1; ++i) { // TODO: use merge rec
    addToColumnInplace(left, weights[i]);
  }

  // get results for all players
  for (int i = players - 1; i >= 0; --i) {
    if (!banzhaf(i)) {
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
  // TODO: must use number of subsets as the denominator
  BigNum swings = countSwingsColumn(mergeRecBanzhaf(0, player - 1), mergeRecBanzhaf(player + 1, players - 1), weights[player]);
  return normalizeRawBanzhaf(vector<ZZ>{swings})[0];
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
    cout << i << ' ' << flush;
    res[i] = shapley(i);
  }
  cout << endl;
  return res;
}

ZZ VotingGame::countSwingsTable(const Polynomial2D & a, int weight) {
  ZZ res(0);
  for (int i = 0; i < a.columns; ++i) {
    ZZ cur(0);
    for (int j = quota - weight; j < quota; ++j) {
      cur += a.get(j, i);
    }
    res += cur*factorial(i) * factorial(players - i - 1);
  }
  return res;
}

ZZ VotingGame::countSwingsTable(const SparsePolynomial2D & a, int weight) {
  ZZ res(0);
  vector<ZZ> temp(players);
  for (const auto & i: a.data) {
    if (i.first.first >= quota - weight && i.first.first < quota) {
      temp[i.first.second] += i.second;
    }
  }
  for (int i = 0; i < players; ++i) {
    res += temp[i]*factorial(i) * factorial(players - i - 1);
  }
  return res;
}

Polynomial2D VotingGame::mergeRecShapleyDense(int st, int en, int depth) {
  if (en < 0 || st > en) return emptyTable();
  if (depth < cutoffDepth) { // DENSE
    if (st == en) {
      Polynomial2D res = tableWithOne(weights[st]);
      return res;
    }

    auto res = mergeRecShapleyDense(st, (st + en) / 2, depth + 1);
    res *= mergeRecShapleyDense((st + en) / 2 + 1, en, depth + 1);
    res.shrink(quota, maxPlayers);

    return res;

  } else { // SPARSE
    auto ret = mergeRecShapleySparse(st, en);
    Polynomial2D res(quota, en - st + 2);
    for (const auto &i: ret.data) {
      res.set(i.first.first, i.first.second, i.second);
    }
    return res;
  }
}

SparsePolynomial2D VotingGame::mergeRecShapleySparse(int st, int en) {
  if (en < 0 || st > en) {
    SparsePolynomial2D res(quota, maxPlayers + 1);
    res.data[{0, 0}] = 1;
    return res;
  } else if (st == en) {
    SparsePolynomial2D res(quota, maxPlayers + 1);
    res.data[{0, 0}] = 1;
    res.data[{weights[st], 1}] = 1;
    return res;
  }

  auto r1 = mergeRecShapleySparse(st, (st + en) / 2);
  auto r2 = mergeRecShapleySparse((st + en) / 2 + 1, en);
  return r1 * r2;
}

int VotingGame::getCutoffDepth() {
  return round(log2(pow(players, 2.0/3.0)));
}

void VotingGame::precompMaxPlayers() {
  auto wc = weights;
  sort(wc.begin(), wc.end());
  maxPlayers = 1;
  ll cumSum = 0;
  for (size_t i = 0; i < wc.size(); ++ i) {
    if (!wc[i]) continue;
    cumSum += wc[i];
    if (cumSum > quota - 1) break;
    else maxPlayers ++;
  }
  //maxPlayers = players - 1;
  cout << "maxPlayers: " << maxPlayers << endl;
}

const vector<ll> & VotingGame::getWeights() const {
  return weights;
}

ll VotingGame::getQuota() const {
  return quota;
}

