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

Graph uniform_random_2d_graph(int n){
    assert(n >= 3);
    
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0, 10); // Random uniform double distribution in [0, 10)

    std::vector<double> xs;
    std::vector<double> ys;
    for(int i = 0; i < n; i++){
        xs.push_back(dis(gen));
        ys.push_back(dis(gen));
    }

    Graph graph(n, std::vector<double>(n, 0));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            double dist = sqrt(pow(xs.at(j) - xs.at(i) , 2) + pow(ys.at(j) - ys.at(i), 2));
            graph.at(i).at(j) = dist;    
        }   
    }
    return graph;
}


class Solution {
    public:
        std::vector<int> sequence;
        std::vector<int> delimiters;

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
        void print() {
            printf("Vehicle 0: ");
            for (int j = 0; j < delimiters[0]; ++j)
                printf("%d ", sequence[j]);
            printf("\n");

            for (int i = 1; i < int(delimiters.size()); ++i) {
                printf("Vehicle %d: ", i);
                for (int j = delimiters[i-1]; j < delimiters[i]; ++j)
                    printf("%d ", sequence[j]);
                printf("\n");
            }

            printf("Vehicle %d: ", int(delimiters.size()));
            for (int j = delimiters.back(); j < int(sequence.size()); ++j)
                printf("%d ", sequence[j]);
            printf("\n\n");
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