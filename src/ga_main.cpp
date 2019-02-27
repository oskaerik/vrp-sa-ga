#include "ga.hpp"
#include "sa.hpp"

int main() {
    srand(time(NULL));
    /*
    Graph g = {
        {0, 1, sqrt(2), 1},
        {1, 0, 1, sqrt(2)},
        {sqrt(2), 1, 0, 1},
        {1, sqrt(2), 1, 0}
    };
    
    Graph g = {
        {0, 1, sqrt(2), 1, sqrt(2), 1},
        {1, 0, 1, sqrt(2), 1, sqrt(2)},
        {sqrt(2), 1, 0, 1, 2, sqrt(5)},
        {1, sqrt(2), 1, 0, sqrt(5), 2},
        {sqrt(2), 1, 2, sqrt(5), 0, 1},
        {1, sqrt(2), sqrt(5), 2, 1, 0}
    };
    */
    
    int n = 11, m = 3;
    std::ofstream g_file("out/uniform.csv");
    Graph g = uniform_graph(g_file, n);

    auto s = genetic_algorithm(g, m, 'u');
    printf("--- GA solution ---\nScore\t%lf\n", s.score(g));
    s.print();

    // s = simulated_annealing(g, m);
    // printf("--- SA solution ---\nScore\t%lf\n", s.score(g));
    // s.print();
}
