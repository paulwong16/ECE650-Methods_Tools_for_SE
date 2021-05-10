//
// Created by Zhijie Wang on 2020/10/5.
// Edited by Zhijie Wang on 2020/11/11
//

#ifndef ECE650_A4_GRAPH_H
#define ECE650_A4_GRAPH_H
#include <vector>
#include <queue>

#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"


class graph {
private:
    int graph_size;
    std::vector<std::vector<int>> adj_list;
    std::vector<std::pair<int, int>> edges;
    bool bfs_search(int src, int dest, std::vector<int>& d, std::vector<int>& pi);
    static void get_path(int src, int dest, std::vector<int>& pi, std::vector<int>& path);

public:
    graph();
    explicit graph(int v);
    void set_graph(int v);
    void reset_edges();
    bool add_edge(int vertex_1, int vertex_2);
    int get_graph_size();
    std::vector<std::vector<int>> get_adj_list();
    std::vector<std::pair<int, int>> get_edges();
    void print_graph();
    void search_shortest_path(int src, int dest);
    bool vertex_initialized = false;
    bool edge_initialized = false;
};


#endif //ECE650_A2_GRAPH_H
