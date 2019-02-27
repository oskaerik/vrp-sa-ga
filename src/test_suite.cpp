#include <cstdio>
#include <fstream>
#include <vector>
#include <array>
#include "common.hpp"
#include "solution.hpp"
#include "sa.hpp"
#include "ga.hpp"
#include "bruteforce.hpp"

#define RUNS 5

std::vector<std::array<int,3>> test_cases = {
  {10, 3, 3},
  {10, 5, 3},
  {100, 10, 5},
  {500, 50, 25},
  {1000, 10, 100},
  {1000, 100, 100},
  {1000, 200, 100},
};

int main() {
  srand(time(NULL));
  // Total runtime: 1h 8min
  for (auto test : test_cases) {
    auto n = test[0];
    auto m = test[1];
    auto c = test[2];
    printf("\n\nRunning testcase:\nn: %d m: %d c: %d\n", n, m, c);
    auto u_g = uniform_graph(n);
    auto c_g = clustered_graph(n, c, 3);
    auto m_g = clustered_and_uniform_graph(n, c, 3);

    for (int i = 0; i < RUNS; ++i) simulated_annealing(u_g, m, c, 'u', i);
    for (int i = 0; i < RUNS; ++i) simulated_annealing(c_g, m, c, 'c', i);
    for (int i = 0; i < RUNS; ++i) simulated_annealing(m_g, m, c, 'm', i);
    puts("SA done.");

    for (int i = 0; i < RUNS; ++i) genetic_algorithm(u_g, m, c, 'u', i, 10);
    for (int i = 0; i < RUNS; ++i) genetic_algorithm(c_g, m, c, 'c', i, 10);
    for (int i = 0; i < RUNS; ++i) genetic_algorithm(m_g, m, c, 'm', i, 10);
    puts("GA done.");

    if (n > 11) continue;

    brute_force(u_g, m, c, 'u');
    brute_force(c_g, m, c, 'c');
    brute_force(m_g, m, c, 'm');
    puts("Bruteforce done.");
  }
}
