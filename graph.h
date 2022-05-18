#ifndef PARALLEL_LABS_GRAPH_H
#define PARALLEL_LABS_GRAPH_H

#include <vector>
#include <filesystem>
#include <fstream>
#include <regex>

namespace fs = std::filesystem;

template<typename T>
struct Arc {
    int parent;
    int child;
    T weight;
};

template<typename T>
class Graph {
public:
    explicit Graph(const fs::path &graph_path) {
        std::ifstream input_file{graph_path};
        if (!input_file.is_open()) {
            throw std::runtime_error("Could not open the graph file");
        }
        std::string line;
        std::regex regex("[\t ]+");
        while (std::getline(input_file, line)) {
            std::vector<std::string> split(
                    std::sregex_token_iterator(line.begin(), line.end(), regex, -1),
                    std::sregex_token_iterator()
            );
            int parent = std::stoi(split[0]);
            int child = std::stoi(split[1]);
            if (child >= node_count) {
                node_count = child + 1;
            }
            std::istringstream ss(split[2]);
            T weight;
            ss >> weight;

            arcs.push_back(Arc<T>{parent, child, weight});
        }
    }

    [[nodiscard]] const std::vector<Arc<T>> &get_arcs() const {
        return arcs;
    };

    [[nodiscard]] size_t get_arc_count() const {
        return arcs.size();
    }

    [[nodiscard]] size_t get_node_count() const {
        return node_count;
    };

    void print_contents() const {
        for (auto &arc : arcs) {
            std::cout << arc.parent << "\t" << arc.child << "\t" << arc.weight << std::endl;
        }
    }

private:
    std::vector<Arc<T>> arcs{};
    size_t node_count = 0;
};

#endif //PARALLEL_LABS_GRAPH_H
