//
// Created by maty on 07/12/19.
//

#ifndef COAL_GAME_MATH_H
#define COAL_GAME_MATH_H

#define INF (numeric_limits<double>::infinity())

#include <bits/stdc++.h>
#include <NTL/ZZX.h>
using namespace NTL;
#define EPS (1e-7)
#define matrix vector<vector<double>>
#define in_range(n, l, r) (n >= l && n <= r)
using namespace std;

double logAdd(double, double);
double logSub(double, double);
double logInc(double & a, double b);
double logDec(double & a, double b);
pair<double, double> logAdd(pair<double, double>, pair<double, double>);
pair<double, double> logInc(pair<double, double> & a, pair<double, double> b);
double extendedLogToNorm(pair<double, double> a);

pair<vector<double>, double> logToPaddedNorm(const vector<double> &a);
pair<matrix, double> logToPaddedNorm(const matrix &a);
vector<double> & logToNorm(vector<double> &a);
vector<double> logToNormRes(const vector<double> & a);
matrix logToNormRes(const matrix & a);
void normToLog(vector<double> &a);
double logFact(int);
double logChoose(int n, int k);

void printVec(const vector<double> &a);
void printVec(const vector<int> &a);
void printVec(const vector<long long int> &a);
void printMtx(const matrix &a);
void printMtx(const vector<vector<int>> &a);
void normalize(vector<double> &a, vector<double> &b);
vector<int> inversePermutation(const vector<int> & p);
double mean(const vector<double> & a);
double sd(const vector<double> & a, double mean);
double sd(const vector<double> & a);
double median(vector<double> & a);

vector<int> random_perm(int);
vector<int> random_subset(int n);

class LogSumMatrix {
  size_t n, m;
  vector<matrix> dif; // differences along columns
  // 0 is for positive, 1 for negative
public:
  LogSumMatrix(size_t n, size_t m);

  void addSubmatrix(size_t left, size_t right, size_t top, size_t bot, double val);
  // consolidate all the additions and get the actual matrix
  matrix getLogResInplace();
};

void deconvolution(ZZX & c, const ZZX & b);
void cutPolynom(ZZX & c, int maxLength);

struct LogNum {
  LogNum(): v(-INF) {}
  LogNum(double v, bool isLog = false): v(isLog ? v : log(v)) {}
  double v;

  inline LogNum operator * (const LogNum & a) const {
    return {v + a.v, true};
  }
  inline LogNum operator / (const LogNum & a) const {
    return LogNum(v - a.v, true);
  }
  inline LogNum operator + (const LogNum & a) const {
    return LogNum(logAdd(v, a.v), true);
  }
  inline LogNum operator - (const LogNum & a) const {
    return LogNum(logSub(v, a.v), true);
  }
  inline LogNum & operator += (const LogNum & a) {
    logInc(v, a.v);
    return *this;
  }
  inline LogNum & operator -= (const LogNum & a) {
    logDec(v, a.v);
    return *this;
  }
  inline LogNum & operator *= (const LogNum & a) {
    v += a.v;
    return *this;
  }
  inline LogNum & operator /= (const LogNum & a) {
    v -= a.v;
    return *this;
  }
  static LogNum nChooseK(int n, int k) {
    return logChoose(n, k);
  }
  inline bool operator < (const LogNum & other) const {
    return v < other.v;
  }
  inline bool operator == (const LogNum & other) const {
    return v == other.v;
  }
  inline bool operator > (const LogNum & other) const {
    return v > other.v;
  }
  double norm() const { return exp(v); }
  friend ostream & operator << (ostream &out, const LogNum & a);
  double val() const { return v; }
};

struct ExtLogNum {
  ExtLogNum(): pos(-INF), neg(-INF) {}
  ExtLogNum(double v, bool isLog = false) {
    if (v >= 0) pos = log(v);
    else neg = log(-v);
  }
  inline ExtLogNum & operator *= (double a) {
    if (a < 0) {
      swap(pos, neg);
      a = abs(a);
    }
    pos *= a, neg *= a;
    return *this;
  }
  inline ExtLogNum & operator /= (double a) {
    if (a < 0) {
      swap(pos, neg);
      a = abs(a);
    }
    pos /= a, neg /= a;
    return *this;
  }
  inline ExtLogNum & operator /= (const LogNum & a) {
    pos /= a, neg /= a;
    return *this;
  }
  inline ExtLogNum & operator += (const ExtLogNum & a) {
    pos += a.pos;
    neg += a.neg;
    return *this;
  }
  double norm() const {
    LogNum val = max(pos, neg) - min(pos, neg);
    return exp(neg > pos ? -val.val() : val.val());
  }
  LogNum pos, neg;
};

vector<double> toNormal(const vector<LogNum> & a);

ostream & operator << (ostream &out, const LogNum & a);

struct Polynomial2D {
  Polynomial2D(int rows, int columns): rows(rows), columns(columns) {
    data.SetMaxLength(rows * columns);
  }
  int rows, columns;
  ZZX data;
  Polynomial2D & operator *= (Polynomial2D a);
  Polynomial2D operator * (const Polynomial2D & a);
  ZZ get(int row, int column) const;
  void set(int row, int column, const ZZ & val);
  void set(int row, int column, int val);
  void print() const;
  void cutRows(int r);
  void shrink(int nr, int nc);
  void efficientMul(Polynomial2D & a);
private:
  void resize(int nrows, int ncolumns);
};

ZZ factorial(int n);

void printVec(const vector<LogNum> &a);

using IntPair = std::pair<int, int>;

struct IntPairHash {
  static_assert(sizeof(int) * 2 == sizeof(size_t));

  size_t operator()(IntPair p) const noexcept {
    return size_t(p.first) << 32 | p.second;
  }
};

struct SparsePolynomial2D {
  int maxRows, maxColumns;

  SparsePolynomial2D(int maxRows, int maxColumns) : maxRows(maxRows), maxColumns(maxColumns) {}

  SparsePolynomial2D operator * (const SparsePolynomial2D & o) const;

  SparsePolynomial2D & operator *= (const SparsePolynomial2D & o);

  unordered_map<pair<int,int>, ZZ, IntPairHash> data;
};

ZZ nChooseK(int a, int b);

#endif //COAL_GAME_MATH_H
