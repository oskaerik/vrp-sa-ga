#include <cstdio>
#include <fstream>
#include "common.hpp"
#include "sa.hpp"
#include "ga.hpp"
#include "bruteforce.hpp"

#define N 1000
#define M 100
#define C 10
#define BRUTE_LIMIT 10

int main() {
  srand(time(NULL));
  Graph clustered_uniform_g = clustered_and_uniform_graph(1000, 10, 3);
  auto s = simulated_annealing(clustered_uniform_g, 3, 'm');
}
