#ifndef _SA_HPP_
#define _SA_HPP_

#include "common.hpp"
#include "solution.hpp"

#define START_TEMP        5000
#define COOL_RATE         0.9999
#define IMPROVE_LIMIT     100000
#define MS_PRINT_INTERVAL 500
#define REHEATS           5

/*
  Always accept if the solution is better.
  If it's worse accept with a probablity
  proportional to how much worse and the
  current temperature.
 */
bool accept(double new_s, double old_s, double temp) {
  if (new_s <= old_s) return true;
  return exp((old_s - new_s) / temp) > double(rand()) / RAND_MAX;
}

Solution simulated_annealing(const Graph &graph, int m, int c, char graph_type, int run) {
  // Create a file for csv output
  char file_name[64];
  sprintf(file_name, "./out/sa_n%lu_m%d_c%d_%c_%d.csv", graph.size(), m, c, graph_type, run);
  std::ofstream file(file_name);
  file << "score,time\n";

  // Start the timer
  auto t0 = high_resolution_clock::now();
  auto printed = t0;

  // Setup experiment
  Solution curr, best, next;
  curr.randomize(graph.size(), m);
  double curr_score = curr.score(graph);
  double best_score = DBL_MAX;

  file << curr_score << ',' << time_since(t0) << '\n';

  // Perform SA
  int reheat = REHEATS;
  while (reheat --> 0) {
    // We did not find any better solution
    // in IMPROVE_LIMIT iterations so reheat
    double temp = START_TEMP;
    int since_improve = IMPROVE_LIMIT;
    while (since_improve --> 0) {
      temp *= COOL_RATE;
      if (time_since(printed) > MS_PRINT_INTERVAL) {
        file << best_score << ',' << time_since(t0) << '\n';
        printed = high_resolution_clock::now();
      }

      // Find a local solution
      next = curr;
      next.mutate();
      double new_score = next.score(graph);
      if (!accept(new_score, curr_score, temp))
        continue;

      // We accept the new solution
      // Set it to the current solution
      curr = next;
      curr_score = new_score;
      if (curr_score >= best_score)
        continue;

      // Current solution is the best so far, save it
      best = curr;
      best_score = curr_score;
      since_improve = IMPROVE_LIMIT;
    }
  }
  return best;
}

#endif /* _SA_HPP_ */
