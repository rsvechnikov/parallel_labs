#ifndef PARALLEL_LABS_BELLMAN_FORD_PTHREAD_NAIVE_H
#define PARALLEL_LABS_BELLMAN_FORD_PTHREAD_NAIVE_H

#include <pthread.h>
#include "graph.h"


template<typename T> requires std::totally_ordered<T>
struct ThreadArgs {
    T max_value;
    Arc<T> *arc;
    T *dist;
};

template<typename T>
requires std::totally_ordered<T>
void *worker_thread(void *arg) {
    auto *args = static_cast<ThreadArgs<T> *>(arg);
    const auto arc = args->arc;
    T *dist = args->dist;
    if (dist[arc->parent] != args->max_value &&
        dist[arc->parent] + arc->weight < dist[arc->child]) {
        dist[arc->child] = dist[arc->parent] + arc->weight;
    }
    pthread_exit(nullptr);
}

template<typename T>
requires std::totally_ordered<T>
std::vector<T> find_shortest_paths(Graph<T> &graph, int start, T max_value) {
    const size_t node_count = graph.get_node_count();

    T dist[node_count];

    for (int i = 0; i < node_count; i++) {
        dist[i] = max_value;
    }
    dist[start] = T();

    auto arcs = graph.get_arcs();
    pthread_t threads[arcs.size()];
    ThreadArgs<T> thread_args[arcs.size()];
    for (int i = 1; i < node_count; i++) {
        for (int j = 0; j < arcs.size(); j++) {
            thread_args[j] = {max_value, &arcs[j], dist};
            pthread_create(&threads[j], nullptr, worker_thread<T>, &thread_args[j]);

        }
        for (pthread_t thread: threads) {
            pthread_join(thread, nullptr);
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


#endif //PARALLEL_LABS_BELLMAN_FORD_PTHREAD_NAIVE_H
