//
// Created by maty on 04/01/20.
//

#include "sum_of_voting.h"
#include "math.h"

ll SumOfVoting::v(const vector<int> &coal) {
  ll res = 0;
  for (size_t i = 0; i < games.size(); ++i) {
    res += games[i].v(coal);
  }
  return res;
}

vector<double> SumOfVoting::banzhaf() {
  vector<double> res(players, 0);
  for (size_t i = 0; i < games.size(); ++i) {
    cout << i << ' ' << flush;
    auto cur = games[i].banzhaf();
    for (size_t j = 0; j < cur.size(); ++j) {
      res[j] += cur[j];
    }
  }
  cout << endl;
  return res;
}

vector<double> SumOfVoting::shapley() {
  vector<double> res(players, 0);
  for (size_t i = 0; i < games.size(); ++i) {
    cout << i << ' ' << flush; // TODO: probably remove in final version
    auto cur = games[i].shapley();
    for (size_t j = 0; j < cur.size(); ++j) {
      res[j] += cur[j];
    }
  }
  cout << endl;
  return res;
}

SumOfVoting::SumOfVoting(const vector<vector<ll>> &weights, const vector<ll> &quotas) : CoalGame(weights[0].size()) {
  assert(weights.size() == quotas.size());
  for (size_t i = 0; i < weights.size(); ++i) {
    games.push_back(VotingGame(weights[i], quotas[i]));
  }
}

