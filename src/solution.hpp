#ifndef _SOLUTION_HPP_
#define _SOLUTION_HPP_

#include "common.hpp"

// Represents a solution
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
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(sequence.begin(), sequence.end(), gen);

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

    // Overloads << to write out a solution, as nodes allocated per vehicle
    friend std::ostream& operator<< (std::ostream& stream, Solution &s) {
      stream << ",";
      for (int j = 0; j < s.delimiters[0]; ++j)
        stream << s.sequence[j] << ",";
      stream << "\n";

      for (int i = 1; i < int(s.delimiters.size()); ++i) {
          stream << ",";
          for (int j = s.delimiters[i-1]; j < s.delimiters[i]; ++j)
            stream << s.sequence[j] << ",";
          stream << "\n";
      }

      stream << ",";
      for (int j = s.delimiters.back(); j < int(s.sequence.size()); ++j)
        stream << s.sequence[j] << ",";
      stream << "\n";
      return stream;
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

#endif /* _SOLUTION_HPP_ */
