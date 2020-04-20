//
// Created by maty on 07/02/20.
//

#include "voting_game.h"
#include <NTL/RR.h>
#include <assert.h>
#include <algorithm>

VotingGame::VotingGame(const vector<int> & weights, int quota) : CoalitionalGame(weights.size()), quota(quota), weights(weights) {

  nonzeroPlayers = players;
  maxWeight = 0;
  for (int i = 0; i < players; ++ i) {
    assert(this->weights[i] >= 0);
    this->weights[i] = min(quota, this->weights[i]);
    maxWeight = max(maxWeight, (int)this->weights[i]);
    if (!this->weights[i]) nonzeroPlayers --;
  }

  precompMaxPlayers();





  auto w = weights;
  sort(w.begin(), w.end());
  // decrease unneeded resolution
  for (size_t i = 0; i < w.size(); ++i) {
    if (uniqueWeights.empty() || uniqueWeights.back() != w[i]) {
      uniqueWeights.push_back(w[i]);
      weightCount.push_back(1);
    } else weightCount.back() ++;
  }
  // TODO: put into separate function
  // balance counts of weights by pairing small with large
  std::set<pair<int,int>> pr;
  for (size_t i = 0; i < uniqueWeights.size(); ++ i) {
    pr.insert({weightCount[i], uniqueWeights[i]});
  }
  vector<pair<int,int>> res;
  int turn = 0;
  while (pr.size()) {
    if (turn%2) {
      res.push_back(*pr.begin());
      pr.erase(pr.begin());
    } else {
      res.push_back(*pr.rbegin());
      pr.erase(--pr.end());
    }
    turn ++;
  }

  for (size_t i = 0; i < uniqueWeights.size(); ++ i) {
    uniqueWeights[i] = res[i].second;
    weightCount[i] = res[i].first;
  }
}

ZZX VotingGame::emptyColumn() {
  ZZX res;
  SetCoeff(res, 0, 1);
  return res;
}

ZZX VotingGame::columnWithOne(int weight, int count) {
  return columnWithOne(weight, count, maxPlayersAll, quota);
}

ZZX VotingGame::columnWithOne(int weight, int count, int maxPlayers, int quota) {
  ZZX res = emptyColumn();
  // n choose k
  ZZ nck(1);
  for (int i = 0; i <= count; i ++) {
    if (i*weight >= quota) break;
    SetCoeff(res, i*weight, nck);
    nck *= (count - i);
    nck /= (i + 1);
  }
  return res;
}

vector<double> VotingGame::banzhaf() {
  return banzhafNewDp();
}

vector<double> VotingGame::shapley(const vector<int> & p) {
  return shapleyNewDp(p);
}

vector<double> VotingGame::banzhaf(const vector<int> & p) {
  return banzhafNewDp(p);
}

double VotingGame::v(const vector<int> &coalition) {
  int sum = 0;
  for (int i: coalition) sum += weights[i];
  return sum >= quota;
}

ZZX VotingGame::addToColumn(const ZZX &a, int weight, int count) {
  if (!count) return a;
  ZZX res = a * columnWithOne(weight, count);
  cutPolynom(res, quota);
  return res;
}

void VotingGame::addToColumnInplace(ZZX &a, int weight, int count) {
  a *= columnWithOne(weight, count);
  cutPolynom(a, quota);
}

void VotingGame::removeFromColumn(ZZX &a, int weight, int count) {
  a = reverse(a, a.rep.length() - 1);
  ZZX t = columnWithOne(weight, count);
  t = reverse(t, t.rep.length() - 1);
  deconvolution(a, t);
  a = reverse(a, a.rep.length() - 1);
}

ZZX VotingGame::mergeRecBanzhaf(int st, int en) {
  return mergeRecBanzhaf(st, en, quota);
}

ZZX VotingGame::mergeRecBanzhaf(int st, int en, int quota) {
  if (en < 0 || st >= players) return emptyColumn();
  if (st == en) {
    ZZX res = columnWithOne(uniqueWeights[st], weightCount[st]);
    return res;
  }
  ZZX res = mergeRecBanzhaf(st, (st + en) / 2) * mergeRecBanzhaf((st + en) / 2 + 1, en);
  cutPolynom(res, quota);

  return res;
}

vector<double> VotingGame::shapley() {
  return shapleyNewDp();
}

void VotingGame::addToTableInplace(Polynomial2D &a, int weight, int count) {
  if (!weight || !count) return;
  a *= tableWithOne(weight, count);
  a.shrink(quota, maxPlayers+1);
}

Polynomial2D VotingGame::emptyTable() {
  Polynomial2D res(1, 1);
  res.set(0, 0, 1);
  return res;
}

Polynomial2D VotingGame::tableWithOne(int weight, int count) {
  return tableWithOne(weight, count, maxPlayers, quota);
}

Polynomial2D VotingGame::tableWithOne(int weight, int count, int maxPlayers, int quota) {
  Polynomial2D res(min(weight * count + 1, (int)quota), min((int)count + 1, maxPlayers + 1));
  if (weight == 0) { // this must be corrected when normalizing
    Polynomial2D res(1, 1);
    res.set(0, 0, 1);
    return res;
  }
  ZZ nck(1);
  for (int i = 0; i <= min((int)count, maxPlayers+1); ++i) {
    if (i*weight >= quota) break;
    res.set(i*weight, i, nck);
    nck *= (count - i);
    nck /= (i + 1);
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
    res += cur * factorialCached(i) * factorialCached(players - i - 1);
  }
  return res;
}

