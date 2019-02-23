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
  Solution s;
  s.randomize(g.size(),m);
  Solution best = s;
  double best_score = s.score(g);
  double curr_score = best_score;

  double temp = 100, cool_rate = 0.99999;
  for (int i = 0; i < 1000000; ++i) {
    temp *= cool_rate;
    Solution s2 = s;
    s2.mutate();
    double new_score = s2.score(g);
    if (!accept(new_score, curr_score, temp))
      continue;
    curr_score = new_score;
    s = s2;
    if (curr_score < best_score) {
      best_score = new_score;
      best = s;
    }
  }
  return best;
}

#endif /* _SA_HPP_ */
