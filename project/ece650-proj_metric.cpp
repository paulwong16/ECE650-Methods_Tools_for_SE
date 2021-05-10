//
// Created by 王智杰 on 2020/11/26.
//

// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <algorithm>
#include <random>
#include "graph.h"

pthread_t pid1, pid_timer;

struct args {
    int graph_size;
    std::vector<std::vector<int>> adj_list;
    std::vector<std::pair<int, int>> edges;
    std::vector<int> solution;
    long runtime;
};

void* timer(void* argp) {
    sleep(1);
    pthread_cancel(pid1);
    return nullptr;
}

void* solve_vertex_cover(void* argp) {
    clockid_t cid;
    pthread_getcpuclockid(pthread_self(), &cid);
    struct timespec start;
    clock_gettime(cid, &start);
    auto* arg = (struct args*)argp;
    if (arg->edges.empty()) {
        arg->solution = std::vector<int> {};
    }
    auto* solver = new Minisat::Solver;
    int success = -1;
    std::vector<int> solution;
    pthread_create(&pid_timer, nullptr, &timer, nullptr);
    for (int i = 1; i < arg->graph_size; i++) {
        pthread_testcancel();
        // create variables
        for (int j = 0; j < arg->graph_size; j++) {
            for (int k = 0; k < i; k++) {
                solver->newVar();
            }
        }

        // at least one vertex is the ith vertex in the vertex cover
        for (int j = 0; j < i; j++) {
            Minisat::vec<Minisat::Lit> clause;
            for (int k = 0; k < arg->graph_size; k++) {
                clause.push(Minisat::mkLit(k * i + j));
            }
            solver->addClause(clause);
        }

        // no one vertex can appear twice in a vertex cover
        for (int j = 0; j < arg->graph_size; j++) {
            for (int k = 0; k < i; k++) {
                for (int l = 0; l < k; l++) {
                    solver->addClause(~Minisat::mkLit(j * i + l), ~Minisat::mkLit(j * i + k));
                }
            }
        }

        // no more than one vertex appears in the mth position of vertex cover
        for (int j = 0; j < i; j ++) {
            for (int k = 0; k < arg->graph_size; k++) {
                for (int l = 0; l < k; l++) {
                    solver->addClause(~Minisat::mkLit(l * i + j), ~Minisat::mkLit(k * i + j));
                }
            }
        }

        // every edge is incident to at least one vertex in the vertex cover
        for (auto edge : arg->edges) {
            int start = edge.first;
            int end = edge.second;
            Minisat::vec<Minisat::Lit> clause;
            for (int j = 0; j < i; j++) {
                clause.push(Minisat::mkLit(start * i + j));
                clause.push(Minisat::mkLit(end * i + j));
            }
            solver->addClause(clause);
        }

        if (solver->solve()) {
            success = i;
            break;
        }
        else {
            delete solver;
            solver = new Minisat::Solver();
        }
    }

    for (int i = 0; i < arg->graph_size; i++) {
        for (int j = 0; j < success; j++) {
            if (solver->modelValue(i * success + j) == Minisat::l_True) {
                solution.push_back(i);
            }
        }
    }
    std::sort(solution.begin(), solution.end());
    arg->solution = solution;
    pthread_cancel(pid_timer);
    struct timespec end;
    clock_gettime(cid, &end);
    arg->runtime = end.tv_nsec - start.tv_nsec;
    return nullptr;
}

void* solve_vertex_cover_1(void* argp) {
    clockid_t cid;
    pthread_getcpuclockid(pthread_self(), &cid);
    struct timespec start;
    clock_gettime(cid, &start);
    auto* arg = (struct args*)argp;
    std::vector<std::vector<int>> adj_list_copy = arg->adj_list;
    std::vector<int> solution;
    int graph_size_now = -1;
    while (graph_size_now != 0) {
        graph_size_now = 0;
        int highest_degree = 0;
        int vertex_to_throw = -1;
        for (int i = 0; i < arg->graph_size; i++) {
            if (adj_list_copy[i].size() > highest_degree) {
                highest_degree = adj_list_copy[i].size();
                vertex_to_throw = i;
            }
        }
        adj_list_copy[vertex_to_throw].clear();
        for (int i = 0; i < arg->graph_size; i++) {
            for (int j = 0; j < adj_list_copy[i].size(); j++) {
                if (adj_list_copy[i][j] == vertex_to_throw) {
                    adj_list_copy[i].erase(adj_list_copy[i].begin() + j);
                }
            }
            graph_size_now += adj_list_copy[i].size();
        }
        if (vertex_to_throw > -1) solution.push_back(vertex_to_throw);
    }
    std::sort(solution.begin(), solution.end());
    arg->solution = solution;
    struct timespec end;
    clock_gettime(cid, &end);
    arg->runtime = end.tv_nsec - start.tv_nsec;
    return nullptr;
}

void* solve_vertex_cover_2(void* argp) {
    clockid_t cid;
    pthread_getcpuclockid(pthread_self(), &cid);
    struct timespec start;
    clock_gettime(cid, &start);
    auto* arg = (struct args*)argp;
    std::vector<std::pair<int, int>> edges_copy = arg->edges;
    std::vector<int> solution;
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(edges_copy), std::end(edges_copy), rng);
    while (!edges_copy.empty()) {
        std::pair<int, int> edge = edges_copy.back();
        edges_copy.pop_back();
        int edges_copy_size = edges_copy.size();
        for (int i = edges_copy_size - 1; i >= 0; i--) {
            if ((edges_copy[i].first == edge.first) || (edges_copy[i].first == edge.second) ||
                (edges_copy[i].second == edge.first) || (edges_copy[i].second == edge.second)) {
                edges_copy.erase(edges_copy.begin() + i);
            }
        }
        solution.push_back(edge.first);
        solution.push_back(edge.second);
    }
    std::sort(solution.begin(), solution.end());
    arg->solution = solution;
    struct timespec end;
    clock_gettime(cid, &end);
    arg->runtime = end.tv_nsec - start.tv_nsec;
    return nullptr;
}

