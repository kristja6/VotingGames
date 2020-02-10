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
  /*cout << "gcd stuff" << endl;
  ll g = w[0];
  for (int i = 1; i < w.size(); ++i) {
    g = __gcd(g, w[i]);
  }
  cout << "reducing with gcd: " << g << endl;
  if (g > 1) {
    for (int i = 0; i < w.size(); ++i) w[i] /= g;
    this->quota /= g;
  }
  cout << "constructor done" << endl;*/
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

/*vector<double> VotingNonunique::unmergeColumns(vector<double> c, vector<double> b) {
  vector<double> res(quota);
  for (int i = 0; i < quota; ++ i) {
    res[i] = c[i];
    for (int j = 0; j < i; ++ j) {
      logDec(res[i], res[j] + b[i - j]);
    }
    res[i] -= b[0];
  }
  return res;
}*/

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
  if (en < 0 || st >= w.size()) return emptyColumn();
  if (st == en) {
    ZZX res = columnWithOne(w[st], cnt[st]);
    return res;
  }
  ZZX res = mergeRec(st, (st + en)/2) * mergeRec((st + en)/2 + 1, en);

  if (res.rep.length() > quota) {
    res.SetLength(quota);
    res.normalize();
  }

  return res;
}

vector<double> VotingNonunique::banzhaf() {
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

      right = addToColumn(right, w[i], cnt[i]);
      if (right.rep.length() > quota) {
        right.SetLength(quota);
        right.normalize();
      }

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
    //res[i] = conv<double>(weightToRes[origWeights[i]]);
  }

  //normalizeBanzhafLogSums(res);
  //logToNorm(res);
  return res;
}

/*vector<double> VotingNonunique::fillLeft(int last) {
  vector<double> left = emptyColumn();
  for (int i = 0; i <= last; ++i) {
    //left = mergeColumns(left, columnWithOne(w[i], cnt[i]));
    left = addToColumn(left, w[i], cnt[i]);
  }
  return left;
}*/

ll VotingNonunique::v(const vector<int> &coalition) {
  int sum = 0;
  for (int i: coalition) sum += origWeights[i];
  return sum >= quota;
}

/*vector<double> VotingNonunique::mergeColumns(vector<double> a, vector<double> b) {
  vector<double> res(quota, -INF);
  for (int i = 0; i < quota; ++ i) {
    for (int j = 0; j <= i; ++ j) {
      logInc(res[i], a[j] + b[i-j]);
    }
  }
  return res;
}*/

ZZX VotingNonunique::addToColumn(const ZZX &a, ll weight, ll count) {
  if (!count) return a;
  /*vector<double> res = a;
  for (int i = 0; i < quota; ++i) {
    for (int j = 1; j <= count && i - j*weight >= 0; ++j) {
      logInc(res[i], a[i - j*weight] + logChoose(count, j));
    }
  }*/
  return a * columnWithOne(weight, count);
}

void VotingNonunique::addToColumnInplace(ZZX &a, ll weight, ll count) {
  a *= columnWithOne(weight, count);
}

void VotingNonunique::removeFromColumn(ZZX &a, ll weight, ll count) {
  a = reverse(a, a.rep.length() - 1);
  ZZX t = columnWithOne(weight, count);
  t = reverse(t, t.rep.length() - 1);
  deconvolution(a, t);
  a = reverse(a, a.rep.length() - 1);
}

