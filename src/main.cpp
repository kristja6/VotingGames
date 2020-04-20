#include <vector>
#include <set>
#include <fstream>
#include "coalitional_game.h"
#include "math.h"
#include "voting_game.h"
#include "arguments.h"

void example1() {
  cout << "Monte carlo" << endl;
  const vector<int> w = vector<int>{4, 8, 20, 32, 44};
  CoalitionalGame game(5, [w] (const vector<int> & players) {
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
  CoalitionalGame game(5, [w] (const vector<int> & players) {
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

void readingFileFailed() {
  cout << "Error: reading from the file has failed" << endl;
  exit(1);
};

int main(int argc, const char ** argv) {
  srand(time(0));
  Arguments args;
  args.ReadArguments(argc, argv);

  if (!args.has(OPT_SHAPLEY) && !args.has(OPT_BANZHAF)) {
    args.printHelp();
    return 1;
  }

  VotingGameInstance instance;
  if (args.has(OPT_READ_FROM_STDIN)) instance = readVotingGameInstance(cin);
  else {
    if (args.inputFileName.empty()) {
      readingFileFailed();
    }
    try {
      ifstream in(args.inputFileName);
      if (in.fail()) readingFileFailed();
      instance = readVotingGameInstance(in);
    } catch (...) {
      readingFileFailed();
    }
  }


  cout.precision(numeric_limits<double>::max_digits10);
  if (args.has(OPT_SHAPLEY)) {
    printVec(VotingGame(instance.first, instance.second).shapley());
  }
  if (args.has(OPT_BANZHAF)) {
    printVec(VotingGame(instance.first, instance.second).banzhaf());
  }

  return 0;
}
