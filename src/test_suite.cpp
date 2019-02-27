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

using namespace std::chrono;

void print_time(const high_resolution_clock::time_point &before) {
  auto after = high_resolution_clock::now();
  auto time_span = duration_cast<duration<double>>(after - before);
  printf("    Time: %fs\n", time_span.count());
}

int main() {
  srand(time(NULL));
  std::ofstream uni_g_file("./out/uniform_11.graph");
  Graph uniform_g = uniform_graph(uni_g_file, 11);
  std::ofstream cluster_g_file("./out/clustered_11_3.graph");
  Graph clusted_g = clustered_graph(cluster_g_file, 11, 3, 3);
  std::ofstream cluster_uni_g_file("./out/uniform_11_3.graph");
  Graph clustered_uniform_g = clustered_and_uniform_graph(cluster_uni_g_file, 11, 3, 3);
}
