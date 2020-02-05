#include <string>
#include <Rcpp.h>
#include "voting_game.h"
#include "microarray_game.h"
#include "sum_of_voting.h"
#include "voting_microarray.h"

using namespace Rcpp;
using namespace std;

// helper methods
vector<vector<int>> matrixToVectorsInt(const NumericMatrix & mtx) {
  vector<vector<int>> m(mtx.nrow(), vector<int>(mtx.ncol()));
  for (int i = 0; i < mtx.nrow(); ++i) {
    for (int j = 0; j < mtx.ncol(); ++j) {
      m[i][j] = mtx[i + j*mtx.nrow()];
    }
  }
  return m;
}

vector<vector<double>> matrixToVectorsDouble(const NumericMatrix & mtx) {
  vector<vector<double>> m(mtx.nrow(), vector<double>(mtx.ncol()));
  for (int i = 0; i < mtx.nrow(); ++i) {
    for (int j = 0; j < mtx.ncol(); ++j) {
      m[i][j] = mtx[i + j*mtx.nrow()];
    }
  }
  return m;
}

NumericMatrix vectorsToMatrix(const vector<vector<int>> & mtx) {
  NumericMatrix res(mtx.size(), mtx[0].size());
  for (size_t i= 0; i < mtx.size(); ++i) {
    for (size_t j = 0; j < mtx[i].size(); ++j) {
      res[i + j*res.nrow()] = mtx[i][j];
    }
  }
  return res;
}

// [[Rcpp::export]]
NumericVector votingBanzhaf(const NumericVector & weights, const long long int quota) {
  vector<double> res;
  VotingGame game(vector<long long int>(weights.begin(), weights.end()), quota);
  res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingShapley(const IntegerVector & weights, const long long int quota) {
  VotingGame game(vector<long long int>(weights.begin(), weights.end()), quota);
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double votingVal(const IntegerVector & weights, const long long int quota, const IntegerVector & coal) {
  VotingGame game(vector<long long int>(weights.begin(), weights.end()), quota);
  return game.v(vector<int>(coal.begin(), coal.end()));
}

// [[Rcpp::export]]
NumericVector microarrayBanzhaf(const NumericMatrix & mtx) {
  auto m = matrixToVectorsInt(mtx);
  MicroarrayGame game(m);
  auto res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector microarrayShapley(const NumericMatrix & mtx) {
  MicroarrayGame game(matrixToVectorsInt(mtx));
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double microarrayValue(const NumericMatrix & mtx, const IntegerMatrix & coal) {
  MicroarrayGame game(matrixToVectorsInt(mtx));
  return game.v(vector<int>(coal.begin(), coal.end()));
}

// [[Rcpp::export]]
NumericVector microarrayStrongestCoalition(const NumericMatrix & mtx, const int size) {
  MicroarrayGame game(matrixToVectorsInt(mtx));
  auto res = game.optimalCoalitionOfFixedSize(size);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericMatrix expressionsToFeaturesStd(const NumericMatrix & special, const NumericMatrix & control) {
  auto res = MicroarrayGame::expressionsToFeaturesStd2Groups(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  return vectorsToMatrix(res);
}

// [[Rcpp::export]]
NumericVector votingMicroarrayBanzhaf(const NumericMatrix &special, const NumericMatrix &control) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  auto res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingMicroarrayShapley(const NumericMatrix &special, const NumericMatrix &control) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double votingMicroarrayValue(const NumericMatrix & special, const NumericMatrix & control, const IntegerMatrix & coal) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  return game.v(vector<int>(coal.begin(), coal.end()));
}

// [[Rcpp::export]]
bool votingMicroarrayPredict(const NumericMatrix &special, const NumericMatrix &control, const NumericVector &sample) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  vector<double> expr(sample.begin(), sample.end());
  return game.predict(game.expressionsToCoalition(expr));
}

