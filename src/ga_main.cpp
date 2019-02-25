#include "ga.hpp"

int main() {
    srand(time(NULL));
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
    assert(n > 2);
    assert(m > 1);
    assert(m < n);

    // Generate a random population
    std::vector<Solution> population(POP_SIZE);
    for (auto &s : population)
        s.randomize(n, m);

    for (int i = 0; i < 10; ++i) {
        printf("--- Solution %d ---\n", i);
        population[i].print();
        population[i].mutate();
        population[i].print();
        printf("\n");
    }
}