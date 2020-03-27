//
// Created by maty on 07/02/20.
//

#include "voting_nonunique.h"
#include <NTL/RR.h>
#include <assert.h>
#include <algorithm>

VotingNonunique::VotingNonunique(const vector<int> & weights, int quota) : VotingGame(weights, quota),
rollingShapley(emptyTable()) {
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

ZZX VotingNonunique::columnWithOne(int weight, int count) {
  return columnWithOne(weight, count, maxPlayers, quota);
}

ZZX VotingNonunique::columnWithOne(int weight, int count, int maxPlayers, int quota) {
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

void VotingNonunique::banzhafRec(int first, int last, ZZX pf) {
  const int mid = (first + last) / 2;
  if (first == last) {
    const int i = first;
    addToColumnInplace(rollingBanzhaf, uniqueWeights[i], weightCount[i] - 1);
    weightToRes[uniqueWeights[i]] = countSwingsColumn(rollingBanzhaf, pf, uniqueWeights[i]);
    sum += weightToRes[uniqueWeights[i]] * weightCount[i];
    addToColumnInplace(rollingBanzhaf, uniqueWeights[i], 1);
    return;
  }

  ZZX old = pf;
  pf *= mergeRecBanzhaf(first, mid);
  cutPolynom(pf, quota);

  banzhafRec(mid + 1, last, pf);
  banzhafRec(first, mid, old);
}

vector<double> VotingNonunique::banzhafNewOnlyConvolution() {
  weightToRes.clear();
  sum = 0;
  rollingBanzhaf = emptyColumn();
  banzhafRec(0, uniqueWeights.size() - 1, emptyColumn());

  vector<ZZ> sums(players);
  for (int i = 0; i < players; ++i) {
    sums[i] = weightToRes[weights[i]];
  }
  return normalizeRawBanzhaf(sums);
}

vector<double> VotingNonunique::banzhaf() {
  return banzhafNewDp();
}

vector<double> VotingNonunique::shapley(const vector<int> & p) {
  return shapleyNewDp(p);
}

vector<double> VotingNonunique::banzhaf(const vector<int> & p) {
  return banzhafNewDp(p);
}

vector<double> VotingNonunique::banzhafNewWithDeconvolution() {

  ZZX right = emptyColumn();
  ZZX left(uniqueWeights.size());

  left = mergeRecBanzhaf(0, uniqueWeights.size() - 2);

  // get results for all players
  ZZ sum(0);
  for (int i = uniqueWeights.size() - 1; i >= 0; --i) {
    weightToRes[uniqueWeights[i]] = countSwingsColumn(addToColumn(right, uniqueWeights[i], weightCount[i] - 1), left, uniqueWeights[i]);
    sum += weightToRes[uniqueWeights[i]] * weightCount[i];

    if (i > 0) {

      addToColumnInplace(right, uniqueWeights[i], weightCount[i]);
      cutPolynom(right, quota);

      removeFromColumn(left, uniqueWeights[i - 1], weightCount[i - 1]);
    }
  }

  vector<ZZ> sums(players);
  for (int i = 0; i < players; ++ i) sums[i] = weightToRes[weights[i]];

  return normalizeRawBanzhaf(sums);
}

double VotingNonunique::v(const vector<int> &coalition) {
  int sum = 0;
  for (int i: coalition) sum += weights[i];
  return sum >= quota;
}

ZZX VotingNonunique::addToColumn(const ZZX &a, int weight, int count) {
  if (!count) return a;
  ZZX res = a * columnWithOne(weight, count);
  cutPolynom(res, quota);
  return res;
}

void VotingNonunique::addToColumnInplace(ZZX &a, int weight, int count) {
  a *= columnWithOne(weight, count);
  cutPolynom(a, quota);
}

void VotingNonunique::removeFromColumn(ZZX &a, int weight, int count) {
  a = reverse(a, a.rep.length() - 1);
  ZZX t = columnWithOne(weight, count);
  t = reverse(t, t.rep.length() - 1);
  deconvolution(a, t);
  a = reverse(a, a.rep.length() - 1);
}

ZZX VotingNonunique::mergeRecBanzhaf(int st, int en) {
  return mergeRecBanzhaf(st, en, quota);
}

ZZX VotingNonunique::mergeRecBanzhaf(int st, int en, int quota) {
  if (en < 0 || st >= players) return emptyColumn();
  if (st == en) {
    ZZX res = columnWithOne(uniqueWeights[st], weightCount[st]);
    return res;
  }
  ZZX res = mergeRecBanzhaf(st, (st + en) / 2) * mergeRecBanzhaf((st + en) / 2 + 1, en);
  cutPolynom(res, quota);

  return res;
}

double VotingNonunique::shapley(int player) {
  if (!weights[player]) return 0;
  if (shapleyCache.find(weights[player]) != shapleyCache.end())
    return shapleyCache[weights[player]];
  // find index of my weight
  // TODO: make this faster
  int idx = -1;
  for (size_t i = 0; i < uniqueWeights.size(); ++ i) {
    if (weights[player] == uniqueWeights[i]) {
      idx = i;
      break;
    }
  }
  assert(idx != -1);

  auto tab = mergeRecShapley(0, idx - 1) * mergeRecShapley(idx + 1, uniqueWeights.size() - 1);
  addToTableInplace(tab, uniqueWeights[idx], weightCount[idx] - 1);

  ZZ swings = countSwingsTable(tab, uniqueWeights[idx]);
  return shapleyCache[weights[player]] = normalizeRawShapley(vector<ZZ>{swings}, factorial(nonzeroPlayers))[0];
}

vector<double> VotingNonunique::shapleyNewForEachPlayer() {
  vector<double> res(players);
  for (int i = 0; i < players; ++ i) {
    res[i] = shapley(i);
  }
  return res;
}

vector<double> VotingNonunique::shapleyNew() {
  weightToRes.clear();
  rollingShapley = emptyTable();
  shapleyMergeRec(0, uniqueWeights.size() - 1, emptyTable());
  // TODO: make a function for normalization
  vector<double> res(players);
  for (int i = 0; i < players; ++i) {
    RR temp = conv<RR>(weightToRes[weights[i]]);
    temp /= conv<RR>(factorial(players));
    res[i] = conv<double>(temp);
  }
  return res;
}

vector<double> VotingNonunique::shapley() {
  return shapleyNewDp();
}

void VotingNonunique::shapleyMergeRec(int first, int last, const Polynomial2D &pf) {
  const int mid = (first + last) / 2;
  if (first == last) {
    const int i = first;
    addToTableInplace(rollingShapley, uniqueWeights[i], weightCount[i] - 1);
    rollingShapley.shrink(quota, maxPlayers+1);

    weightToRes[uniqueWeights[i]] = countSwingsTable(rollingShapley * pf, uniqueWeights[i]);

    addToTableInplace(rollingShapley, uniqueWeights[i], 1);
    rollingShapley.shrink(quota, maxPlayers+1);
    return;
  }

  Polynomial2D npf = pf;
  npf *= mergeRecShapley(first, mid);
  npf.shrink(quota, maxPlayers+1);

  shapleyMergeRec(mid + 1, last, npf);
  shapleyMergeRec(first, mid, pf);
}

void VotingNonunique::addToTableInplace(Polynomial2D &a, int weight, int count) {
  if (!weight || !count) return;
  a *= tableWithOne(weight, count);
  a.shrink(quota, maxPlayers+1);
}

Polynomial2D VotingNonunique::tableWithOne(int weight, int count) {
  return tableWithOne(weight, count, maxPlayers, quota);
}

Polynomial2D VotingNonunique::tableWithOne(int weight, int count, int maxPlayers, int quota) {
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

Polynomial2D VotingNonunique::mergeRecShapley(int st, int en, int maxPlayers, int quota) {
  if (en < 0 || st > en) return VotingGame::emptyTable();
  if (st == en) {
    return tableWithOne(uniqueWeights[st], weightCount[st]);
  }
  Polynomial2D a = mergeRecShapley(st, (st + en)/2);
  Polynomial2D b = mergeRecShapley((st + en)/2 + 1, en);
  a.efficientMul(b);
  a.shrink(quota, maxPlayers+1);
  return a;
}

Polynomial2D VotingNonunique::mergeRecShapley(int st, int en) {
  return mergeRecShapley(st, en, maxPlayers, quota);
}

vector<double> VotingNonunique::shapleyNewDp() {
  vector<int> p(players);
  dbg << "weights: " << endl;
  for (int i = 0; i < uniqueWeights.size(); ++ i) {
    dbg << uniqueWeights[i] << ": " << weightCount[i] << endl;
  }
  for (int i = 0; i < players; ++i) p[i] = i;
  return shapleyNewDp(p);
}

vector<double> VotingNonunique::shapleyNewDp(const vector<int> & p) {
  unordered_map<int,ZZ> sums;
  vector<vector<ZZ>> left = vector<vector<ZZ>>(maxPlayersAll, vector<ZZ>(quota + maxWeight, ZZ(0)));

  Polynomial2D * temp = new Polynomial2D(mergeRecShapley(0, uniqueWeights.size() - 1, maxPlayersAll, quota + maxWeight));

  for (int i = 0; i < maxPlayersAll; ++i) {
    for (int j = 0; j < quota; ++j) {
      left[i][j] = temp->get(j, i);
      temp->set(j, i, 0);
    }
  }
  delete temp;

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
    // compute sum for player i
    for (int j = 0; j < maxPlayersAll; ++ j) {
      ZZ curCount;
      for (int k = max(0, quota - weights[i]); k < quota; ++ k) {
        curCount += cpy[j][k];
      }
      sums[weights[i]] += curCount * factorial(j) * factorial(nonzeroPlayers - j - 1);
    }
  }
  vector<ZZ> res(players, ZZ(0));
  for (int i = 0; i < players; ++i) {
    res[i] = sums[weights[i]];
  }
  return normalizeRawShapley(res, factorial(nonzeroPlayers));
}

vector<double> VotingNonunique::banzhafNewDp(const vector<int> & p) {
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

vector<double> VotingNonunique::banzhafNewDp() {
  vector<int> p(players);
  for (int i = 0; i < players; ++ i) p[i] = i;
  return banzhafNewDp(p);
}


vector<double> VotingNonunique::banzhafTop(int topN) {
  return VotingNonunique(weights, quota).banzhaf(getTopPlayers(topN));
}

vector<double> VotingNonunique::shapleyTop(int topN) {
  return VotingNonunique(weights, quota).shapley(getTopPlayers(topN));
}
