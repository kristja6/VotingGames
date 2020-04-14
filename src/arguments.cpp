//
// Created by maty on 06/12/19.
//

#include <sstream>
#include "arguments.h"

bool Arguments::ReadArguments(int argc, const char **argv) {
  if (argc <= 1) return false;
  string allArgs;
  for (int i = 1; i < argc; ++ i) {
    allArgs += string(argv[i]) + " ";
  }
  stringstream str(allArgs);
  while (str) {
    string cur;
    str >> cur;
    if (cur.size() < 2) continue;
    cur = cur.substr(2);
    if (cur == "top") {
      str >> topPlayers;
    } else namedArguments.insert(cur);
  }
  return true;
}

pair<vector<int>, int> readVotingGameInstance() {
  pair<vector<int>, int> res;
  int n;
  cin >> n;
  res.first = vector<int>(n);
  cin >> res.second;
  for (int i = 0; i < n; ++i) {
    cin >> res.first[i];
  }
  return res;
}