template <class T>
void get_result(const std::vector<T>& r) {
    double mean = 0;
    double sd = 0;
    for (auto ele : r) {
        mean += ele;
    }
    mean /= r.size();
    for (auto ele : r) {
        sd += pow(ele - mean, 2);
    }
    sd = sqrt(sd / r.size());
    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Standard Deviation: " << sd << std::endl;
}

int main(int argc, char** argv) {
    graph G;
    std::ifstream file("../output_graph2");
    std::string line;
    int last_v = 0;
    std::vector<long> r1, r2, r3;
    std::vector<double> a1, a2;
    // read from stdin until EOF
    while (!std::cin.eof() && std::getline(file, line)) {
        // read a line of input until EOL and store in a string

//        std::getline(std::cin, line);

        // create an input stream based on the line
        // we will use the input stream to parse the line
        std::istringstream input(line);
        // while there are characters in the input line
        if (std::cin.eof()) {
            break;
        }
        char operation;
        input >> operation;
        if (operation == 'V') {
            int v;
            input >> v;
            if ((last_v != 0) && (v != last_v)) {
                std::cout << "graph size: " << last_v << std::endl;
                get_result(r1);
                get_result(r2);
                get_result(r3);
                get_result(a1);
                get_result(a2);
                r1.clear();
                r2.clear();
                r3.clear();
                a1.clear();
                a2.clear();
            }
            last_v = v;
            G.set_graph(v);
        }
        else if (operation == 'E') {
            if (G.vertex_initialized && !G.edge_initialized) {
                char separator;
                input >> separator;
                input >> separator;
                if (separator == '}') {
                    G.edge_initialized = true;
                    break;
                }
                std::vector<int> edge;
                int vertex;
                while (!input.eof()) {
                    input >> vertex;
                    edge.push_back(vertex);
                    input >> separator;
                    input >> vertex;
                    edge.push_back(vertex);
                    input >> separator;
                    input >> separator;
                    input >> separator;
                }
                for (std::size_t i = 0; i < edge.size(); i+=2){
                    if (!G.add_edge(edge[i], edge[i+1])){
                        G.reset_edges();
                        break;
                    }
                    else {
                        G.edge_initialized = true;
                    }
                }
            }
            else if (G.edge_initialized) {
                std::cerr << "Error: Edges have been specified" << std::endl;
            }
            else {
                std::cerr << "Error: Need to input vertex number first." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Unknown command." << std::endl;
        }
        if (G.vertex_initialized && G.edge_initialized) {
            for (int t = 0; t < 10; t ++) {
                std::vector<int> s1, s2, s3;
                int graph_size = G.get_graph_size();
                std::vector<std::vector<int>> adj_list = G.get_adj_list();
                std::vector<std::pair<int, int>> edges = G.get_edges();
                pthread_t pid2, pid3;
                args *arg1 = new args{graph_size, adj_list, edges, s1};
                args *arg2 = new args{graph_size, adj_list, edges, s2};
                args *arg3 = new args{graph_size, adj_list, edges, s3};
                pthread_create(&pid1, nullptr, &solve_vertex_cover, arg1);
                pthread_create(&pid2, nullptr, &solve_vertex_cover_1, arg2);
                pthread_create(&pid3, nullptr, &solve_vertex_cover_2, arg3);
                pthread_join(pid1, nullptr);
                pthread_join(pid2, nullptr);
                pthread_join(pid3, nullptr);
//                std::cout << "CNF-SAT-VC: " << std::endl;
                if (arg1->solution.empty()) {
//                    std::cout << "timeout" << std::endl;
                    r1.push_back(1 * 1000000);
                } else {
//                    for (int i = 0; i < arg1->solution.size() - 1; i++) {
//                        std::cout << arg1->solution[i] + 1 << ",";
//                    }
//                    std::cout << arg1->solution[arg1->solution.size() - 1] + 1 << std::endl;
//                    std::cout << arg1->runtime.tv_nsec / 1000 << " us" << std::endl;
                    r1.push_back(arg1->runtime / 1000);
                }
//                std::cout << "APPROX-VC-1: " << std::endl;
//                for (int i = 0; i < arg2->solution.size() - 1; i++) {
//                    std::cout << arg2->solution[i] + 1 << ",";
//                }
//                std::cout << arg2->solution[arg2->solution.size() - 1] + 1 << std::endl;
//                std::cout << arg2->runtime.tv_nsec / 1000 << " us" << std::endl;
                r2.push_back(arg2->runtime / 1000);
                if (!arg1->solution.empty()) {
                    a1.push_back((double)arg2->solution.size() / (double)arg1->solution.size());
                }
//                std::cout << "APPROX-VC-2: " << std::endl;
//                for (int i = 0; i < arg3->solution.size() - 1; i++) {
//                    std::cout << arg3->solution[i] + 1 << ",";
//                }
//                std::cout << arg3->solution[arg3->solution.size() - 1] + 1 << std::endl;
//                std::cout << arg3->runtime.tv_nsec / 1000 << " us" << std::endl;
                r3.push_back(arg3->runtime / 1000);
                if (!arg1->solution.empty()) {
                    a2.push_back((double)arg3->solution.size() / (double)arg1->solution.size());
                }
            }
        }
    }
    std::cout << "graph size: " << last_v << std::endl;
    get_result(r1);
    get_result(r2);
    get_result(r3);
    get_result(a1);
    get_result(a2);
}
