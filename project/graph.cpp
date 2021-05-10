//
// Created by Zhijie Wang on 2020/10/5.
//

#include "graph.h"
#include <iostream>
#include <limits>
#include <algorithm>

/*!
 * Default constructor.
 */
graph::graph() {
    graph_size = -1;
}

/*!
 * Constructor with vertex num (only used in debugging)
 * @param v vertex number
 */
graph::graph(int v) {
    graph_size = v;
    adj_list = std::vector<std::vector<int>> (graph_size);
    vertex_initialized = true;
}

/*!
 * Public function to add an edge
 * @param vertex_1
 * @param vertex_2
 * @return
 */
bool graph::add_edge(int vertex_1, int vertex_2) {
    if ((vertex_1 > graph_size) || (vertex_2 > graph_size) || (vertex_1 <= 0) || (vertex_2 <= 0)) {
        std::cerr<< "Error: vertex index out of range." << std::endl;
        return false;
    }
    if (vertex_1 == vertex_2) {
        adj_list[vertex_1 - 1].push_back(vertex_1 - 1);
        return true;
    }
    adj_list[vertex_1 - 1].push_back(vertex_2 - 1);
    adj_list[vertex_2 - 1].push_back(vertex_1 - 1);
    edges.push_back(std::pair<int, int> (vertex_1 -1, vertex_2 -1));
    return true;
}

/*!
 * Print the adjacency list (only used in debugging)
 */
void graph::print_graph() {
    for (int i = 0; i < graph_size; i++) {
        std::cout << i + 1 << ": ";
        for (auto element : adj_list[i]) {
            std::cout << element + 1 << ' ';
        }
        std::cout << std::endl;
    }
}

/*!
 * Private function of BFS.
 * @param src source
 * @param dest destination
 * @param d distance matrix
 * @param pi forward matrix
 * @return
 */
bool graph::bfs_search(int src, int dest, std::vector<int>& d, std::vector<int>& pi) {
    std::vector<bool> is_visited(graph_size, false);
    for (int i = 0; i < graph_size; i++) {
        d[i] = std::numeric_limits<int>::max();
        pi[i] = -1;
    }
    d[src] = 0;
    is_visited[src] = true;
    std::queue<int> candidates;
    candidates.push(src);
    while (!candidates.empty()) {
        int v = candidates.front();
        candidates.pop();
        for (auto u : adj_list[v]) {
            if (!is_visited[u]) {
                is_visited[u] = true;
                d[u] = d[v] + 1;
                pi[u] = v;
                candidates.push(u);
            }
            if (u == dest) {
                // Handle the issue of start = end
                if (u == src) {
                    pi[u] = v;
                }
                return true;
            }
        }
    }

    return false;
}

/*!
 * Public function of searching shortest path
 * @param src source
 * @param dest destination
 */
void graph::search_shortest_path(int src, int dest) {
//    int d[graph_size];
//    int pi[graph_size];
    std::vector<int> d(graph_size);
    std::vector<int> pi(graph_size);
    if ((src > graph_size) || (dest > graph_size) || (src <= 0) || (dest <= 0)) {
        std::cerr << "Error: vertex index out of range." << std::endl;
        return;
    }
    if (bfs_search(src - 1, dest - 1, d, pi)) {
        std::vector<int> path;
        get_path(src - 1, dest - 1, pi, path);
        for (std::size_t i = 0; i < path.size(); i ++) {
            std::cout << path[path.size() - i - 1] << '-';
        }
        std::cout << dest << std::endl;
    }
    else {
        std::cerr << "Error: No shortest path exists." << std::endl;
    }
}

/*!
 * Private function of getting a shortest path.
 * @param dest destination
 * @param pi forward matrix
 * @param path
 */
void graph::get_path(int src, int dest, std::vector<int>& pi, std::vector<int>& path) {
    int v = dest;
    while (pi[v] != src) {
        path.push_back(pi[v] + 1);
        v = pi[v];
    }
    path.push_back(pi[v] + 1);
}

/*!
 * Public function to set graph size and initialize adjacency list.
 * @param v
 */
void graph::set_graph(int v) {
    graph_size = v;
    adj_list = std::vector<std::vector<int>> (graph_size);
    edges.clear();
    vertex_initialized = true;
    edge_initialized = false;
}

/*!
 * Public funtion to reset the adjacency list.
 */
void graph::reset_edges() {
    adj_list = std::vector<std::vector<int>> (graph_size);
    edges.clear();
    edge_initialized = false;
}

int graph::get_graph_size() {
    return graph_size;
}

std::vector<std::vector<int>> graph::get_adj_list() {
    return adj_list;
}

std::vector<std::pair<int, int>> graph::get_edges() {
    return edges;
}
