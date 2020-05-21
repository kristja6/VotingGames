#include <Rcpp.h>
#include "sum_of_voting.h"
#include "RInterface.h"
#include "sum_of_unanimity.h"

using namespace Rcpp;
using namespace std;

vector<vector<int>> matrixToVectorsInt(const NumericMatrix & mtx) {
  vector<vector<int>> m(mtx.nrow(), vector<int>(mtx.ncol()));
  for (int i = 0; i < mtx.nrow(); ++i) {
    for (int j = 0; j < mtx.ncol(); ++j) {
      m[i][j] = mtx[i + j*mtx.nrow()];
    }
  }
  return m;
}

// [[Rcpp::export]]
NumericVector votingBanzhaf(const NumericVector & weights, const int quota, const IntegerVector & players, const String & denom) {
  vector<double> res;
  VotingGame game(vector<int>(weights.begin(), weights.end()), quota);
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingShapley(const IntegerVector & weights, const int quota, const IntegerVector & players) {
  VotingGame game(vector<int>(weights.begin(), weights.end()), quota);
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingBanzhafTop(const NumericVector & weights, const int quota, const int topN, const String & denom) {
  vector<double> res;
  VotingGame game(vector<int>(weights.begin(), weights.end()), quota);
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  res = game.banzhafTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingShapleyTop(const IntegerVector & weights, const int quota, const int topN) {
  VotingGame game(vector<int>(weights.begin(), weights.end()), quota);
  auto res = game.shapleyTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double votingVal(const IntegerVector & weights, const int quota, const IntegerVector & coal) {
  VotingGame game(vector<int>(weights.begin(), weights.end()), quota);
  return game.v(vector<int>(coal.begin(), coal.end()));
}

// [[Rcpp::export]]
NumericVector sumOfVotingBanzhaf(const NumericMatrix & weights, const NumericVector & quotas, const IntegerVector & players) {
  SumOfVoting game(matrixToVectorsInt(weights), vector<int>(quotas.begin(), quotas.end()));
  auto res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector sumOfVotingShapley(const NumericMatrix & weights, const NumericVector & quotas, const IntegerVector & players) {
  SumOfVoting game(matrixToVectorsInt(weights), vector<int>(quotas.begin(), quotas.end()));
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector sumOfVotingBanzhafTop(const NumericMatrix & weights, const NumericVector & quotas, const int topN) {
  SumOfVoting game(matrixToVectorsInt(weights), vector<int>(quotas.begin(), quotas.end()));
  auto res = game.banzhafTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector sumOfVotingShapleyTop(const NumericMatrix & weights, const NumericVector & quotas, const int topN) {
  SumOfVoting game(matrixToVectorsInt(weights), vector<int>(quotas.begin(), quotas.end()));
  auto res = game.shapleyTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double sumOfVotingValue(const NumericMatrix & weights, const NumericVector & quotas, const IntegerVector & coal) {
  SumOfVoting game(matrixToVectorsInt(weights), vector<int>(quotas.begin(), quotas.end()));
  return game.v(vector<int>(coal.begin(), coal.end()));
}

// [[Rcpp::export]]
NumericVector microarrayBanzhaf(const NumericMatrix & mtx) {
  auto m = matrixToVectorsInt(mtx);
  SumOfUnanimity game(m);
  auto res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector microarrayShapley(const NumericMatrix & mtx) {
  SumOfUnanimity game(matrixToVectorsInt(mtx));
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double microarrayValue(const NumericMatrix & mtx, const IntegerVector & coal) {
  SumOfUnanimity game(matrixToVectorsInt(mtx));
  return game.v(vector<int>(coal.begin(), coal.end()));
}

