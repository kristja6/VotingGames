//
// Created by maty on 07/12/19.
//

#include <assert.h>
#include <algorithm>
#include <numeric>
#include <NTL/ZZ.h>
#include "math.h"

void printVec(const vector<double> &a) {
  for (auto i: a) cout << i << ' ';
  cout << endl;
}

void printVec(const vector<int> &a) {
  for (auto i: a) cout << i << ' ';
  cout << endl;
}

void printVec(const vector<long long int> &a) {
  for (auto i: a) cout << i << ' ';
  cout << endl << endl;
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

void printMtx(const vector<vector<int>> &a) {
  for (size_t i = 0; i < a.size(); ++i) {
    for (size_t j = 0; j < a[i].size(); ++j) {
      cout << a[i][j] << ' ';
    }
    cout << "\n";
  }
  cout << endl << endl;
}

double mean(const vector<double> &a) {
  return accumulate(a.begin(), a.end(), 0.0)/a.size();
}

double sd(const vector<double> &a, double mean) {
  double sq_sum = inner_product(a.begin(), a.end(), a.begin(), 0.0);
  return sqrt(sq_sum / a.size() - mean * mean);
}

double sd(const vector<double> &a) {
  return sd(a, mean(a));
}

// random permutation of {0, 1, ..., n - 1}
vector<int> random_perm(int n) {
  vector<int> res(n);
  for (int i = 0; i < n; ++ i) res[i] = i;
  random_shuffle(res.begin(), res.end());
  return res;
}

// random subset of {0, 1, ..., n - 1}
vector<int> random_subset(int n) {
  vector<int> res;
  for (int i = 0; i < n; ++ i) {
    if (rand()&2) res.push_back(i);
  }
  return res;
}

void cutPolynom(ZZX &c, int maxLength) {
  if (c.rep.length() > maxLength) {
    c.SetLength(maxLength);
    c.normalize();
  }
}

vector<ZZ> factorialCache;
ZZ factorialCached(int n) {
  assert(n >= 0);
  if (factorialCache.empty()) factorialCache.emplace_back(ZZ(1));
  if (n < (int)factorialCache.size()) return factorialCache[n];
  for (int i = factorialCache.size(); i <= n; ++ i) {
    factorialCache.push_back(factorialCache.back() * i);
  }
  return factorialCached(n);
}

ZZ multiplySequence(vector<int> a) {
  if (a.size() == 1) return ZZ(a.back());
  else if (a.size() == 0) return ZZ(1);

  vector<int> b;
  vector<int> c;
  for (int i = 0; i < (int)a.size(); ++i) {
    if (i%2 == 0) b.push_back(a[i]);
    else c.push_back(a[i]);
  }
  return multiplySequence(b) * multiplySequence(c);
}

ZZ factorialNoCache(int n) {
  if (n <= 1) return ZZ(1);
  vector<int> seq(n - 1);
  for (int i = 0; i < (int)seq.size(); ++ i) seq[i] = i+2;
  return multiplySequence(seq);
}

ZZ Polynomial2D::get(int row, int column) const {
  return coeff(data, row*columns + column);
}

void Polynomial2D::set(int row, int column, const ZZ &val) {
  SetCoeff(data, row*columns + column, val);
}
void Polynomial2D::set(int row, int column, int val) {
  SetCoeff(data, row*columns + column, val);
}

void Polynomial2D::print() const {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      cout << get(i, j) << ' ';
    }
    cout << "\n";
  }
  cout << endl;
}

void Polynomial2D::resize(int nrows, int ncolumns) {
  ZZX ndata;
  ndata.SetLength(nrows * ncolumns);
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      ndata[i*ncolumns + j] = get(i, j);
    }
  }
  ndata.normalize();
  rows = nrows;
  columns = ncolumns;
  data = move(ndata);
}

Polynomial2D &Polynomial2D::operator*=(Polynomial2D a) {
  int nrows = a.rows + rows - 1;
  int ncolumns = a.columns + columns - 1;
  resize(nrows, ncolumns);
  a.resize(nrows, ncolumns);
  data *= a.data;
  return *this;
}

void Polynomial2D::cutRows(int r) {
  if (rows > r) {
    rows = r;
    cutPolynom(data, rows * columns);
  }
}

Polynomial2D Polynomial2D::operator*(const Polynomial2D & a) {
  Polynomial2D res = a;
  res *= *this;
  return res;
}

void Polynomial2D::shrink(int nr, int nc) {
  if (nc < columns) {
    ZZX ndata;
    ndata.SetMaxLength(nr*nc);
    for (int i = 0; i < nr; ++i) {
      for (int j = 0; j < nc; ++j) {
        SetCoeff(ndata, i * nc + j, coeff(data, i * columns + j));
      }
    }
    rows = nr;
    columns = nc;
    data = move(ndata);
  } else if (nr < rows) {
    cutRows(nr);
  }
}

void Polynomial2D::efficientMul(Polynomial2D &a) {
  int nrows = a.rows + rows - 1;
  int ncolumns = a.columns + columns - 1;
  resize(nrows, ncolumns);
  a.resize(nrows, ncolumns);
  data *= a.data;
}
