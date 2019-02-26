#ifndef _SA_HPP_
#define _SA_HPP_

#include <cfloat>
#include "common.hpp"

#define COOL_RATE 0.9999
#define IMPROVE_LIMIT 100000

/*
  Always accept equal or better solutions.
  If worse accept with some probability, dependent on
  how much worse and current temperature.
 */
bool accept(double new_s, double old_s, double temp) {
  if (new_s <= old_s) return true;
  return exp((old_s - new_s) / temp) > double(rand()) / RAND_MAX;
}

struct SA_Answer {
  Solution s;
  int iterations;
  int improvements;
  double score;

  SA_Answer(Solution s, int iterations, int improvements, double score)
  : s(s), iterations(iterations), improvements(improvements), score(score) { }
};

SA_Answer simulated_annealing(const Graph &graph, int m) {
  Solution curr, best, next;
  curr.randomize(graph.size(),m);
  double curr_score = curr.score(graph), best_score = DBL_MAX;

  int reheat = 1, iterations = 0, improvements = 0;
  while (reheat --> 0) {
    double temp = 5000;
    int since_improve = IMPROVE_LIMIT;
    while (since_improve --> 0) {
      ++iterations;
      temp *= COOL_RATE;
      next = curr;
      next.mutate();
      double new_score = next.score(graph);
      if (!accept(new_score, curr_score, temp))
        continue;
      curr_score = new_score;
      curr = next;
      if (curr_score >= best_score)
        continue;
      ++improvements;
      best_score = new_score;
      best = curr;
      since_improve = IMPROVE_LIMIT;
    }
  }
  return { best, iterations, improvements, best_score };
}

#endif /* _SA_HPP_ */
