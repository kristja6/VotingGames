#include "voting_game.h"
#include "math.h"
#include "voting_nonunique.h"
#include <NTL/RR.h>
#include <assert.h>
#include <map>
#include <algorithm>

VotingGame::VotingGame(const vector<int> &weights, int quota) :
    CoalGame(weights.size()), weights(weights), quota(quota) {
  nonzeroPlayers = players;
  maxWeight = 0;
  for (int i = 0; i < players; ++ i) {
    assert(this->weights[i] >= 0);
    this->weights[i] = min(quota, this->weights[i]);
    maxWeight = max(maxWeight, (int)this->weights[i]);
    if (!this->weights[i]) nonzeroPlayers --;
  }

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

ZZ VotingGame::countSwingsColumn(const ZZX & a, const ZZX & b, int weight) {
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
    sum += sums[i] = countSwingsColumn(right, left, weights[i]);
    if (i > 0) {
      addToColumnInplace(right, weights[i]);
      removeFromColumnInplace(left, weights[i - 1]);
    }
  }

  return normalizeRawBanzhaf(sums);
}

vector<double> VotingGame::banzhaf() {
  return banzhafUnoDp();
}

void VotingGame::addToColumnInplace(ZZX &a, int weight) {
  if (!weight) return;
  for (int i = min(quota, (int)(a.rep.length() + weight - 1)); i >= weight; --i) {
    SetCoeff(a, i, coeff(a, i) + coeff(a, i - weight));
  }
}

void VotingGame::removeFromColumnInplace(ZZX &a, int weight) {
  if (!weight) return;
  for (int i = weight; i < a.rep.length(); ++i) {
    SetCoeff(a, i, coeff(a, i) - coeff(a, i - weight));
  }
  a.normalize();
}

vector<double> VotingGame::shapley() {
  return VotingNonunique(weights, quota).shapley();
}

vector<double> VotingGame::shapleyNewForEach() {
  vector<double> res(players);
  for (int i = 0; i < players; ++ i) {
    res[i] = shapley(i);
  }
  return res;
}

double VotingGame::shapley(int player) {
  Polynomial2D tab = mergeRecShapleyDense(0, player - 1, maxPlayers) * mergeRecShapleyDense(player + 1, players - 1, maxPlayers);
  tab.shrink(quota, maxPlayers+1);
  ZZ swings = countSwingsTable(tab, weights[player]);
  return normalizeRawShapley(vector<ZZ>{swings})[0];
}

vector<double> VotingGame::shapleyUnoDp() {
  vector<int> mapping;
  vector<int> newWeights;
  int newQuota = reduceDummyPlayers();
  for (int i = 0; i < players; ++i) {
    if (weights[i] == 0) continue;
    mapping.push_back(i);
    newWeights.push_back(weights[i]);
  }
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
  vector<ZZ> sums = vector<ZZ>(players, ZZ(0));

  vector<vector<ZZ>> left = vector<vector<ZZ>>(players, vector<ZZ>(quota, ZZ(0)));
  left[0][0] = 1;
  vector<vector<ZZ>> right = vector<vector<ZZ>>(players, vector<ZZ>(quota, ZZ(0)));
  vector<vector<ZZ>> rightPf = vector<vector<ZZ>>(players, vector<ZZ>(quota, ZZ(0)));


  for (int i = 0; i < players; ++i) {
    right[i][0] = factorial(i) * factorial(players - i - 1);
  }

  // fill up left
  int oldm = maxPlayers;
  maxPlayers = players; // TODO: make this cleaner
  auto temp = mergeRecShapleyDense(0, players - 2, players); // TODO: use original like in Uno
  maxPlayers = oldm;
  for (int i = 0; i < players; ++i) {
    for (int j = 0; j < quota; ++j) {
      left[i][j] = temp.get(j, i);
    }
  }

  for (int i = players - 1; i >= 0 ; --i) {
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
  return normalizeRawShapley(sums);
}

int VotingGame::reduceDummyPlayers() {
  int reducedQuota = quota;
  map<int, vector<int>> w;
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
    if (false) { // TODO: reduce dummy players here
      const int curW = weights[i];
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
  setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  ZZ swings = countSwingsColumn(mergeRecBanzhaf(0, player - 1), mergeRecBanzhaf(player + 1, players - 1), weights[player]);
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
    res[i] = shapley(i);
  }
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

Polynomial2D VotingGame::mergeRecShapleyDense(int st, int en, int depth) {
  if (en < 0 || st > en) return emptyTable();
  else if (st == en) {
    Polynomial2D res = tableWithOne(weights[st]);
    return res;
  }

  auto res = mergeRecShapleyDense(st, (st + en) / 2, depth + 1);
  res *= mergeRecShapleyDense((st + en) / 2 + 1, en, depth + 1);
  res.shrink(quota, maxPlayers+1);

  return res;

}

void VotingGame::precompMaxPlayers() {
  auto wc = weights;
  sort(wc.begin(), wc.end());
  maxPlayersAll = 1;
  maxPlayers = 1;
  int cumSum = 0;
  for (size_t i = 0; i < wc.size(); ++ i) {
    cumSum += wc[i];
    if (cumSum <= quota - 1) maxPlayers ++;
    if (cumSum <= quota - 1 + maxWeight) maxPlayersAll ++;
  }
  maxPlayers = min(maxPlayers, players);
  maxPlayersAll = min(maxPlayersAll, players);
}

const vector<int> & VotingGame::getWeights() const {
  return weights;
}

int VotingGame::getQuota() const {
  return quota;
}

vector<int> VotingGame::getTopPlayers(int topN) {
  topN = max(topN, 1);
  vector<int> p;
  vector<pair<int, int>> weightIdx(players);
  for (int i = 0; i < players; ++i) weightIdx[i].first = -weights[i], weightIdx[i].second = i;
  sort(weightIdx.begin(), weightIdx.end());
  while (p.size() < players && (p.size() < topN || weightIdx[p.size()] == weightIdx[p.size() - 1])) {
    p.push_back(weightIdx[p.size()].second);
  }
  return p;
}
