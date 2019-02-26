#ifndef _GA_HPP_
#define _GA_HPP_

#include <utility>
#include <cfloat>
#include "common.hpp"

#define POP_SIZE 1000
#define GEN_LIMIT 1000
#define MUT_RATE 0.1
#define GEN_PRINT GEN_LIMIT+1

using Population = std::vector<Solution>;

Solution aex(Solution &p1, Solution &p2) {
    // Get permutation mappings, from (index) -> to (value)
    std::vector<int> map1 = p1.get_permutation_map();
    std::vector<int> map2 = p2.get_permutation_map();

    // Perform AEX (indicies 0 and 1 are ok)
    Solution c = p1;
    c.permutation_map.clear(); // Not the same permutation map as the parent
    std::vector<bool> added(map1.size(), false);
    added[c.sequence[0]] = added[c.sequence[1]] = true;
    bool p1_turn = false;
    for (int i = 2; i < int(p1.sequence.size()); ++i) {
        int candidate = p1_turn ? map1[c.sequence[i-1]] : map2[c.sequence[i-1]];
        if (added[candidate]) {
            // Pick random non-added node
            std::vector<int> remaining;
            remaining.reserve(c.sequence.size());
            for (int i = 1; i < int(added.size()); ++i)
                if (!added[i]) remaining.push_back(i);
            candidate = remaining[rand() % remaining.size()];
        }
        c.sequence[i] = candidate;
        added[candidate] = true;
        p1_turn = !p1_turn;
    }

    // Sanity check
    assert(added[0] == false);
    for (int i = 1; i < int(added.size()); ++i)
        assert(added[i] == true);

    return c;
}

// Calculate penalty for a solution
double penalty(const Solution &s, const Graph &g) {
    return s.score(g);
}

// Calculate fitness for the whole population
std::vector<double> fitness(const Population &pop, const Graph &g) {
    // Calculate penalties
    std::vector<double> penalties(pop.size());
    double max_penalty = -1;
    for (int i = 0; i < int(penalties.size()); ++i) {
        penalties[i] = penalty(pop[i], g);
        assert(penalties[i] >= 0);
        if (penalties[i] > max_penalty) max_penalty = penalties[i];
    }

    // Calculate fitness for each solution
    std::vector<double> fitnesses(pop.size());
    for (int i = 0; i < int(fitnesses.size()); ++i)
        fitnesses[i] = (max_penalty - penalties[i]) / max_penalty;
    return fitnesses;
}

// Returns the index of the selected solution (using roulette wheel selection)
int roulette(const std::vector<double> &fitnesses) {
    double sum = 0;
    for (auto &f : fitnesses) sum += f;
    double val = sum * (double(rand()) / RAND_MAX);
    for (int i = 0; i < int(fitnesses.size()); ++i) {
        val -= fitnesses[i];
        if (val < 0) return i;
    }
    return fitnesses.back(); // Only happens on rounding error
}

void statistics(const Population &pop, const Graph &g, int generation) {
    printf("Generation\t%d\n", generation);
    printf("Population size\t%d\n", int(pop.size()));

    auto fitnesses = fitness(pop, g);
    double mean = 0;
    std::pair<double, int> best = {DBL_MAX, -1};
    std::pair<double, int> worst = {-DBL_MAX, -1};
    std::vector<double> penalties(pop.size());
    for (int i = 0; i < int(penalties.size()); ++i) {
        penalties[i] = penalty(pop[i], g);
        mean += penalties[i]/pop.size();
        if (penalties[i] < best.first) best = {penalties[i], i};
        if (penalties[i] > worst.first) worst = {penalties[i], i};
    }

    printf("Mean route\t%lf\n", mean);
    printf("Best route\t%lf\n", best.first);
    pop[best.second].print();
    printf("Worst route\t%lf\n", worst.first);
    pop[worst.second].print();
    printf("\n");
}

Solution genetic_algorithm(const Graph &g, int m) {
    int n = g.size();
    assert(n > 2);
    assert(m > 1);
    assert(m < n);

    // Generate a random population
    Population pop(POP_SIZE);
    for (auto &s : pop)
        s.randomize(n, m);

    // Perform GA
    for (int i = 0; i < GEN_LIMIT; ++i) {
        if ((i+1) % GEN_PRINT == 0) statistics(pop, g, i+1);
        auto fitnesses = fitness(pop, g);
        Population pop_next(POP_SIZE);
        for (int j = 0; j < int(pop_next.size()); ++j) {
            // Create child and (maybe) mutate it
            auto p1 = pop[roulette(fitnesses)];
            auto p2 = pop[roulette(fitnesses)];
            auto c = aex(p1, p2);
            if (double(rand()) / RAND_MAX < MUT_RATE) c.mutate();

            // If c is better than the parents, keep it, otherwise roulette
            auto p1_penalty = penalty(p1, g);
            auto p2_penalty = penalty(p2, g);
            auto c_penalty = penalty(c, g);
            Solution chosen = c;
            if (c_penalty >= p1_penalty || c_penalty >= p2_penalty) {
                // Roulette between p1, p2 and c
                Population mini = {p1, p2, c};
                chosen = mini[roulette(fitness(mini, g))];
            }
            pop_next[j] = chosen;
        }
        pop = pop_next;
    }

    // Select best solution
    auto fitnesses = fitness(pop, g);
    std::pair<double, int> best = {-1, -1};
    for (int i = 0; i < int(fitnesses.size()); ++i)
        if (fitnesses[i] > best.first) best = {fitnesses[i], i};
    return pop[best.second];
}

#endif /* _GA_HPP_ */
