//
// Created by maty on 07/02/20.
//

#include "voting_nonunique.h"
#include <NTL/RR.h>

VotingNonunique::VotingNonunique(vector<ll> weights, ll quota) : CoalGame(weights.size()), origWeights(weights)
, quota(quota), rollingShapley(VotingGame::emptyTable()) {
  sort(weights.begin(), weights.end());
  // decrease unneeded resolution
  for (size_t i = 0; i < weights.size(); ++i) {
    if (weights[i] == 0) {
      players --;
      continue;
    }
    if (w.empty() || w.back() != weights[i]) {
      w.push_back(weights[i]);
      cnt.push_back(1);
    } else cnt.back() ++;
  }
  // remove zero players
  precompMaxPlayers();
  cout << "quota: " << quota << endl;
  for (int i = 0; i < w.size(); ++ i) {
    cout << w[i] << ": " << cnt[i] << endl;
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
    addToColumnInplace(rollingBanzhaf, w[i], cnt[i] - 1);
    weightToRes[w[i]] = countSwingsColumn(rollingBanzhaf, pf, w[i]);
    sum += weightToRes[w[i]] * cnt[i];
    addToColumnInplace(rollingBanzhaf, w[i], 1);
    return;
  }

  ZZX old = pf;
  pf *= mergeRec(first, mid);
  cutPolynom(pf, quota);

  banzhafRec(mid + 1, last, pf);
  banzhafRec(first, mid, old);
}

vector<double> VotingNonunique::banzhaf() {
  cout << "recursion based (diff normalization)" << endl;
  cout << "q = " << quota << ", u = " << w.size() << ", n = " << origWeights.size() << endl;
  weightToRes.clear();
  sum = 0;
  rollingBanzhaf = emptyColumn();
  banzhafRec(0, w.size() - 1, emptyColumn());
  // TODO: make a function for normalization
  vector<double> res(players);
  for (int i = 0; i < players; ++i) {
    RR temp = conv<RR>(weightToRes[origWeights[i]]);
    temp /= conv<RR>(sum);
    //temp /= conv<RR>(power(ZZ(2), players - 1));
    res[i] = conv<double>(temp);
  }
  return res;
}

vector<double> VotingNonunique::banzhafSlowUnmerge() {
  cout << "q = " << quota << ", u = " << w.size() << ", n = " << origWeights.size() << endl;
  map<ll,ZZ> weightToRes;

  ZZX right = emptyColumn();
  ZZX left(w.size());

  left = mergeRec(0, w.size() - 2);

  // get results for all players
  ZZ sum(0);
  for (int i = w.size() - 1; i >= 0; --i) {
    weightToRes[w[i]] = countSwingsColumn(addToColumn(right, w[i], cnt[i] - 1), left, w[i]);
    sum += weightToRes[w[i]] * cnt[i];

    if (i > 0) {

      addToColumnInplace(right, w[i], cnt[i]);
      cutPolynom(right, quota);

      removeFromColumn(left, w[i - 1], cnt[i - 1]);
      cout << i << ": " << left.rep.length() << ' ' << right.rep.length() << endl;
    }
    cout << endl;
  }

  vector<double> res(players);

  for (int i = 0; i < players; ++i) {
    RR temp = conv<RR>(weightToRes[origWeights[i]]);
    temp /= conv<RR>(sum);
    res[i] = conv<double>(temp);
  }

  return res;
}

vector<double> VotingNonunique::banzhafSlow() {
  cout << "q = " << quota << ", u = " << w.size() << ", n = " << origWeights.size() << endl;
  map<ll,ZZ> weightToRes;

  ZZX right = emptyColumn();
  ZZX left(w.size());


  // get results for all players
  ZZ sum(0);
  for (int i = w.size() - 1; i >= 0; --i) {
    left = mergeRec(0, i - 1);
    weightToRes[w[i]] = countSwingsColumn(addToColumn(right, w[i], cnt[i] - 1), left, w[i]);
    sum += weightToRes[w[i]] * cnt[i];

    if (i > 0) {

      addToColumnInplace(right, w[i], cnt[i]);
      cutPolynom(right, quota);
      cout << i << ": " << left.rep.length() << ' ' << right.rep.length() << endl;
    }
    cout << endl;
  }

  vector<double> res(players);

  for (int i = 0; i < players; ++i) {
    RR temp = conv<RR>(weightToRes[origWeights[i]]);
    temp /= conv<RR>(sum);
    res[i] = conv<double>(temp);
  }

  return res;
}

