#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using Graph = std::vector<std::vector<double>>;

struct Solution {
    std::vector<int> sequence; // size = n
    std::vector<int> delimiters; // size = m-1
};

Solution random_solution(int n, int m) {
    Solution s = { std::vector<int>(n-1), std::vector<int>(m-1) };

    // Generate random sequence
    for (int i = 0; i < int(s.sequence.size()); ++i)
        s.sequence[i] = i+1;
    std::random_shuffle(s.sequence.begin(), s.sequence.end());

    // Set delimiters randomly
    for (int i = 0; i < int(s.delimiters.size()); ++i)
        s.delimiters[i] = rand() % n;
    std::sort(s.delimiters.begin(), s.delimiters.end());

    return s;
}

void print_solution(Solution &s) {
    printf("Vehicle 0: ");
    for (int j = 0; j < s.delimiters[0]; ++j)
        printf("%d ", s.sequence[j]);
    printf("\n");

    for (int i = 1; i < int(s.delimiters.size()); ++i) {
        printf("Vehicle %d: ", i);
        for (int j = s.delimiters[i-1]; j < s.delimiters[i]; ++j)
            printf("%d ", s.sequence[j]);
        printf("\n");
    }

    printf("Vehicle %d: ", int(s.delimiters.size()));
    for (int j = s.delimiters.back(); j < int(s.sequence.size()); ++j)
        printf("%d ", s.sequence[j]);
    printf("\n");
}

int main() {
    /*
    Graph g = {
        {0, 1, sqrt(2), 1},
        {1, 0, 1, sqrt(2)},
        {sqrt(2), 1, 0, 1},
        {1, sqrt(2), 1, 0}
    };
    */
    Graph g = {
        {0, 1, sqrt(2), 1, sqrt(2), 1},
        {1, 0, 1, sqrt(2), 1, sqrt(2)},
        {sqrt(2), 1, 0, 1, 2, sqrt(5)},
        {1, sqrt(2), 1, 0, sqrt(5), 2},
        {sqrt(2), 1, 2, sqrt(5), 0, 1},
        {1, sqrt(2), sqrt(5), 2, 1, 0}
    };

    int n = g.size(); // nodes
    int m = 3; // vehicles

    for (int i = 0; i < 10; ++i) {
        Solution s = random_solution(n, m);
        print_solution(s);
        printf("\n");
    }
}