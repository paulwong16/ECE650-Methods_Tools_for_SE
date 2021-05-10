// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include "graph.h"

int main(int argc, char** argv) {
    graph G;

    // read from stdin until EOF
    while (!std::cin.eof()) {
        // read a line of input until EOL and store in a string
        std::string line;
        std::getline(std::cin, line);

        // create an input stream based on the line
        // we will use the input stream to parse the line
        std::istringstream input(line);
        if (line[0] != 's') {
            std::cout << line << std::endl;
        }
        // while there are characters in the input line
        if (std::cin.eof()) {
            break;
        }
        char operation;
        input >> operation;
        if (operation == 'V') {
            int v;
            input >> v;
            G.set_graph(v);
        }
        else if (operation == 'E') {
            if (G.vertex_initialized && !G.edge_initialized) {
                char separator;
                input >> separator;
                input >> separator;
                if (separator == '}') {
                    G.edge_initialized = true;
                    continue;
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
        else if (operation == 's') {
            if (G.vertex_initialized) {
                int start;
                int end;
                input >> start;
                input >> end;
                G.search_shortest_path(start, end);
            }
            else {
                std::cerr << "Error: Need to create a graph first (input vertex number)." << std::endl;
            }
        }
        else {
            std::cerr << "Error: Unknown command." << std::endl;
        }
    }
}
