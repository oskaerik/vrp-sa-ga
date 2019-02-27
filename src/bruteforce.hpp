#ifndef _BRUTEFORCE_HPP_
#define _BRUTEFORCE_HPP_

#include "common.hpp"
#include <limits>

void brute_force_helper(int n, int m, const std::vector<int> & delimiters, int position, std::vector<std::vector<int>> & partitions) {
    if(position == delimiters.size()){
        partitions.push_back(delimiters);
        return;
    }

    int prev = (position == 0) ? 0 : delimiters.at(position - 1);
    for(int i = prev; i <= n-1; i++){
        std::vector<int> newDelimiters(delimiters);
        newDelimiters.at(position) = i;
        brute_force_helper(n, m, newDelimiters, position + 1, partitions);
    }
    return;
}

Solution brute_force(const Graph & graph, int m, char graph_type) {
    int n = graph.size();

    assert(n >= 3 && m >= 2);
    assert(m < n);

    std::vector<int> sequence(n-1);
    std::iota (sequence.begin(), sequence.end(), 1);

    std::vector<int> delimiters(m-1, 0);
    std::vector<std::vector<int>> partitions;
    brute_force_helper(n, m, delimiters, 0, partitions);

    double minScore = std::numeric_limits<double>::max();
    Solution best_solution;
    do {
        for(std::vector<int> delimiters : partitions){
            Solution solution;
            solution.sequence = sequence;
            solution.delimiters = delimiters;

            double score = solution.score(graph);
            if(score < minScore){
                minScore = score;
                best_solution = solution;
            }
        }
    } while (std::next_permutation(sequence.begin(), sequence.end()));

    char file_name[64];
    sprintf(file_name, "./out/brute_n%lu_m%d_%c.csv", graph.size(), m, graph_type);
    std::ofstream file(file_name);
    file << "score\n" << best_solution.score(graph) << '\n';

    return best_solution;
}

#endif