ll VotingNonunique::v(const vector<int> &coalition) {
  int sum = 0;
  for (int i: coalition) sum += origWeights[i];
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

ZZX VotingNonunique::emptyColumn() {
  ZZX res;
  SetCoeff(res, 0, 1);
  return res;
}

ZZ VotingNonunique::countSwingsColumn(const ZZX & a, const ZZX & b, ll weight) {
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

ZZX VotingNonunique::mergeRec(int st, int en) {
  if (en < 0 || st >= players) return emptyColumn();
  if (st == en) {
    ZZX res = columnWithOne(w[st], cnt[st]);
    return res;
  }
  ZZX res = mergeRec(st, (st + en)/2) * mergeRec((st + en)/2 + 1, en);
  cutPolynom(res, quota);

  return res;
}

double VotingNonunique::shapley(int player) {
  if (!origWeights[player]) return 0;
  if (shapleyCache.find(origWeights[player]) != shapleyCache.end())
    return shapleyCache[origWeights[player]];
  // find index of my weight
  int idx = -1;
  for (int i = 0; i < w.size(); ++ i) {
    if (origWeights[player] == w[i]) {
      idx = i;
      break;
    }
  }
  assert(idx != -1);

  //auto tab = mergeRecShapleySparse(0, idx - 1) * mergeRecShapleySparse(idx + 1, w.size() - 1);
  auto tab = mergeRecShapley(0, idx - 1) * mergeRecShapley(idx + 1, w.size() - 1);
  //tab.cutRows(quota);
  //tab.shrink(quota, maxPlayers);
  addToTableInplace(tab, w[idx], cnt[idx] - 1);

  ZZ swings = VotingGame::countSwingsTable(tab, w[idx], quota, players);

  //cout << swings << endl;
  RR temp = conv<RR>(swings);
  temp /= conv<RR>(factorial(players));
  return shapleyCache[origWeights[player]] = conv<double>(temp);
}

vector<double> VotingNonunique::shapleyTest() {
  cout << "Shapley test: q = " << quota << ", u = " << w.size() << ", n = " << players << endl;
  cout << origWeights.size() << endl;
  vector<double> res(players);
  for (int i = 0; i < players; ++ i) {
    cout << i << ' ' << flush;
    res[i] = shapley(i);
  }
  cout << endl;
  return res;
}

vector<double> VotingNonunique::shapley() {
  cout << "Shapley: q = " << quota << ", u = " << w.size() << ", n = " << players << endl;
  cout << "max players: " << maxPlayers << endl;
  for (int i = 0; i < w.size(); ++ i) {
    cout << w[i] << ": " << cnt[i] << endl;
  }

  weightToRes.clear();
  rollingShapley = VotingGame::emptyTable();
  shapleyRec(0, w.size() - 1, VotingGame::emptyTable());
  cout << endl;
  // TODO: make a function for normalization
  vector<double> res(players);
  for (int i = 0; i < players; ++i) {
    RR temp = conv<RR>(weightToRes[origWeights[i]]);
    temp /= conv<RR>(factorial(players));
    res[i] = conv<double>(temp);
  }
  return res;
}

void VotingNonunique::shapleyRec(int first, int last, const Polynomial2D & pf) {
  const int mid = (first + last) / 2;
  if (first == last) {
    cout << first << ' ' << flush;
    const int i = first;
    addToTableInplace(rollingShapley, w[i], cnt[i] - 1);
    //rollingShapley.cutRows(quota);
    rollingShapley.shrink(quota, maxPlayers);

    weightToRes[w[i]] = VotingGame::countSwingsTable(rollingShapley * pf, w[i], quota, players);

    addToTableInplace(rollingShapley, w[i], 1);
    //rollingShapley.cutRows(quota);
    rollingShapley.shrink(quota, maxPlayers);
    return;
  }

  Polynomial2D npf = pf;
  npf *= mergeRecShapley(first, mid);
  //npf.cutRows(quota);
  npf.shrink(quota, maxPlayers);

  shapleyRec(mid + 1, last, npf);
  shapleyRec(first, mid, pf);
}

void VotingNonunique::addToTableInplace(Polynomial2D &a, ll weight, ll count) {
  if (!weight || !count) return;
  a *= tableWithOne(weight, count);
  //a.cutRows(quota);
  a.shrink(quota, maxPlayers);
}

void VotingNonunique::addToTableInplace(SparsePolynomial2D &a, ll weight, ll count) {
  if (!weight || !count) return;
  a *= tableWithOneSparse(weight, count);
}

Polynomial2D VotingNonunique::tableWithOne(ll weight, ll count) {
  Polynomial2D res(weight * count + 1, min((int)count + 1, maxPlayers + 1));
  ZZ nck(1);
  for (int i = 0; i <= min((int)count, maxPlayers); ++i) {
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
  for (int i = 0; i <= min((int)count, maxPlayers); ++i) {
    if (i*weight >= quota) break;
    res.data[{i*weight, i}] = nck;
    nck *= (count - i);
    nck /= (i + 1);
  }
  return res;
}

Polynomial2D VotingNonunique::mergeRecShapley(int st, int en) {
  if (en < 0 || st >= w.size()) return VotingGame::emptyTable();
  if (st == en) {
    Polynomial2D res = tableWithOne(w[st], cnt[st]);
    return res;
  }
  Polynomial2D res = mergeRecShapley(st, (st + en)/2) * mergeRecShapley((st + en)/2 + 1, en);
  //res.cutRows(quota);
  res.shrink(quota, maxPlayers);

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
    SparsePolynomial2D res = sparseWithOne(w[st], cnt[st]);
    return res;
  }
  auto r1 = mergeRecShapleySparse(st, (st + en) / 2);
  auto r2 = mergeRecShapleySparse((st + en) / 2 + 1, en);
  return r1 * r2;
}


void VotingNonunique::precompMaxPlayers() {
  auto wc = origWeights;
  sort(wc.begin(), wc.end());
  maxPlayers = 1;
  ll cumSum = 0;
  for (int i = 0; i < wc.size(); ++ i) {
    if (!wc[i]) continue;
    cumSum += wc[i];
    if (cumSum > quota - 1) break;
    else maxPlayers ++;
  }
  //maxPlayers = players - 1;
  cout << "maxPlayers: " << maxPlayers << endl;
}
