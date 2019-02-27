#include <cstdio>
#include <fstream>
#include <vector>
#include <array>
#include "common.hpp"
#include "sa.hpp"
#include "ga.hpp"
#include "bruteforce.hpp"

std::vector<std::array<int,3>> test_cases = {
  {10, 3, 3},
  {10, 5, 3},
  {100, 10, 5},
  {500, 50, 25},
  {1000, 10, 100},
  {1000, 100, 100},
  {1000, 200, 100}
};

int main() {
  srand(time(NULL));
  for (auto[n,m,c] : test_cases) {
    printf("\n\nRunning testcase:\nn: %d m: %d c: %d\n", n, m, c);
    auto u_g = uniform_graph(n);
    auto c_g = clustered_graph(n, c, 3);
    auto m_g = clustered_and_uniform_graph(n, c, 3);

    simulated_annealing(u_g, m, 'u');
    simulated_annealing(c_g, m, 'c');
    simulated_annealing(m_g, m, 'm');
    puts("SA done.");

    genetic_algorithm(u_g, m, 'u');
    genetic_algorithm(c_g, m, 'c');
    genetic_algorithm(m_g, m, 'm');
    puts("GA done.");

    if (n > 11) continue;

    brute_force(u_g, m, 'u');
    brute_force(c_g, m, 'c');
    brute_force(m_g, m, 'm');
    puts("Bruteforce done.");
  }
}
