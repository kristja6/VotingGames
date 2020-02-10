//
// Created by maty on 07/12/19.
//

#ifndef COAL_GAME_MATH_H
#define COAL_GAME_MATH_H

#define INF (numeric_limits<double>::infinity())

#include <bits/stdc++.h>
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


#endif //COAL_GAME_MATH_H
