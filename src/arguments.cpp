//
// Created by maty on 06/12/19.
//

#include "arguments.h"

bool Arguments::ReadArguments(int argc, const char **argv) {
  if (argc <= 1) return false;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') { // a switch argument
      size_t len = strlen(argv[i]);
      for (int j = 1; j < (int)len; ++j) {
        string res;
        switch (argv[i][j]) { // display help
          case 'z': isZeroOneGame = true; break;
          case '-':
            for (size_t k = j + 1; k < len; ++ k) res.push_back(argv[i][k]);
            namedArguments.insert(res);
            j = len;
            break;
          default:
            return false;
        }
      }
    } else { // is the optimized function
      inputFileName = string(argv[i]);
    }
  }
  return true;
}

pair<vector<ll>, ll> readVotingGameInstance() {
  pair<vector<ll>, ll> res;
  int n;
  cin >> n;
  res.first = vector<ll>(n);
  cin >> res.second;
  for (int i = 0; i < n; ++i) {
    cin >> res.first[i];
  }
  return res;
}
