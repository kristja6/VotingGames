//
// Created by maty on 06/12/19.
//

#include <sstream>
#include "arguments.h"

bool Arguments::ReadArguments(int argc, const char **argv) {
  if (argc <= 1) return false;
  for (int i = 1; i < argc; ++ i) {
    string cur = string(argv[i]);
    if (cur.empty()) continue;
    if (cur[0] == '-') {
      cur = cur.substr(1);
      for (char c: cur) flags.insert(string(1, c));
    } else {
      inputFileName = cur;
    }
  }
  return true;
}

VotingGameInstance readVotingGameInstance(istream & in) {
  VotingGameInstance res;
  int n;
  in >> n;
  if (!in) throw exception();
  res.first = vector<int>(n);
  in >> res.second;
  for (int i = 0; i < n; ++i) {
    if (!in) throw exception();
    in >> res.first[i];
  }
  return res;
}
