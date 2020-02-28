#include <Rcpp.h>
#include "voting_game.h"
#include "microarray_game.h"
#include "sum_of_voting.h"
#include "voting_microarray.h"
#include "RInterface.h"

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

vector<vector<int>> matrixToVectorsLong(const NumericMatrix & mtx) {
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
NumericVector votingBanzhaf(const NumericVector & weights, const int quota, const IntegerVector & players, const String & denom) {
  vector<double> res;
  VotingNonunique game(vector<int>(weights.begin(), weights.end()), quota);
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingShapley(const IntegerVector & weights, const int quota, const IntegerVector & players) {
  VotingNonunique game(vector<int>(weights.begin(), weights.end()), quota);
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingBanzhafTop(const NumericVector & weights, const int quota, const int topN, const String & denom) {
  vector<double> res;
  VotingNonunique game(vector<int>(weights.begin(), weights.end()), quota);
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  res = game.banzhafTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingShapleyTop(const IntegerVector & weights, const int quota, const int topN) {
  VotingNonunique game(vector<int>(weights.begin(), weights.end()), quota);
  auto res = game.shapleyTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double votingVal(const IntegerVector & weights, const int quota, const IntegerVector & coal) {
  VotingNonunique game(vector<int>(weights.begin(), weights.end()), quota);
  return game.v(vector<int>(coal.begin(), coal.end()));
}

// [[Rcpp::export]]
NumericVector microarrayBanzhaf(const NumericMatrix & mtx, const IntegerVector & players, const String & denom) {
  auto m = matrixToVectorsInt(mtx);
  MicroarrayGame game(m);
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  auto res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector microarrayShapley(const NumericMatrix & mtx, const IntegerVector & players) {
  MicroarrayGame game(matrixToVectorsInt(mtx));
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double microarrayValue(const NumericMatrix & mtx, const IntegerVector & coal) {
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
NumericVector votingMicroarrayBanzhaf(const NumericMatrix &special, const NumericMatrix &control, const IntegerVector & players, const String & denom) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  auto res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingMicroarrayBanzhafTop(const NumericMatrix &special, const NumericMatrix &control, int topN, const String & denom) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  auto res = game.banzhafTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingMicroarrayShapley(const NumericMatrix &special, const NumericMatrix &control, const IntegerVector & players) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector votingMicroarrayShapleyTop(const NumericMatrix &special, const NumericMatrix &control, int topN) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  auto res = game.shapleyTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double votingMicroarrayValue(const NumericMatrix & special, const NumericMatrix & control, const IntegerVector & coal) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  return game.v(vector<int>(coal.begin(), coal.end()));
}

// [[Rcpp::export]]
bool votingMicroarrayPredict(const NumericMatrix &special, const NumericMatrix &control, const NumericVector &sample) {
  VotingMicroarray game(matrixToVectorsDouble(special), matrixToVectorsDouble(control));
  vector<double> expr(sample.begin(), sample.end());
  return game.predict(game.expressionsToCoalition(expr));
}

// [[Rcpp::export]]
NumericVector sumOfVotingBanzhaf(const NumericMatrix & weights, const NumericVector & quotas, const IntegerVector & players, const String & denom) {
  SumOfVoting game(matrixToVectorsLong(weights), vector<int>(quotas.begin(), quotas.end()));
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  auto res = game.banzhaf();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector sumOfVotingShapley(const NumericMatrix & weights, const NumericVector & quotas, const IntegerVector & players) {
  SumOfVoting game(matrixToVectorsLong(weights), vector<int>(quotas.begin(), quotas.end()));
  auto res = game.shapley();
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector sumOfVotingBanzhafTop(const NumericMatrix & weights, const NumericVector & quotas, const int topN, const String & denom) {
  SumOfVoting game(matrixToVectorsLong(weights), vector<int>(quotas.begin(), quotas.end()));
  if (denom == SUBSETS_NAME) game.setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  auto res = game.banzhafTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
NumericVector sumOfVotingShapleyTop(const NumericMatrix & weights, const NumericVector & quotas, const int topN) {
  SumOfVoting game(matrixToVectorsLong(weights), vector<int>(quotas.begin(), quotas.end()));
  auto res = game.shapleyTop(topN);
  return NumericVector(res.begin(), res.end());
}

// [[Rcpp::export]]
double sumOfVotingValue(const NumericMatrix & weights, const NumericVector & quotas, const IntegerVector & coal) {
  SumOfVoting game(matrixToVectorsLong(weights), vector<int>(quotas.begin(), quotas.end()));
  return game.v(vector<int>(coal.begin(), coal.end()));
}

