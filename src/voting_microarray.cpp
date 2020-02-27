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
    /*avgControl[i] = mean(control[i]);
    avgSpecial[i] = mean(special[i]);*/
    avgControl[i] = median(this->control[i]);
    avgSpecial[i] = median(this->special[i]);
    //s[i] = sd(control[i], avgControl[i]);
  }
  //cout << "fitting" << endl;
  for (int j = 0; j < samples; ++j) {
    //cout << "game number " << j << endl;
    vector<int> weights(genes);
    int sum = 0;
    for (int i = 0; i < genes; ++i) {
      //sum += weights[i] = round(fabs(special[i][j] - avgControl[i]));
      // TODO: try different methods to choose weights
      int t = round(fabs(special[i][j] - avgControl[i]));
      //if (t <= 2) t = 0;
      sum += weights[i] = t;
    }
    /*if (sum) {
      //VotingNonunique temp(weights, sum/2); // works quite nicely
      VotingNonunique temp(weights, sum/2); // seems ok as well
      gamesNonunique.push_back(temp);
    }*/
    // pick optimal sum
    // true if special
    vector<pair<double, bool>> sumsOfWeights(special[0].size() + control[0].size());
    for (size_t i = 0; i < special[0].size(); ++i) {
      //cout << i << ' ' << flush;
      double sum = 0;
      vector<double> expressions(genes);
      for (int k = 0; k < genes; ++k) expressions[k] = special[k][i];
      vector<int> coal = expressionsToCoalition(expressions);
      for (size_t k = 0; k < coal.size(); ++k) sum += weights[coal[k]];
      sumsOfWeights[i] = {sum, true};
    }
    for (size_t i = 0; i < control[0].size(); ++i) {
      //cout << i << ' ' << flush;
      double sum = 0;
      vector<double> expressions(genes);
      for (int k = 0; k < genes; ++k) expressions[k] = control[k][i];
      vector<int> coal = expressionsToCoalition(expressions);
      for (size_t k = 0; k < coal.size(); ++k) sum += weights[coal[k]];
      sumsOfWeights[i + special[0].size()] = {sum, false};
    }
    //cout << endl;
    sort(sumsOfWeights.begin(), sumsOfWeights.end());


    // TODO: make this more rigorous (normalize by number of control/special, find the actual optimal interval
    int wrong = control[0].size();
    int bestWrong = wrong;
    int firstIdx = 0;
    int lastIdx = sumsOfWeights.size() - 1;
    for (size_t i = 0; i < sumsOfWeights.size(); ++i) {
      //cout << fixed << "(" << sumsOfWeights[i].first << ", " << sumsOfWeights[i].second << ", " << wrong << ") " << endl;
      if (!sumsOfWeights[i].second) wrong --;
      else wrong ++;
      if (wrong < bestWrong) {
        bestWrong = wrong;
        firstIdx = i;
      }
      if (wrong <= bestWrong) {
        lastIdx = i;
      }
    }
    int quota = (sumsOfWeights[firstIdx].first + sumsOfWeights[min(lastIdx + 1, (int)sumsOfWeights.size() - 1)].first)/2;
    vector<double> sortedWeights(sumsOfWeights.size());
    for (size_t i = 0; i < sortedWeights.size(); ++i) {
      sortedWeights[i] = sumsOfWeights[i].first;
    }
    //quota = sumsOfWeights[sumsOfWeights.size()/2].first; // use weighted median (same number of samples from both classes)
    quota = median(sortedWeights); // use weighted median (same number of samples from both classes)
    //cout << "quota: " << quota << ", quota/sum: " << double(quota)/sum << endl;
    //cout << endl' ' << coal.size() << ;
    //cout << endl;


    VotingNonunique temp(weights, quota); // seems ok as well
    gamesNonunique.push_back(temp);
    gamesUnique.push_back(VotingGame(weights, quota));
  }

  avgSpecialVal = avgMatrixVal(special);
  avgControlVal = avgMatrixVal(control);
}

vector<int> VotingMicroarray::expressionsToCoalition(const vector<double> &expressions) {
  vector<int> res;
  for (int i = 0; i < players; ++i) {
    if (fabs(expressions[i] - avgSpecial[i]) < fabs(expressions[i] - avgControl[i])) {
      res.push_back(i);
    }
  }
  return res;
}

// k nearest neighbors
/*vector<int> VotingMicroarray::expressionsToCoalition(const vector<double> &expressions) {
  vector<int> res;
  for (int i = 0; i < players; ++i) {
    // classify gene as normal or abnormal (in coalition)
    // if (abs(expressions[i] - avgControl[i]) > 4) res.push_back(i);
    vector<pair<double, bool>> labeledExp;
    for (int j = 0; j < special[0].size(); ++j) labeledExp.push_back({fabs(expressions[i] - special[i][j]), true});
    for (int j = 0; j < special[0].size(); ++j) labeledExp.push_back({fabs(expressions[i] - control[i][j]), false});
    sort(labeledExp.begin(), labeledExp.end());
    bool isSpec = true;
    for (int j = 0; j < special[0].size()/2; ++j) {
      if (!labeledExp[j].second) isSpec = false;
    }
    if (isSpec) res.push_back(i);
  }
  return res;
}*/

double VotingMicroarray::avgMatrixVal(const matrix &mtx) {
  // special
  vector<double> vals;
  for (size_t i = 0; i < mtx[0].size(); ++i) {
    vector<double> curExp(players);
    for (int j = 0; j < players; ++j) {
      curExp[j] = mtx[j][i];
    }
    //avg += v(expressionsToCoalition(curExp));
    vals.push_back(v(expressionsToCoalition(curExp)));
  }
  return median(vals);
  //return avg / mtx[0].size();
}

bool VotingMicroarray::predict(const vector<int> &coal) {
  double val = v(coal);
  return abs(val - avgSpecialVal) < abs(val - avgControlVal);
}
