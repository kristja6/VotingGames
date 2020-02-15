//
// Created by maty on 07/02/20.
//

#include "voting_nonunique.h"
#include <NTL/RR.h>

VotingNonunique::VotingNonunique(vector<ll> weights, ll quota) : CoalGame(weights.size()), origWeights(weights), quota(quota) {
  sort(weights.begin(), weights.end());
  // decrease unneeded resolution
  for (size_t i = 0; i < weights.size(); ++i) {
    if (weights[i] == 0) continue;
    if (w.empty() || w.back() != weights[i]) {
      w.push_back(weights[i]);
      cnt.push_back(1);
    } else cnt.back() ++;
  }
}

ZZX VotingNonunique::emptyColumn() {
  ZZX res;
  SetCoeff(res, 0, 1);
  return res;
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
  if (en < 0 || st >= (int)w.size()) return emptyColumn();
  if (st == en) {
    ZZX res = columnWithOne(w[st], cnt[st]);
    return res;
  }
  ZZX res = mergeRec(st, (st + en)/2) * mergeRec((st + en)/2 + 1, en);
  cutPolynom(res, quota);

  return res;
}


void VotingNonunique::banzhafRec(int first, int last, ZZX pf) {
  const int mid = (first + last) / 2;
  if (first == last) {
    const int i = first;
    cout << "add " << flush;
    addToColumnInplace(rolling, w[i], cnt[i] - 1);
    cout << "counting " << flush;
    weightToRes[w[i]] = countSwingsColumn(rolling, pf, w[i]);
    sum += weightToRes[w[i]] * cnt[i];
    cout << "add " << endl;
    addToColumnInplace(rolling, w[i], 1);
    return;
  }

  ZZX old = pf;
  pf *= mergeRec(first, mid);
  cutPolynom(pf, quota);

  banzhafRec(mid + 1, last, pf);
  banzhafRec(first, mid, old);
}

vector<double> VotingNonunique::banzhaf() {
  cout << "using fft" << endl;
  cout << "q = " << quota << ", u = " << w.size() << ", n = " << origWeights.size() << endl;
  weightToRes.clear();
  sum = 0;
  rolling = emptyColumn();
  banzhafRec(0, w.size() - 1, emptyColumn());
  // TODO: make a function for normalization
  vector<double> res(players);
  for (int i = 0; i < players; ++i) {
    RR temp = conv<RR>(weightToRes[origWeights[i]]);
    temp /= conv<RR>(sum);
    res[i] = conv<double>(temp);
  }
  return res;
}

vector<double> VotingNonunique::banzhafSlowUnmerge() {
  cout << "using fft" << endl;
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
  cout << "using fft" << endl;
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

