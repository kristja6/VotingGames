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
        switch (argv[i][j]) { // display help
          case 'z': isZeroOneGame = true; break;
          default: return false;
        }
      }
    } else { // is the optimized function
      inputFileName = string(argv[i]);
    }
  }
  return true;
}
