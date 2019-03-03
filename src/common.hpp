#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <cmath>
#include <cassert>
#include <cstdio>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <cfloat>
#include <utility>

using namespace std::chrono;
using ms = std::chrono::microseconds;
using Graph = std::vector<std::vector<double>>;
using point = std::pair<double,double>;

double dist(const point &p1, const point &p2) {
  double x = p1.first - p2.first;
  double y = p1.second - p2.second;
  return sqrt(x*x + y*y);
}

// Returns a graph with points distributed
// uniformly in the plane.
Graph uniform_graph(int n) {
  assert(n >= 3);

  // Create a file for graph csv output
  char file_name[64];
  sprintf(file_name, "./out/graph_u_n%d.csv", n);
  std::ofstream file(file_name);
  file << "x,y\n";

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0, 100); // Random uniform double distribution in [0, 100)

  // Genereate points uniformly
  std::vector<point> nodes(n);
  for(int i = 0; i < n; i++) {
    double x = dis(gen);
    double y = dis(gen);
    nodes[i] = { x, y };
    file << x << ',' << y << '\n';
  }

  // Create a graph with edge weights equal
  // to euclidian distance between the nodes
  Graph graph(n, std::vector<double>(n));
  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++)
      graph[i][j] = dist(nodes[i], nodes[j]);
  return graph;
}

// Returns a graph with points normally distributed
// around clusters. Clusters are uniformly distributed
// in the plane.
Graph clustered_graph(int n, int c, int sd) {
  assert(n >= 3);
  assert(c >= 2);

  // Create a file for graph csv output
  char file_name[64];
  sprintf(file_name, "./out/graph_c_n%d_c%d_sd%d.csv", n, c, sd);
  std::ofstream file(file_name);
  file << "x,y\n";

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> cluster_dis(0, 100); // Random uniform double distribution in [0, 100)
  std::normal_distribution<> node_dis(0, sd);

  // Generate all cluster points
  std::vector<point> clusters(c);
  for (int i = 0; i < c; ++i)
    clusters[i] = { cluster_dis(gen), cluster_dis(gen) };

  // Generate the nodes with offsets from the clusters
  std::vector<point> nodes(n);
  for (int i = 0; i < n; ++i) {
    auto cluster = clusters[rand() % c];
    double x = cluster.first + node_dis(gen);
    double y = cluster.second + node_dis(gen);
    nodes[i] = { x, y };
    file << x << ',' << y << '\n';
  }

  // Create a graph with edge weights equal
  // to euclidian distance between the nodes
  Graph graph(n, std::vector<double>(n));
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      graph[i][j] = dist(nodes[i], nodes[j]);
  return graph;
}

// Returns a mix of uniform and clustered graph.
// Half of the nodes are uniformly distributed,
// the other half are normally distributed around cluster points.
Graph clustered_and_uniform_graph(int n, int c, int sd) {
  assert(n >= 3);
  assert(c >= 2);

  // Create a file for graph csv output
  char file_name[64];
  sprintf(file_name, "./out/graph_m_n%d_c%d_sd%d.csv", n, c, sd);
  std::ofstream file(file_name);
  file << "x,y\n";

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0, 100); // Random uniform double distribution in [0, 100)
  std::normal_distribution<> node_dis(0, sd);

  // Generate all cluster points
  std::vector<point> clusters(c);
  for (int i = 0; i < c; ++i)
    clusters[i] = { dis(gen), dis(gen) };

  std::vector<point> nodes(n);

  // Generate half of the nodes around the clusters
  int i = 0;
  for (; i < n/2; ++i) {
    auto cluster = clusters[rand() % c];
    double x = cluster.first + node_dis(gen);
    double y = cluster.second + node_dis(gen);
    nodes[i] = { x, y };
    file << x << ',' << y << '\n';
  }

  // Genereate the other half uniformly
  for (; i < n; ++i) {
    double x = dis(gen);
    double y = dis(gen);
    nodes[i] = { x, y };
    file << x << ',' << y << '\n';
  }

  // Create a graph with edge weights equal
  // to euclidian distance between the nodes
  Graph graph(n, std::vector<double>(n));
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      graph[i][j] = dist(nodes[i], nodes[j]);
  return graph;
}

auto ms_since(const high_resolution_clock::time_point &t0) {
  auto t1 = high_resolution_clock::now();
  auto time_span = duration_cast<ms>(t1 - t0);
  return time_span.count();
}

#endif /* _COMMON_HPP_ */
