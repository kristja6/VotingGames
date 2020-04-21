//
// Created by maty on 06/12/19.
//

#ifndef COAL_GAME_ARGUMENTS_H
#define COAL_GAME_ARGUMENTS_H

#include <string>
#include <vector>
#include "coalitional_game.h"

using namespace std;

#define OPT_SHAPLEY "s"
#define OPT_BANZHAF "b"
#define OPT_READ_FROM_STDIN "i"

typedef pair<vector<int>, int> VotingGameInstance;


struct Arguments {
  const string helpText = "Usage: voting-games [OPTION] ... [FILENAME]\n"
                          "\n"
                          "  Options:\n"
                          "    -b Compute the Banzhaf index\n"
                          "    -s Compute the Shapley value\n"
                          "    -i Read the voting game definition from stdin instead of a file\n";

  // return true on success
  bool ReadArguments(int argc, const char **argv);
  void printHelp() {
    cout << helpText << endl;
  }

  string inputFileName;
  bool has(const string & arg) { return flags.count(arg); }
  std::set<string> flags;
  int topPlayers;
  bool failed = false;
};

VotingGameInstance readVotingGameInstance(istream &);


#endif //COAL_GAME_ARGUMENTS_H
