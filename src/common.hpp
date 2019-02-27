#ifndef _COMMON_HPP_
#define _COMMON_HPP_

#include <cmath>
#include <cassert>
#include <cstdio>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <chrono>

using namespace std::chrono;
using ms = std::chrono::microseconds;

using Graph = std::vector<std::vector<double>>;

Graph uniform_graph(int n){
  assert(n >= 3);
  char file_name[64];
  sprintf(file_name, "./out/graph_u_n%d.csv", n);
  std::ofstream file(file_name);
  file << "x,y\n";

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0, 100); // Random uniform double distribution in [0, 10)

  std::vector<std::pair<double,double>> nodes(n);
  for(int i = 0; i < n; i++) {
    double x = dis(gen);
    double y = dis(gen);
    nodes[i] = { x, y };
    file << x << ", " << y << '\n';
  }

  Graph graph(n, std::vector<double>(n));
  for(int i = 0; i < n; i++)
    for(int j = 0; j < n; j++) {
      double x = nodes[i].first - nodes[j].first;
      double y = nodes[i].second - nodes[j].second;
      graph[i][j] = sqrt(x*x + y*y);
    }
  return graph;
}

Graph clustered_graph(int n, int c, int sd) {
  assert(n >= 3);
  assert(c >= 2);
  char file_name[64];
  sprintf(file_name, "./out/graph_c_n%d_c%d_sd%d.csv", n, c, sd);
  std::ofstream file(file_name);
  file << "x,y\n";

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> cluster_dis(0, 100); // Random uniform double distribution in [0, 10)
  std::normal_distribution<> node_dis(0, sd);

  std::vector<std::pair<double,double>> clusters(c);
  for (int i = 0; i < c; ++i)
    clusters[i] = { cluster_dis(gen), cluster_dis(gen) };

  std::vector<std::pair<double,double>> nodes(n);
  for (int i = 0; i < n; ++i) {
    auto cluster = clusters[rand() % c];
    double x = cluster.first + node_dis(gen);
    double y = cluster.second + node_dis(gen);
    nodes[i] = { x, y };
    file << x << ", " << y << '\n';
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

Graph clustered_and_uniform_graph(int n, int c, int sd) {
  assert(n >= 3);
  assert(c >= 2);
  char file_name[64];
  sprintf(file_name, "./out/graph_m_n%d_c%d_sd%d.csv", n, c, sd);
  std::ofstream file(file_name);
  file << "x,y\n";

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0, 100); // Random uniform double distribution in [0, 10)
  std::normal_distribution<> node_dis(0, sd);

  std::vector<std::pair<double,double>> clusters(c);
  for (int i = 0; i < c; ++i)
    clusters[i] = { dis(gen), dis(gen) };

  std::vector<std::pair<double,double>> nodes(n);
  int i = 0;
  for (; i < n/2; ++i) {
    auto cluster = clusters[rand() % c];
    double x = cluster.first + node_dis(gen);
    double y = cluster.second + node_dis(gen);
    nodes[i] = { x, y };
    file << x << ", " << y << '\n';
  }
  for (; i < n; ++i) {
    double x = dis(gen);
    double y = dis(gen);
    nodes[i] = { x, y };
    file << x << ", " << y << '\n';
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

auto ms_since(const high_resolution_clock::time_point &t0) {
  auto t1 = high_resolution_clock::now();
  auto time_span = duration_cast<ms>(t1 - t0);
  return time_span.count();
}

#endif
