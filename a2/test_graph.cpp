//
// Created by Zhijie Wang on 2020/10/5.
//

#include "graph.h"
#include <iostream>
#include <vector>
#include <map>

int main() {
    int V = 15;
    graph G(V);
//    std::vector<std::vector<int>> edges{{2, 6}, {2, 8}, {2, 5}, {6, 5}, {5, 8}, {6, 10}, {10, 8}};
    std::vector<std::vector<int>> edges{{1, 3}, {3, 2}, {3, 4}, {4, 5}, {5, 2}};
    for (auto edge : edges) {
        G.add_edge(edge[0] - 1, edge[1] - 1);
    }
    G.search_shortest_path(5, 1);
    return 0;
}