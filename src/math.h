//
// Created by maty on 07/12/19.
//

#ifndef COAL_GAME_MATH_H
#define COAL_GAME_MATH_H

#define INF (numeric_limits<double>::infinity())

#include <vector>
#include <NTL/ZZX.h>
using namespace NTL;
#define matrix vector<vector<double>>
using namespace std;

void printVec(const vector<double> &a);
void printVec(const vector<int> &a);
void printVec(const vector<long long int> &a);
void printMtx(const vector<vector<int>> &a);

vector<int> random_perm(int);
vector<int> random_subset(int n);

void cutPolynom(ZZX & c, int maxLength);

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

ZZ factorialCached(int n);
ZZ factorialNoCache(int n);

#endif //COAL_GAME_MATH_H
