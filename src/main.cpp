#include <cstdio>
#include "common.hpp"
#include "sa.hpp"
#include "ga.hpp"
#include "bruteforce.hpp"

#define N 100
#define M 10

int main() {
  srand(time(NULL));
  Graph g = uniform_random_2d_graph(N);
  auto s = genetic_algorithm(g, M);
  printf("ga score: %f\n", s.score(g));
  s.print();
  s = simulated_annealing(g, M);
  printf("sa score: %f\n", s.score(g));
  s.print();
  // s = brute_force(g, M);
  // printf("brute score: %f\n", s.score(g));
  // s.print();
}
