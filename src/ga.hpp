#ifndef _GA_HPP_
#define _GA_HPP_

#include "common.hpp"
#include "solution.hpp"

#define POP_SIZE 1000
#define GEN_LIMIT 1000
#define MUT_RATE 0.1
#define GEN_PRINT GEN_LIMIT+1

using Population = std::vector<Solution>;

// Performs the AEX crossover to create a child from two parents
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

// Performs GA
Solution genetic_algorithm(const Graph &g, int m, int c, char graph_type, int run, int pop_size = POP_SIZE) {
    // Create file for output
    char file_name[64];
    sprintf(file_name, "./out/ga_n%lu_m%d_c%d_%c_%d.csv", g.size(), m, c, graph_type, run);
    std::ofstream file(file_name);
    file << "score,time\n";

    // Set up experiment
    int n = g.size();
    assert(n > 2);
    assert(m > 1);
    assert(m < n);
    auto t0 = std::chrono::high_resolution_clock::now();

    // Generate a random population
    Population pop(pop_size);
    for (auto &s : pop)
        s.randomize(n, m);

    // Perform GA
    std::pair<Solution, double> best = { pop[0], DBL_MAX };
    for (int i = 0; i < GEN_LIMIT; ++i) {
        auto fitnesses = fitness(pop, g);
        Population pop_next(pop_size);
        for (int j = 0; j < int(pop_next.size()); ++j) {
            // Create child and (maybe) mutate it
            auto p1 = pop[roulette(fitnesses)];
            auto p2 = pop[roulette(fitnesses)];
            auto c = aex(p1, p2);
            if (double(rand()) / RAND_MAX < MUT_RATE) c.mutate();

            // If c is better than the parents, keep it, otherwise roulette
            Population cp_pop = { c, p1, p2 };
            std::vector<double> cp_pen = { penalty(c, g), penalty(p1, g), penalty(p2, g) };
            Solution chosen = cp_pop[0];
            double chosen_pen = cp_pen[0];
            if (cp_pen[0] >= cp_pen[1] || cp_pen[0] >= cp_pen[2]) {
                // Roulette between p1, p2 and c
                int r = roulette(fitness(cp_pop, g));
                chosen = cp_pop[r];
                chosen_pen = cp_pen[r];
            }
            pop_next[j] = chosen;
            if (chosen_pen < best.second) best = { chosen, chosen_pen };
        }
        file << best.second << ',' << time_since(t0) << '\n';
        pop = pop_next;
    }
    return best.first;
}

#endif /* _GA_HPP_ */
