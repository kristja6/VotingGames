//
// Created by maty on 04/01/20.
//

#include "sum_of_voting.h"

SumOfVoting::SumOfVoting(const matrix &special, const matrix &control) : CoalGame(special.size()) {
  const int genes = special.size();
  const int samples = special[0].size();
  vector<double> u(genes), s(genes);
  for (int i = 0; i < genes; ++i) {
    u[i] = mean(control[i]);
    s[i] = sd(control[i], u[i]);
  }
  for (int j = 0; j < samples; ++j) {
    vector<ll> weights(genes);
    int sum = 0;
    for (int i = 0; i < genes; ++i) {
      sum += weights[i] = round(fabs(special[i][j] - u[i]));
    }
    if (sum) {
      //VotingGame temp(weights, sum/2); // works quite nicely
      VotingGame temp(weights, (sum*5)/4);
      games.push_back(temp);
    }
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
    cout << i << ' ' << flush;
    auto cur = games[i].banzhafDpFast();
    //auto cur = vector<double>(players, 0);
    //printVec(cur);
    for (size_t j = 0; j < cur.size(); ++j) {
      res[j] += cur[j];
    }
  }
  cout << endl;
  return res;
}
