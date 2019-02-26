#include <cstdio>
#include "common.hpp"
#include "sa.hpp"
#include "ga.hpp"
#include "bruteforce.hpp"

#define N 1000
#define M 100
#define C 10
#define BRUTE_LIMIT 10

using namespace std::chrono;

void print_time(const high_resolution_clock::time_point &before) {
  auto after = high_resolution_clock::now();
  auto time_span = duration_cast<duration<double>>(after - before);
  printf("    Time: %fs\n", time_span.count());
}

void test(const Graph &g, int m) {
  {
    auto before = high_resolution_clock::now();
    auto sa = simulated_annealing(g, m);
    printf("  SA:\n");
    printf("    Score: %f\n", sa.score);
    printf("    Iterations: %d\n", sa.iterations);
    printf("    Improvements: %d\n", sa.improvements);
    print_time(before);
  }
  {
    auto before = high_resolution_clock::now();
    auto ga = genetic_algorithm(g, m);
    printf("  GA:\n");
    printf("    Score: %f\n", ga.score);
    printf("    Iterations: %d\n", ga.iterations);
    print_time(before);
  }
  if (g.size() > BRUTE_LIMIT) return;
  {
    auto before = high_resolution_clock::now();
    auto brute = brute_force(g, m);
    printf("  Bruteforce:\n");
    printf("    Score: %f\n", brute.score(g));
    print_time(before);
  }
}

void test_all_graphs(int n, int m, int c) {
  printf("nodes: %d\n", n);
  printf("vehicles: %d\n", m);
  {
    Graph graph = clustered_graph(n, c, 4);
    printf("clustered graph:\n  clusters: %d\n", c);
    test(graph, m);
  }
  putchar('\n');
  {
    Graph graph = uniform_graph(n);
    printf("uniform graph:\n");
    test(graph, m);
  }
  putchar('\n');
  {
    Graph graph = clustered_and_uniform_graph(n, c, 4);
    printf("clustered and uniform graph:\n");
    test(graph, m);
  }

  puts("\n---------------\n");
}

int main() {
  srand(time(NULL));
  test_all_graphs(N, M, C);
}
