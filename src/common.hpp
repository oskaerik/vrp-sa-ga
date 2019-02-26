#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <cmath>
#include <cassert>
#include <cstdio>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>

using Graph = std::vector<std::vector<double>>;

Graph uniform_random_graph(int n, int size = 10){
    assert(n >= 3);

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0, size); // Random uniform double distribution in [0, 10)

    std::vector<double> xs;
    std::vector<double> ys;
    for(int i = 0; i < n; i++){
        xs.push_back(dis(gen));
        ys.push_back(dis(gen));
    }

    Graph graph(n, std::vector<double>(n));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < n; j++)
            graph[i][j] = sqrt(pow(xs[j] - xs[i] , 2) + pow(ys[j] - ys[i], 2));
    return graph;
}

Graph random_clustered_graph(int n, int c, int sd, int size = 100) {
  assert(n >= 3);
  assert(c >= 2);

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> cluster_dis(0, size); // Random uniform double distribution in [0, 10)
  std::normal_distribution<> node_dis(0, sd);

  std::vector<std::pair<double,double>> clusters(c);
  for (int i = 0; i < c; ++i) {
    clusters[i] = { cluster_dis(gen), cluster_dis(gen) };
  }

  std::vector<std::pair<double,double>> nodes(n);
  for (int i = 0; i < n; ++i) {
    auto cluster = clusters[rand() % c];
    double x = cluster.first + node_dis(gen);
    double y = cluster.second + node_dis(gen);
    nodes[i] = { x, y };
  }

  Graph graph(n, std::vector<double>(n));
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) {
      double x = nodes[i].first - nodes[j].first;
      double y = nodes[i].second - nodes[j].second;
      graph[i][j] = sqrt(x*x + y*y);
    }
  return graph;
}

class Solution {
public:
    std::vector<int> sequence;
    std::vector<int> delimiters;

    // Calculates score of solution
    double score(const Graph & graph) const {
        int n = graph.size();
        int m = delimiters.size() + 1;
        assert(n >= 3 && m >= 2);
        assert(m < n);

        double maxRouteLength = -1;
        int i = 0;
        for (int j = 0; j <= int(delimiters.size()); j++){ //sic!
            int delimiter = (j < int(delimiters.size())) ? delimiters.at(j) : n - 1;
            if(i == delimiter)
                continue;

            double routeLength = graph.at(0).at(sequence.at(i));
            for(; i < delimiter - 1; i++){
                routeLength += graph.at(sequence.at(i)).at(sequence.at(i + 1));
            }
            routeLength += graph.at(sequence.at(i)).at(0);
            maxRouteLength = std::max(maxRouteLength, routeLength);

            if (++i >= n-1) break;
        }
        return maxRouteLength;
    }

    // Generates a random solution
    void randomize(int n, int m) {
        sequence.resize(n-1);
        delimiters.resize(m-1);

        // Generate random sequence
        for (int i = 0; i < int(sequence.size()); ++i)
            sequence[i] = i+1;
        std::random_shuffle(sequence.begin(), sequence.end());

        // Set delimiters randomly
        for (int i = 0; i < int(delimiters.size()); ++i)
            delimiters[i] = rand() % n;
        std::sort(delimiters.begin(), delimiters.end());
    }

    // Mutates the solution
    void mutate() {
        if (rand() % 2 == 0) mutation_swap();
        else mutation_delimiter();
    }

    // Print solution for debugging
    void print() const {
        printf("\tVehicle 0: ");
        for (int j = 0; j < delimiters[0]; ++j)
            printf("%d ", sequence[j]);
        printf("\n");

        for (int i = 1; i < int(delimiters.size()); ++i) {
            printf("\tVehicle %d: ", i);
            for (int j = delimiters[i-1]; j < delimiters[i]; ++j)
                printf("%d ", sequence[j]);
            printf("\n");
        }

        printf("\tVehicle %d: ", int(delimiters.size()));
        for (int j = delimiters.back(); j < int(sequence.size()); ++j)
            printf("%d ", sequence[j]);
        printf("\n");
    }

    // Used by GA in AEX
    std::vector<int> permutation_map;
    std::vector<int> get_permutation_map() {
        if (!permutation_map.empty()) return permutation_map;
        permutation_map = std::vector<int>(sequence.size()+1);
        permutation_map[sequence.back()] = sequence[0];
        for (int i = 1; i < int(permutation_map.size())-1; ++i)
            permutation_map[sequence[i-1]] = sequence[i];
        return permutation_map;
    }
private:
    // Swap two nodes in the sequence
    void mutation_swap() {
        int i = rand() % sequence.size();
        int j = i;
        while (j == i)
            j = rand() % sequence.size();
        std::swap(sequence[i], sequence[j]);
    }

    // Helper to move the delimiter (if possible)
    bool move_delimiter(int i, bool left) {
        if (left && delimiters[i] > 0
                && (i == 0 || delimiters[i-1] < delimiters[i])) {
            --delimiters[i];
            return true;
        } else if (!left && delimiters[i] < int(sequence.size())
                && (i == int(delimiters.size())-1 || delimiters[i+1] > delimiters[i])) {
            ++delimiters[i];
            return true;
        }
        return false;
    }

    // Mutates the solution by moving a random delimiter right or left
    void mutation_delimiter() {
        bool flipped = false;
        int start = rand() % delimiters.size();
        int i = start;
        int direction = rand() % 2 == 0 ? -1 : 1;
        while (true) {
            // Try moving left first if direction is -1, otherwise try right first
            if (move_delimiter(i, direction == -1)) return;
            if (move_delimiter(i, direction != -1)) return;
            i += direction;
            if (i < 0 || i >= int(delimiters.size())) {
                // Out of bounds
                assert(!flipped); // Sanity check, should happen max once
                i = start;
                direction *= -1;
                flipped = true;
            }
        }
    }
};

#endif
