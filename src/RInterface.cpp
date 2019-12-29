#include <string>
#include <Rcpp.h>
#include "voting_game.h"
#include "microarray_game.h"

using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
NumericVector votingBanzhaf(const NumericVector & weights, const long long int quota) {
  vector<double> res;
  VotingGame game(vector<long long int>(weights.begin(), weights.end()), quota);
  res = game.banzhafDpFast();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingShapley(const IntegerVector & weights, const long long int quota) {
  VotingGame game(vector<long long int>(weights.begin(), weights.end()), quota);
  auto res = game.shapleyDpFast();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector microarrayBanzhaf(const NumericMatrix & mtx) {
  vector<vector<int>> m(mtx.nrow(), vector<int>(mtx.ncol()));
  for (int i = 0; i < mtx.nrow(); ++i) {
    for (int j = 0; j < mtx.ncol(); ++j) {
      m[i][j] = mtx[i + j*mtx.nrow()];
    }
  }
  MicroarrayGame game(m);
  printMtx(m);
  auto res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector microarrayShapley(const NumericMatrix & mtx) {
  vector<vector<int>> m(mtx.nrow(), vector<int>(mtx.ncol()));
  for (int i = 0; i < mtx.nrow(); ++i) {
    for (int j = 0; j < mtx.ncol(); ++j) {
      m[i][j] = mtx[i + j*mtx.nrow()];
    }
  }
  MicroarrayGame game(m);
  printMtx(m);
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}
