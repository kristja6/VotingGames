#include <bits/stdc++.h>
#include "coal_game.h"
#include "voting_game.h"
#include "math.h"
#include "microarray_game.h"
#include "voting_nonunique.h"
#include "arguments.h"

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
  cout << "Banzhaf: " << endl;
  printVec(game.banzhafMonteCarlo(100000));
  cout << "Shapley: " << endl;
  printVec(game.shapleyMonteCarlo(100000));
}

void example2() {
  cout << "DP" << endl;
  VotingGame game(vector<ll>{4, 8, 0, 20, 32, 0, 44, 1}, 54);
  cout << "Banzhaf: " << endl;
  printVec(game.banzhaf());
  cout << "Shapley: " << endl;
  printVec(game.shapley());
}

void example3() {
  cout << "Enumeration" << endl;
  const vector<ll> w = vector<ll>{4, 8, 20, 32, 44};
  CoalGame<int> game(5,  [w] (const vector<int> & players) {
    int sum = 0;
    for (int player : players) {
      sum += w[player];
    }
    if (sum >= 54) return 1;
    else return 0;
  });
  cout << "Banzhaf: " << endl;
  printVec(game.banzhafEnum());
  cout << "Shapley: " << endl;
  printVec(game.shapleyEnum());
}

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
  cout << "normal" << endl;
  printVec(game.banzhaf());
  cout << "slow" << endl;
  //printVec(game.banzhafDpSlow());
  VotingNonunique game2(game.weights, game.quota);
  cout << "fft" << endl;
  printVec(game2.banzhaf());
  cout << "fft n^2" << endl;
  //printVec(game2.banzhafSlow());
  cout << "fft tree" << endl;
  //printVec(game.banzhafFast());
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
  cout << "PLAYERS: " << game.players << endl;
  //printVec(game.shapley(true));
  cout << "normal" << endl;
  //cout << (game.shapley()) << endl;
  //printVec(game.shapley());
  //printVec(game.shapleyNew());
  //printVec(game.shapleyEnum());
  cout << "nonunique weights" << endl;
  VotingNonunique game2(game.weights, game.quota);
  //printVec(game2.shapley());
  printVec(game2.shapleyTest());
}

void measureMonteCarloShapley(VotingGame game) {
  auto res = game.shapleyMonteCarlo(1000000);
  printVec(res);
}

void measureBestCoalition(MicroarrayGame game) {
  for (int i = 1; i < 3; ++i) {
    vector<int> vec = game.optimalCoalitionOfFixedSize(i);
    cout << game.v(vec) << endl;
    printVec(vec);
  }
}

void measureVoting() {
  VotingGame game(cin);
  //measureBB(game);
  //measureDPBadMerge(game);
  //measureFastDP(game);
  //measureSlowDP(game);
  //measureMonteCarlo(game);
  measureFastShapley(game);
  //measureSlowShapley(game);
  //measureMonteCarloShapley(game);
  /*for (int i = 1; i < 3; ++i) {
    vector<int> vec = game.optimalCoalitionOfFixedSize(i);
    cout << game.v(vec) << endl;
    printVec(vec);
  }*/
}

void measureMicroarray() {
  MicroarrayGame game(cin);
  printVec(game.banzhaf());
  //printVec(game.shapley());
  for (int i = 0; i < game.players; ++i) {
    cout << game.banzhafInteraction(vector<int>{i}) << ' ' << flush;
  }
  cout << endl;
}

void measureSumOfVoting() {

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

void testInteractionIndex() {
  cout << "Interaction index" << endl;
  //VotingGame game(5, vector<ll>{4, 8, 20, 32, 44}, 54);
  VotingGame game(vector<ll>{4, 4, 4, 4, 1, 1, 1, 1}, 8);
  printVec(game.banzhaf());
  for (int i = 0; i < game.players; ++i) {
    for (int j = 0; j < game.players; ++j) {
      std::set<int> subset = {i, j};
      cout << i << ' ' << j << ": " << game.banzhafInteractionEnum(vector<int>(subset.begin(), subset.end())) << endl;
    }
  }
  cout << endl;
}

int main(int argc, const char ** argv) {
  srand(time(0));
  Arguments args;
  args.ReadArguments(argc, argv);
  auto instance = readVotingGameInstance();

  CoalGame<ll> * game = nullptr;
  if (args.has("unique")) {
    game = new VotingGame(instance.first, instance.second);
  } else if (args.has("nonunique")) {
    game = new VotingNonunique(instance.first, instance.second);
  }
  if (game == nullptr) {
    // TODO: meaningful output here
    cout << "Specify algorithm" << endl;
  }

  if (args.has("shapley")) {
    printVec(game->banzhaf());
  } else {
    printVec(game->shapley());
  }

  return 0;
}
