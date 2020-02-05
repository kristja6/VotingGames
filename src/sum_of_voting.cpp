//
// Created by maty on 04/01/20.
//

#include "sum_of_voting.h"

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
    cout << i << ' ' << flush;
    auto cur = games[i].shapley();
    for (size_t j = 0; j < cur.size(); ++j) {
      res[j] += cur[j];
    }
  }
  cout << endl;
  return res;
}

