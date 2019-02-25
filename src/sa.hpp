#ifndef _SA_HPP_
#define _SA_HPP_

#include "common.hpp"

/*
  Always accept equal or better solutions.
  If worse accept with some probability, dependent on
  how much worse and current temperature.
 */
bool accept(double new_s, double old_s, double temp) {
  if (new_s <= old_s) return true;
  return exp((old_s - new_s) / temp) > double(rand()) / RAND_MAX;
}

Solution simulated_annealing(const Graph &g, int m) {
  Solution curr;
  curr.randomize(g.size(),m);
  Solution best = curr;
  double best_score = curr.score(g);
  double curr_score = best_score;

  double temp = 5000, cool_rate = 0.99999;
  int since_improvement = 0;
  while (++since_improvement < 1000000) {
    temp *= cool_rate;
    Solution s = curr;
    s.mutate();
    double new_score = s.score(g);
    if (!accept(new_score, curr_score, temp))
      continue;
    curr_score = new_score;
    curr = s;
    if (curr_score < best_score) {
      best_score = new_score;
      best = curr;
      since_improvement = 0;
    }
  }
  return best;
}

#endif /* _SA_HPP_ */
