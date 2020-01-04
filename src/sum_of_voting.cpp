//
// Created by maty on 04/01/20.
//

#include "sum_of_voting.h"

SumOfVoting::SumOfVoting(const matrix &special, const matrix &control) : CoalGame(special.back().size()) {
  vector<vector<int>> res = vector<vector<int>>(special.size(), vector<int>(special[0].size(), 0));
  const int genes = special.size();
  const int samples = special[0].size();
  vector<double> u(genes), s(genes);
  for (int i = 0; i < samples; ++i) {
    u[i] = mean(control[i]);
    s[i] = sd(control[i], u[i]);
  }
  for (int j = 0; j < samples; ++j) {
    vector<ll> weights(genes);
    int sum = 0;
    for (int i = 0; i < genes; ++i) {
      sum += weights[i] = fabs(special[i][j] - u[i]);
      cout << (int)fabs(special[i][j] - u[i]) << ' ';
    }
    cout << endl;
    cout << "sum: " << sum << endl;
    games.emplace_back(VotingGame(weights, sum/2));
  }
}

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
    cout << i << ' ' << endl;
    auto cur = games[i].banzhafDpFast();
    for (size_t j = 0; j < cur.size(); ++j) {
      res[j] += cur[j];
    }
    cout << endl;
  }
  return res;
}
