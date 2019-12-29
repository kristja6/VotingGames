#include <bits/stdc++.h>
#include "coal_game.h"
#include "voting_game.h"
#include "math.h"
#include "microarray_game.h"

void example1() {
  cout << "Monte carlo" << endl;
  const vector<ll> w = vector<ll>{4, 8, 20, 32, 44};
  CoalGame<int> game(5,  [w] (const vector<int> & players) {
    int sum = 0;
    for (int player : players) {
      sum += w[player];
    }
    if (sum >= 54) return 1;
    else return 0;
  });
  //vector<double> res = game.shapleyMonteCarlo(100000);
  vector<double> res = game.banzhafMonteCarlo(100000);
  for (double r: res) cout << r << ' ';
  cout << endl;
}

void example2() {
  cout << "Slow DP" << endl;
  VotingGame game(5, vector<ll>{4, 8, 20, 32, 44}, 54);
  vector<double> res = game.banzhafDpSlow();
  for (double r: res) cout << r << ' ';
  cout << endl;
}

void example3() {
  cout << "BB" << endl;
  VotingGame game(5, vector<ll>{4, 8, 20, 32, 44}, 54);
  vector<double> res = game.banzhafBranchAndBound();
  for (double r: res) cout << r << ' ';
  cout << endl;
}

/*void example4() {
  cout << "DP with slow merge" << endl;
  VotingGame game(5, vector<ll>{4, 8, 20, 32, 44}, 54);
  vector<double> res = game.banzhafDpFastSlowMerge();
  for (double r: res) cout << r << ' ';
  cout << endl;
}*/

/*void example3() {
  CoalGame<int,int> game(5, vector<int>{4, 8, 20, 32, 44}, [] (const vector<int> & w, const vector<int> & players) {
    int sum = 0;
    for (int player : players) {
      sum += w[player];
    }
    if (sum >= 54) return 1;
    else return 0;
  });
  //vector<double> res = game.shapleyMonteCarlo(100000);
  vector<double> res = game.banzhafMonteCarlo(100000);
  for (double r: res) cout << r << ' ';
  cout << endl;
}*/


void measureSlowDP(VotingGame game) {
  cout << "Slow DP" << endl;
  vector<double> res = game.banzhafDpSlow();
  for (double r: res) cout << r << ' ';
  cout << endl;
  cout << endl;
}

void measureFastDP(VotingGame game) {
  cout << "Fast DP" << endl;
  //game.useFFT = true;
  vector<double> res = game.banzhafDpFast();
  for (double r: res) cout << r << ' ';
  cout << endl;
  cout << endl;
}

void measureDPBadMerge(VotingGame game) {
  cout << "Bad merge DP" << endl;
  //game.useFFT = false;
  vector<double> res = game.banzhafDpFast();
  for (double r: res) cout << r << ' ';
  cout << endl;
  cout << endl;
}

void measureMonteCarlo(VotingGame game) {
  cout << "Monte carlo" << endl;
  vector<double> res = game.banzhafMonteCarlo(10000);
  for (double r: res) cout << r << ' ';
  cout << endl;
  cout << endl;
}

void measureBB(VotingGame game) {
  cout << "Branch and bound" << endl;
  vector<double> res = game.banzhafBranchAndBound();
  for (double r: res) cout << r << ' ';
  cout << endl;
  cout << endl;
}

void measureFastShapley(VotingGame game) {
  auto res = game.shapleyDpFast();
  printVec(res);
}

void measureMonteCarloShapley(VotingGame game) {
  auto res = game.shapleyMonteCarlo(1000000);
  printVec(res);
}

void measureVoting() {
  VotingGame game(cin);
  //measureBB(game);
  //measureDPBadMerge(game);
  measureFastDP(game);
  measureSlowDP(game);
  //measureMonteCarlo(game);
  //measureFastShapley(game);
  //measureSlowShapley(game);
  //measureMonteCarloShapley(game);
}

void measureMicroarray() {
  MicroarrayGame game(cin);
  printVec(game.banzhaf());
  printVec(game.shapley());
}

void test1() {
  //cout << fixed << setprecision(12) << endl;
  vector<double> test;
  for (int i = 0; i < 100; ++i) {
    test.push_back(i);
  }
  for (int i = 0; i < 10000; ++i) {
    auto r = logToPaddedNorm(test);
    for (size_t i = 0; i < r.first.size(); ++i) {
      test[i] = log(r.first[i]) + r.second;
    }
  }

  for (auto &i: test) cout << i << ' ';
  cout << endl;
  exit(0);
}

int main() {
  cout << "running" << endl;
  //cout << fixed << setprecision(40) << endl;
  //cout << fixed << endl;
  /*cout << exp(logSub(log(10), log(12))) << endl;
  return 0;*/

  /*vector<double> a;
  for (int i = 0; i < 5; ++i) {
    a.push_back(10);
  }
  FFT fft;
  auto res = fft.convolution(a, a);
  for (auto i: res) cout << i << ' ';
  cout << endl;
  return 0;*/


  srand(time(0));
  /*example1();
  example2();
  example3();
  example4();
  example5();*/
  //measureVoting();
  measureMicroarray();
  /*vector<double> a = {1, 2, 3};
  vector<double> b = {3, 4, 5};

  FFT fft(a.size());
  auto res = convolution.logConvolution(a, b);
  for (auto i: res) cout << i << ' ';
  cout << endl;*/

  return 0;
}
