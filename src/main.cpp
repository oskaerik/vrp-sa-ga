#include <cstdio>
#include "common.hpp"
#include "sa.hpp"
#include "ga.hpp"
#include "bruteforce.hpp"

#define N 10
#define M 3

int main() {
  srand(time(NULL));
  Graph g = random_clustered_graph(N, 5, 4);
  auto s = simulated_annealing(g, M);
  s = brute_force(g, M);
  printf("optimal score: %f\n", s.score(g));
}
