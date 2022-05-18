#ifndef PARALLEL_LABS_BELLMAN_FORD_SERIAL_H
#define PARALLEL_LABS_BELLMAN_FORD_SERIAL_H

#include "graph.h"

template<typename T>
requires std::totally_ordered<T>
std::vector<T> find_shortest_paths(Graph<T> &graph, int start, T max_value) {
    const size_t node_count = graph.get_node_count();

    T dist[node_count];

    for (int i = 0; i < node_count; i++) {
        dist[i] = max_value;
    }
    dist[start] = T();

    for (int i = 1; i < node_count; i++) {
        for (auto arc: graph.get_arcs()) {
            if (dist[arc.parent] != max_value && dist[arc.parent] + arc.weight < dist[arc.child]) {
                dist[arc.child] = dist[arc.parent] + arc.weight;
            }
        }
    }

    for (auto arc: graph.get_arcs()) {
        if (dist[arc.parent] != max_value && dist[arc.parent] + arc.weight < dist[arc.child]) {
            printf("Graph contains negative weight cycle\n");
            return {};
        }
    }

    return {dist, dist + node_count};
}

#endif //PARALLEL_LABS_BELLMAN_FORD_SERIAL_H
