#include "random.h"
// random permutation of {0, 1, ..., n - 1}
vector<int> random_perm(int n) {
  vector<int> res(n);
  for (int i = 0; i < n; ++ i) res[i] = i;
  random_shuffle(res.begin(), res.end());
  return res;
}

// random subset of {0, 1, ..., n - 1}
vector<int> random_subset(int n) {
  vector<int> res;
  for (int i = 0; i < n; ++ i) {
    if (rand()&2) res.push_back(i);
  }
  return res;
}
