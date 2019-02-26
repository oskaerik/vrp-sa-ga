#include <cstdio>
#include "common.hpp"
#include "sa.hpp"
#include "ga.hpp"
#include "bruteforce.hpp"

#define N 10
#define M 3

int main() {
  srand(time(NULL));
  auto g = clustered_and_uniform_graph(1000, 10, 1);
}
