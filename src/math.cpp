//
// Created by maty on 07/12/19.
//

#include "math.h"

double logAdd(double u, double v) {
  if (u < v) swap(u, v);
  if (u == -INF && v == -INF) return -INF;
  else if (u == INF && v == INF) return INF;
  //else return max(u, v) + log(exp(u - max(u, v)) + exp(v - max(u, v)));
  else return u + log1p(exp(v - u));
}

double logSub(double u, double v) {
  if (v == -INF) return u;
  else if (u == -INF) return -INF;
  else if (u == INF && v == INF) return INF;
  else if (v >= u) return -INF;
  else return u + log1p(-exp(v - u));
}

pair<vector<double>, double> logToPaddedNorm(const vector<double> &a) {
  const double top = 0;
  vector<double> res = a;
  double mx = -INF;
  for (auto & i: res) mx = max(i, mx);
  //for (auto & i: res) i += (top - mx);
  for (auto & i: res) {
    i = exp(i + (top - mx));
  }
  return {res, mx - top};
}

pair<matrix, double> logToPaddedNorm(const matrix &a) {
  const double top = 0;
  matrix res = a;
  double mx = -INF;
  for (auto & i: res) for (auto & j: i) mx = max(j, mx);
  //for (auto & i: res) i += (top - mx);
  for (auto & i: res) for (auto & j: i) j = exp(j + (top - mx));
  return {res, mx - top};
}

void logToNorm(vector<double> &a) {
  for (double & i: a) i = exp(i);
}

vector<double> logFactCache;
double logFact(int n) {
  if ((int)logFactCache.size() <= n) {
    // precompute needed values
    if (logFactCache.empty()) logFactCache.push_back(0);
    for (int i = logFactCache.size(); i <= n; ++ i) {
      logFactCache.push_back(logFactCache.back() + log(i));
    }
  }
  return logFactCache[n];
}



void normToLog(vector<double> &a) {
  for (auto & i: a) i = log(max(0.0, i));
}

void printVec(const vector<double> &a) {
  for (auto i: a) cout << i << ' ';
  cout << endl << endl;
}

void normalize(vector<double> &a, vector<double> &b) {
  logToNorm(a);
  logToNorm(b);
}

vector<int> inversePermutation(const vector<int> &p) {
  vector<int> res(p.size());
  for (size_t i = 0; i < p.size(); ++i) {
    res[p[i]] = i;
  }
  return res;
}

void printMtx(const matrix &a) {
  for (size_t i = 0; i < a.size(); ++i) {
    for (size_t j = 0; j < a.back().size(); ++j) {
      cout << a[i][j] << ' ';
    }
    cout << endl;
  }
  cout << endl;
  cout << endl;
}

double logInc(double &a, double b) {
  return a = logAdd(a, b);
}

double logDec(double &a, double b) {
  return a = logSub(a, b);
}

vector<double> logToNormRes(const vector<double> &a) {
  vector<double> res(a.size());
  for (size_t i = 0; i < a.size(); ++i) {
    res[i] = exp(a[i]);
  }
  return res;
}

matrix logToNormRes(const matrix &a) {
  matrix res = a;
  for (size_t i = 0; i < a.size(); ++i) {
    for (size_t j = 0; j < a[i].size(); ++j) {
      res[i][j] = exp(res[i][j]);
    }
  }
  return res;
}

double logChoose(int n, int k) {
  if (n == 0) return -INF;
  return logFact(n) - logFact(n - k) - logFact(k);
}

void printMtx(const vector<vector<int>> &a) {
  for (int i = 0; i < a.size(); ++i) {
    for (int j = 0; j < a[i].size(); ++j) {
      cout << a[i][j] << ' ';
    }
    cout << "\n";
  }
  cout << endl << endl;
}

LogSumMatrix::LogSumMatrix(size_t n, size_t m) : n(n), m(m), dif(vector<matrix>(2, matrix(n, vector<double>(m, -INF)))) {
}

void LogSumMatrix::addSubmatrix(size_t left, size_t right, size_t top, size_t bot, double val) {
  left = max(left, size_t(0));
  top = max(top, size_t(0));
  /*right = min(right, m-1);
  bot = min(bot, n-1);*/

  logInc(dif[0][left][top], val);
  if (bot + 1 < n) logInc(dif[1][left][bot + 1], val);

  if(right + 1 < m) {
    logInc(dif[1][right + 1][top], val);
    if (bot + 1 < n) logInc(dif[0][right + 1][bot + 1], val);
  }
}

matrix LogSumMatrix::getLogResInplace() {
  // first spread along the columns
  for (size_t i = 1; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      logInc(dif[0][i][j], dif[0][i-1][j]);
      logDec(dif[0][i][j], dif[1][i][j]);
    }
  }

  // now spread along the rows
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 1; j < m; ++j) {
      logInc(dif[0][i][j], dif[0][i][j-1]);
      logDec(dif[0][i][j], dif[1][i][j]);
    }
  }
  return dif[0];
}
