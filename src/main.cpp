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
  CoalGame game(5,  [w] (const vector<int> & players) {
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
  CoalGame game(5,  [w] (const vector<int> & players) {
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

  if (args.has("shapley")) {
    if (args.has("opt")) {
      if (args.has("new-only-convolution")) printVec(VotingNonunique(instance.first, instance.second).shapleyNew());
      if (args.has("new-for-each")) printVec(VotingNonunique(instance.first, instance.second).shapleyNewForEachPlayer());
    } else if (args.has("normal")) {
      if (args.has("new")) printVec(VotingGame(instance.first, instance.second).shapleyNew());
      if (args.has("new-for-each")) printVec(VotingGame(instance.first, instance.second).shapleyNewForEach());
      if (args.has("uno")) printVec(VotingGame(instance.first, instance.second).shapleyUnoDp());
    }
  } else if (args.has("banzhaf")) {
    if (args.has("opt")) {
      if (args.has("new-only-convolution")) printVec(VotingNonunique(instance.first, instance.second).banzhafNewOnlyConvolution());
      if (args.has("new-with-deconvolution")) printVec(VotingNonunique(instance.first, instance.second).banzhafNewWithDeconvolution());
    } else if (args.has("normal")) {
      if (args.has("uno")) printVec(VotingGame(instance.first, instance.second).banzhafUnoDp());
      if (args.has("naive")) printVec(VotingGame(instance.first, instance.second).banzhafNaiveDp());
    }
  }

  return 0;
}