Polynomial2D VotingGame::mergeRecShapley(int st, int en, int maxPlayers, int quota) {
  if (en < 0 || st > en) return emptyTable();
  if (st == en) {
    return tableWithOne(uniqueWeights[st], weightCount[st]);
  }
  Polynomial2D a = mergeRecShapley(st, (st + en)/2, maxPlayers, quota);
  Polynomial2D b = mergeRecShapley((st + en)/2 + 1, en, maxPlayers, quota);
  a.efficientMul(b);
  a.shrink(quota, maxPlayers+1);
  return a;
}

Polynomial2D VotingGame::mergeRecShapley(int st, int en) {
  return mergeRecShapley(st, en, maxPlayersAll, quota + maxWeight);
}

vector<double> VotingGame::shapleyNewDp() {
  vector<int> p(players);
  for (int i = 0; i < players; ++i) p[i] = i;
  return shapleyNewDp(p);
}

vector<double> VotingGame::shapleyNewDp(const vector<int> & p) {
  unordered_map<int,double> sums;
  vector<vector<ZZ>> left = vector<vector<ZZ>>(maxPlayersAll, vector<ZZ>(quota + maxWeight, ZZ(0)));
  Polynomial2D * temp = new Polynomial2D(mergeRecShapley(0, (int)uniqueWeights.size() - 1));
  for (int i = 0; i < maxPlayersAll; ++i) {
    for (int j = 0; j < quota + maxWeight; ++j) {
      left[i][j] = temp->get(j, i);
      temp->set(j, i, 0);
    }
  }
  delete temp;

  ZZ largeFact = factorialNoCache(nonzeroPlayers - 1);
  RR normFact = conv<RR>(largeFact * nonzeroPlayers);

  unordered_map<int,vector<int>> weightToPlayers;
  for (int i = 0; i < players; ++ i) weightToPlayers[weights[i]].push_back(i);

  for (size_t idx = 0; idx < p.size(); ++ idx) {
    const int i = p[idx];
    if (sums.find(weights[i]) != sums.end()) continue;
    auto cpy = left;
    // remove player i from cpy
    for (int j = 1; j < maxPlayersAll; ++j) {
      for (int k = weights[i]; k < quota + weights[i]; ++k) {
        cpy[j][k] -= cpy[j - 1][k - weights[i]];
      }
    }
    ZZ fact = largeFact;
    ZZ curSum;
    // compute sum for player i
    for (int j = 0; j < maxPlayersAll; ++ j) {
      if (j > 0) {
        fact *= j;
        fact /= nonzeroPlayers - j;
      }
      ZZ curCount;
      for (int k = max(0, quota - weights[i]); k < quota; ++ k) {
        curCount += cpy[j][k];
      }
      curSum += curCount * fact;
    }
    sums[weights[i]] = conv<double>(conv<RR>(curSum) / normFact);
  }
  vector<double> res(players, 0);
  for (int i = 0; i < players; ++i) {
    res[i] = sums[weights[i]];
  }
  return res;
}

vector<double> VotingGame::banzhafNewDp(const vector<int> & p) {
  unordered_map<int,ZZ> sums;
  vector<ZZ> left = vector<ZZ>(quota + maxWeight, ZZ(0));

  auto temp = mergeRecBanzhaf(0, uniqueWeights.size() - 1, quota + maxWeight);

  for (int i = 0; i < quota + maxWeight; ++i) {
    left[i] = coeff(temp, i);
  }

  for (size_t idx = 0; idx < p.size(); ++ idx) {
    const int i = p[idx];
    if (sums.find(weights[i]) != sums.end()) continue;
    auto cpy = left;
    // remove player i from cpy
    for (int k = weights[i]; k < quota + weights[i]; ++k) {
      cpy[k] -= cpy[k - weights[i]];
    }
    // compute sum for player i
    ZZ curCount;
    for (int k = max(0, quota - weights[i]); k < quota; ++ k) {
      curCount += cpy[k];
    }
    sums[weights[i]] = curCount;
  }
  vector<ZZ> res(players, ZZ(-1));
  for (int i = 0; i < p.size(); ++i) {
    res[p[i]] = sums[weights[p[i]]];
  }
  return normalizeRawBanzhaf(res);
}

vector<double> VotingGame::banzhafNewDp() {
  vector<int> p(players);
  for (int i = 0; i < players; ++ i) p[i] = i;
  return banzhafNewDp(p);
}

vector<double> VotingGame::banzhafTop(int topN) {
  return VotingGame(weights, quota).banzhaf(getTopPlayers(topN));
}

vector<double> VotingGame::shapleyTop(int topN) {
  return VotingGame(weights, quota).shapley(getTopPlayers(topN));
}

void VotingGame::precompMaxPlayers() {
  auto wc = weights;
  sort(wc.begin(), wc.end());
  maxPlayersAll = 1;
  maxPlayers = 1;
  int cumSum = 0;
  for (size_t i = 0; i < wc.size(); ++ i) {
    // skip zero players
    if (wc[i] == 0) continue;
    cumSum += wc[i];
    if (cumSum <= quota - 1) maxPlayers ++;
    if (cumSum <= quota - 1 + maxWeight) maxPlayersAll ++;
  }
  maxPlayers = min(maxPlayers, nonzeroPlayers);
  maxPlayersAll = min(maxPlayersAll, nonzeroPlayers);
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
