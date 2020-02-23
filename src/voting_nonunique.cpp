//
// Created by maty on 07/02/20.
//

#include "voting_nonunique.h"
#include <NTL/RR.h>

VotingNonunique::VotingNonunique(const vector<ll> & weights, ll quota) : VotingGame(weights, quota),
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
}

ZZX VotingNonunique::columnWithOne(ll weight, ll count) {
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
  pf *= banzhafMergeRec(first, mid);
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
  return banzhafNewOnlyConvolution();
}

vector<double> VotingNonunique::banzhafNewWithDeconvolution() {

  ZZX right = emptyColumn();
  ZZX left(uniqueWeights.size());

  left = banzhafMergeRec(0, uniqueWeights.size() - 2);

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

ZZX VotingNonunique::addToColumn(const ZZX &a, ll weight, ll count) {
  if (!count) return a;
  ZZX res = a * columnWithOne(weight, count);
  cutPolynom(res, quota);
  return res;
}

void VotingNonunique::addToColumnInplace(ZZX &a, ll weight, ll count) {
  a *= columnWithOne(weight, count);
  cutPolynom(a, quota);
}

void VotingNonunique::removeFromColumn(ZZX &a, ll weight, ll count) {
  a = reverse(a, a.rep.length() - 1);
  ZZX t = columnWithOne(weight, count);
  t = reverse(t, t.rep.length() - 1);
  deconvolution(a, t);
  a = reverse(a, a.rep.length() - 1);
}

ZZX VotingNonunique::banzhafMergeRec(int st, int en) {
  if (en < 0 || st >= players) return emptyColumn();
  if (st == en) {
    ZZX res = columnWithOne(uniqueWeights[st], weightCount[st]);
    return res;
  }
  ZZX res = banzhafMergeRec(st, (st + en) / 2) * banzhafMergeRec((st + en) / 2 + 1, en);
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
  return shapleyCache[weights[player]] = normalizeRawShapley(vector<ZZ>{swings})[0];
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
  return shapleyNew();
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

void VotingNonunique::addToTableInplace(Polynomial2D &a, ll weight, ll count) {
  if (!weight || !count) return;
  a *= tableWithOne(weight, count);
  a.shrink(quota, maxPlayers+1);
}

void VotingNonunique::addToTableInplace(SparsePolynomial2D &a, ll weight, ll count) {
  if (!weight || !count) return;
  a *= tableWithOneSparse(weight, count);
}

Polynomial2D VotingNonunique::tableWithOne(ll weight, ll count) {
  Polynomial2D res(weight * count + 1, min((int)count + 1, maxPlayers + 1));
  ZZ nck(1);
  for (int i = 0; i <= min((int)count, maxPlayers+1); ++i) {
    if (i*weight >= quota) break;
    res.set(i*weight, i, nck);
    nck *= (count - i);
    nck /= (i + 1);
  }
  return res;
}

SparsePolynomial2D VotingNonunique::tableWithOneSparse(ll weight, ll count) {
  SparsePolynomial2D res(quota, maxPlayers + 1);
  ZZ nck(1);
  for (int i = 0; i <= min((int)count, maxPlayers+1); ++i) {
    if (i*weight >= quota) break;
    res.data[{i*weight, i}] = nck;
    nck *= (count - i);
    nck /= (i + 1);
  }
  return res;
}

Polynomial2D VotingNonunique::mergeRecShapley(int st, int en) {
  if (en < 0 || st > en) return VotingGame::emptyTable();
  if (st == en) {
    Polynomial2D res = tableWithOne(uniqueWeights[st], weightCount[st]);
    return res;
  }
  Polynomial2D res = mergeRecShapley(st, (st + en)/2) * mergeRecShapley((st + en)/2 + 1, en);
  res.shrink(quota, maxPlayers+1);

  return res;
}

SparsePolynomial2D VotingNonunique::sparseWithOne(int weight, int count) {
  SparsePolynomial2D res(quota, maxPlayers+1);
  ZZ nck(1);
  for (int i = 0; i <= min(count, maxPlayers); ++ i) {
    res.data[{i*weight, i}] = nck;
    nck *= (count - i);
    nck /= (i + 1);
  }
  return res;
}

SparsePolynomial2D VotingNonunique::mergeRecShapleySparse(int st, int en) {
  if (en < 0 || st > en) {
    SparsePolynomial2D res(quota, maxPlayers+1);
    res.data[{0, 0}] = 1;
    return res;
  } else if (st == en) {
    SparsePolynomial2D res = sparseWithOne(uniqueWeights[st], weightCount[st]);
    return res;
  }
  auto r1 = mergeRecShapleySparse(st, (st + en) / 2);
  auto r2 = mergeRecShapleySparse((st + en) / 2 + 1, en);
  return r1 * r2;
}

vector<double> VotingNonunique::shapleyNewDp() {
  unordered_map<ll,ZZ> sums;
  vector<vector<ZZ>> left = vector<vector<ZZ>>(players, vector<ZZ>(2*quota, ZZ(0)));

  ll oldm = maxPlayers;
  maxPlayers = players; // TODO: make this cleaner
  quota *= 2; // TODO: make this cleaner
  auto temp = mergeRecShapley(0, uniqueWeights.size() - 1);

  for (int i = 0; i < players; ++i) {
    for (int j = 0; j < quota; ++j) {
      left[i][j] = temp.get(j, i);
    }
  }
  maxPlayers = oldm;
  quota /= 2;

  for (int i = 0; i < players; ++ i) {
    if (sums.find(weights[i]) != sums.end()) continue;
    auto cpy = left;
    // remove player i from cpy
    for (int j = 1; j < players; ++j) {
      for (int k = weights[i]; k < 2*quota; ++k) {
        cpy[j][k] -= cpy[j - 1][k - weights[i]];
      }
    }
    // compute sum for player i
    for (int j = 0; j < players; ++ j) {
      ZZ curCount;
      for (int k = max(0ll, quota - weights[i]); k < quota; ++ k) {
        curCount += cpy[j][k];
      }
      sums[weights[i]] += curCount * factorial(j) * factorial(players - j - 1);
    }
  }
  vector<ZZ> res(players);
  for (int i = 0; i < players; ++i) {
    res[i] = sums[weights[i]];
  }
  return normalizeRawShapley(res);
}

