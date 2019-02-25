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

    //printf("--- Final solution ---\n");
    //genetic_algorithm(g, 3).print();
    Solution s = {{1,4,5,3,2}, {0,3}};
    s.print();
    printf("Score: %lf\n", s.score(g));
}