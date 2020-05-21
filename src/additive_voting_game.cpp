//
// Created by maty on 04/01/20.
//

#include <assert.h>
#include <algorithm>
#include "additive_voting_game.h"
#include "math.h"

AdditiveVotingGame::AdditiveVotingGame(const vector<vector<int>> &weights, const vector<int> &quotas) : CoalitionalGame(weights[0].size()) {
  assert(weights.size() == quotas.size());
  for (size_t i = 0; i < weights.size(); ++i) {
    gamesNonunique.push_back(VotingGame(weights[i], quotas[i]));
  }
  players = weights[0].size();
}

double AdditiveVotingGame::v(const vector<int> &coal) {
  double res = 0;
  for (size_t i = 0; i < gamesNonunique.size(); ++i) {
    res += gamesNonunique[i].v(coal);
  }
  return res;
}

vector<double> AdditiveVotingGame::banzhaf() {
  setBanzhafDenominator(BANZHAF_DENOM_SUBSETS);
  vector<double> res(players, 0);
  for (size_t i = 0; i < gamesNonunique.size(); ++i) {
    cerr << (double)(10000*i / gamesNonunique.size()) / 100.0 << "% " << flush;
    auto cur = gamesNonunique[i].banzhaf();
    for (size_t j = 0; j < cur.size(); ++j) {
      res[j] += cur[j];
    }
  }
  cerr << endl;
  return res;
}

vector<double> AdditiveVotingGame::shapley() {
  vector<double> res(players, 0);
  for (size_t i = 0; i < gamesNonunique.size(); ++i) {
    cerr << (double)(10000*i / gamesNonunique.size()) / 100.0 << "% " << flush;
    auto cur = gamesNonunique[i].shapley();
    for (size_t j = 0; j < cur.size(); ++j) {
      res[j] += cur[j];
    }
  }
  cerr << endl;
  return res;
}

double AdditiveVotingGame::banzhaf(int player) {
  double res = 0;
  for (size_t i = 0; i < gamesNonunique.size(); ++i) {
    cerr << (double)(10000*i / gamesNonunique.size()) / 100.0 << "% " << flush;
    res += gamesNonunique[i].banzhaf(vector<int>(1, player))[0];
  }
  cerr << endl;
  return res;
}

double AdditiveVotingGame::shapley(int player) {
  double res = 0;
  for (size_t i = 0; i < gamesNonunique.size(); ++i) {
    cerr << (double)(10000*i / gamesNonunique.size()) / 100.0 << "% " << flush;
    res += gamesNonunique[i].shapley(vector<int>(1, player))[0];
  }
  cerr << endl;
  return res;
}

vector<double> AdditiveVotingGame::shapleyTop(int topN) {
  auto pl = getTopPlayers(getWeights(), topN);
  vector<double> res(players, -1);
  for (auto i: pl) res[i] = 0;
  for (size_t i = 0; i < gamesNonunique.size(); ++i) {
    auto cur = gamesNonunique[i].shapley(pl);
    for (auto j: pl) {
      res[j] += cur[j];
    }
  }
  return res;
}

vector<double> AdditiveVotingGame::banzhafTop(int topN) {
  auto pl = getTopPlayers(getWeights(), topN);
  vector<double> res(players, -1);
  for (auto i: pl) res[i] = 0;
  for (size_t i = 0; i < gamesNonunique.size(); ++i) {
    auto cur = gamesNonunique[i].banzhaf(pl);
    for (auto j: pl) {
      res[j] += cur[j];
    }
  }
  return res;
}

vector<int> AdditiveVotingGame::getTopPlayers(const vector<vector<int>> &weights, int numberOfTopPlayers) {
  vector<PlayerWeights> pl;
  for (size_t i = 0; i < weights.size(); ++i) {
    pl.push_back(PlayerWeights(weights[i], i));
  }
  sort(pl.begin(), pl.end());
  vector<int> res;
  for (size_t i = 0; i < pl.size(); ++i) {
    if ((int)i >= numberOfTopPlayers && i > 0 && pl[i-1] < pl[i]) break;
    res.push_back(pl[i].idx);
  }
  return res;
}

vector<vector<int>> AdditiveVotingGame::getWeights() {
  vector<vector<int>> weights(players, vector<int>(gamesNonunique.size()));
  for (int i = 0; i < players; ++i) {
    for (size_t j = 0; j < gamesNonunique.size(); ++j) {
      weights[i][j] = gamesNonunique[j].getWeights()[i];
    }
  }
  return weights;
}

void AdditiveVotingGame::setBanzhafDenominator(int denom) {
  banzhafDenominator = denom;
  for (size_t i = 0; i < gamesNonunique.size(); ++ i) {
    gamesNonunique[i].setBanzhafDenominator(denom);
  }
}

PlayerWeights::PlayerWeights(const vector<int> &weights, int idx) : weights(weights), idx(idx) {}

bool PlayerWeights::operator<(const PlayerWeights &o) const {
  bool strict = false;
  for (size_t i = 0; i < weights.size(); ++i) {
    if (weights[i] < o.weights[i]) return false;
    else if (weights[i] > o.weights[i]) strict = true;
  }
  return strict;
}
