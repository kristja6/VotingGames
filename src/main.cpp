#include <vector>
#include <set>
#include "coal_game.h"
#include "voting_game.h"
#include "math.h"
#include "voting_nonunique.h"
#include "arguments.h"

void example1() {
  cout << "Monte carlo" << endl;
  const vector<int> w = vector<int>{4, 8, 20, 32, 44};
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
  VotingGame game(vector<int>{4, 8, 0, 20, 32, 0, 44, 1}, 54);
  cout << "Banzhaf: " << endl;
  printVec(game.banzhaf());
  cout << "Shapley: " << endl;
  printVec(game.shapley());
}

void example3() {
  cout << "Enumeration" << endl;
  const vector<int> w = vector<int>{4, 8, 20, 32, 44};
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

int main(int argc, const char ** argv) {
  srand(time(0));
  Arguments args;
  args.ReadArguments(argc, argv);
  auto instance = readVotingGameInstance();

  if (args.has("shapley")) {
    if (args.has("uno")) printVec(VotingGame(instance.first, instance.second).shapleyUnoDp());
    else printVec(VotingNonunique(instance.first, instance.second).shapleyNewDp());
  } else if (args.has("banzhaf")) {
    if (args.has("uno")) printVec(VotingGame(instance.first, instance.second).banzhafUnoDp());
    else if (args.has("naive")) printVec(VotingGame(instance.first, instance.second).banzhafNaiveDp());
    else printVec(VotingNonunique(instance.first, instance.second).banzhafNewDp());
  }

  return 0;
}
