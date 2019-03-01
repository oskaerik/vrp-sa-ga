# Comparing Simulated Annealing and Genetic Algorithm for a Vehicle Routing Problem

In this project, we investigate a Vehicle Routing Problem (VRP). This is an
NP-hard problem, which means that it is problematic to find the optimal
solution for large test cases, therefore we use the two metaheuristics
Simulated Annealing (SA) and Genetic Algorithm (GA) to find an approximate
solution.

The VRP was formulated as an optimization problem and the optimal solution was
found for some small test cases using exhaustive search. SA and GA was
benchmarked against these optimal solutions. For larger test cases, we
benchmarked SA and GA against each other without looking the actual optimal
solution.

Our results were that SA performed better in all our experiments, both in terms
of time and average achieved score.

Our results are included in the `results/` directory, the plots can be viewed
in the `plot.ipynb` notebook.

## How to run

The test suite can be run by executing `make test` in the `src/` directory.
