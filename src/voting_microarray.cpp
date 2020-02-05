//
// Created by maty on 05/02/20.
//

#include "voting_microarray.h"

VotingMicroarray::VotingMicroarray(const matrix &special, const matrix &control) : SumOfVoting(special.size())
    , special(special), control(control) {
  const int genes = special.size();
  const int samples = special[0].size();

  avgSpecial = vector<double>(genes);
  avgControl = vector<double>(genes);

  vector<double> s(genes);
  for (int i = 0; i < genes; ++i) {
    avgControl[i] = mean(control[i]);
    avgSpecial[i] = mean(special[i]);
    s[i] = sd(control[i], avgControl[i]);
  }
  for (int j = 0; j < samples; ++j) {
    vector<ll> weights(genes);
    int sum = 0;
    for (int i = 0; i < genes; ++i) {
      sum += weights[i] = round(fabs(special[i][j] - avgControl[i]));
    }
    if (sum) {
      VotingGame temp(weights, sum/2); // works quite nicely
      //VotingGame temp(weights, (sum*5)/4); // seems ok as well
      games.push_back(temp);
    }
  }

  avgSpecialVal = avgMatrixVal(special);
  avgControlVal = avgMatrixVal(control);
}

vector<int> VotingMicroarray::expressionsToCoalition(const vector<double> &expressions) {
  vector<int> res;
  for (int i = 0; i < players; ++i) {
    if (abs(expressions[i] - avgSpecial[i]) < abs(expressions[i] - avgControl[i])) {
      res.push_back(i);
    }
  }
  return res;
}

double VotingMicroarray::avgMatrixVal(const matrix &mtx) {
  double avg = 0;
  // special
  for (int i = 0; i < mtx[0].size(); ++i) {
    vector<double> curExp(players);
    for (int j = 0; j < players; ++j) {
      curExp[j] = mtx[j][i];
    }
    avg += v(expressionsToCoalition(curExp));
  }
  return avg / mtx[0].size();
}

bool VotingMicroarray::predict(const vector<int> &coal) {
  double val = v(coal);
  cout << "value: " << val << ' ' << ", avg special: " << avgSpecialVal << ' ' << "avg control: " << avgControlVal << endl;
  return abs(val - avgSpecialVal) < abs(val - avgControlVal);
}
