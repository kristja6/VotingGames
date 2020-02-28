//
// Created by maty on 06/12/19.
//

#ifndef COAL_GAME_ARGUMENTS_H
#define COAL_GAME_ARGUMENTS_H

#include <string>
#include <vector>
#include "coal_game.h"
using namespace std;

struct Arguments {
  const string helpText = "Write something here";

  // return true on success
  bool ReadArguments(int argc, const char **argv);
  void printHelp() {
    cout << helpText << endl;
  }

  bool isZeroOneGame = false;
  string inputFileName;
  bool has(const string & arg) { return namedArguments.count(arg); }
  std::set<string> namedArguments;
  int topPlayers;
};

pair<vector<int>, int> readVotingGameInstance();;


#endif //COAL_GAME_ARGUMENTS_H
