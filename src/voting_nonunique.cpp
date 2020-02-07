//
// Created by maty on 07/02/20.
//

#include "voting_nonunique.h"

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

vector<double> VotingNonunique::emptyColumn() {
  vector<double> res(quota, -INF);
  res[0] = 0;
  return res;
}

vector<double> VotingNonunique::columnWithOne(ll weight, ll count) {
  vector<double> res(quota, -INF);
  for (int i = 0; i <= count; i ++) {
    if (i*weight >= quota) break;
    res[i*weight] = logChoose(count, i);
  }
  return res;
}

vector<double> VotingNonunique::unmergeColumns(vector<double> c, vector<double> b) {
  vector<double> res(quota);
  for (int i = 0; i < quota; ++ i) {
    res[i] = c[i];
    for (int j = 0; j < i; ++ j) {
      logDec(res[i], res[j] + b[i - j]);
    }
    res[i] -= b[0];
  }
  return res;
}

double VotingNonunique::countSwingsColumn(vector<double> a, vector<double> b, ll weight) {
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

/*vector<double> VotingNonunique::banzhafUnstable() {
  map<ll,double> weightToRes;
  set<ll> dummyWeights = {0}; // weights known to have no effect on the outcome of the game

  vector<double> right = emptyColumn();
  vector<double> left = emptyColumn();
  // prepare left
  for (int i = 0; i < w.size() - 1; ++i) {
    left = mergeColumns(left, columnWithOne(w[i], cnt[i]));
  }
  // get results for all players
  for (int i = w.size() - 1; i >= 0; --i) {
    vector<double> myColumn = columnWithOne(w[i], cnt[i] - 1);
    weightToRes[w[i]] = countSwingsColumn(mergeColumns(right, myColumn), left, w[i]);
    cout << w[i] << ": " << weightToRes[w[i]] << endl;
    if (i > 0) {
      right = mergeColumns(right, columnWithOne(w[i], cnt[i]));
      left = unmergeColumns(left, columnWithOne(w[i - 1], cnt[i - 1]));
    }
    printVec(left);
  }
  vector<double> res(players);
  for (int i = 0; i < players; ++i) {
    res[i] = weightToRes[origWeights[i]];
  }
  normalizeBanzhafLogSums(res);
  //logToNorm(res);
  return res;
}*/

vector<double> VotingNonunique::banzhaf() {
  map<ll,double> weightToRes;
  set<ll> dummyWeights = {0}; // weights known to have no effect on the outcome of the game

  vector<double> right = emptyColumn();
  vector<vector<double>> left(w.size());
  left[0] = emptyColumn();
  for (int i = 1; i < w.size(); ++i) {
    left[i] = addToColumn(left[i - 1], w[i - 1], cnt[i - 1]);
  }
  // prepare left
  /*for (int i = 0; i < w.size() - 1; ++i) {
    left = mergeColumns(left, columnWithOne(w[i], cnt[i]));
  }*/
  // get results for all players
  for (int i = w.size() - 1; i >= 0; --i) {
    //vector<double> myColumn = columnWithOne(w[i], cnt[i] - 1);
    weightToRes[w[i]] = countSwingsColumn(addToColumn(right, w[i], cnt[i] - 1), left[i], w[i]);
    if (i > 0) {
      //right = mergeColumns(right, columnWithOne(w[i], cnt[i]));
      right = addToColumn(right, w[i], cnt[i]);
      //left = unmergeColumns(left, columnWithOne(w[i - 1], cnt[i - 1]));
    }
  }
  vector<double> res(players);
  for (int i = 0; i < players; ++i) {
    res[i] = weightToRes[origWeights[i]];
  }
  normalizeBanzhafLogSums(res);
  //logToNorm(res);
  return res;
}

vector<double> VotingNonunique::fillLeft(int last) {
  vector<double> left = emptyColumn();
  for (int i = 0; i <= last; ++i) {
    //left = mergeColumns(left, columnWithOne(w[i], cnt[i]));
    left = addToColumn(left, w[i], cnt[i]);
  }
  return left;
}

ll VotingNonunique::v(const vector<int> &coalition) {
  int sum = 0;
  for (int i: coalition) sum += origWeights[i];
  return sum >= quota;
}

vector<double> VotingNonunique::mergeColumns(vector<double> a, vector<double> b) {
  vector<double> res(quota, -INF);
  for (int i = 0; i < quota; ++ i) {
    for (int j = 0; j <= i; ++ j) {
      logInc(res[i], a[j] + b[i-j]);
    }
  }
  return res;
}

vector<double> VotingNonunique::addToColumn(const vector<double> &a, double weight, ll count) {
  if (!count) return a;
  vector<double> res = a;
  for (int i = 0; i < quota; ++i) {
    for (int j = 1; j <= count && i - j*weight >= 0; ++j) {
      logInc(res[i], a[i - j*weight] + logChoose(count, j));
    }
  }
  return res;
}

