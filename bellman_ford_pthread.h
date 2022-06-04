#ifndef PARALLEL_LABS_BELLMAN_FORD_PTHREAD_H
#define PARALLEL_LABS_BELLMAN_FORD_PTHREAD_H

#include <pthread.h>
#include <cmath>
#include "graph.h"

constexpr int THREAD_NUM = 8;

template<typename T> requires std::totally_ordered<T>
struct ThreadArgs {
    int thread_id;
    T max_value;
    std::vector<Arc<T>> *arcs;
    T *dist;
};

template<typename T>
requires std::totally_ordered<T>
void *worker_thread(void *arg) {
    auto *args = static_cast<ThreadArgs<T> *>(arg);
    const auto arcs = args->arcs;
    T *dist = args->dist;

    const int tid = args->thread_id;
    const int chunk_size = static_cast<int>(std::ceil(
            arcs->size() / static_cast<float>(THREAD_NUM)));
    const int chunk_start = tid * chunk_size;
    const int chunk_end = tid == THREAD_NUM - 1 ? arcs->size() : chunk_start + chunk_size;

    for (int i = chunk_start; i < chunk_end; i++) {
        const auto &arc = arcs->at(i);
        if (dist[arc.parent] != args->max_value &&
            dist[arc.parent] + arc.weight < dist[arc.child]) {
            dist[arc.child] = dist[arc.parent] + arc.weight;
        }
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
    pthread_t threads[THREAD_NUM];
    ThreadArgs<T> thread_args[THREAD_NUM];
    for (int i = 1; i < node_count; i++) {
        for (int j = 0; j < THREAD_NUM; j++) {
            thread_args[j] = {j, max_value, &arcs, dist};
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

#endif //PARALLEL_LABS_BELLMAN_FORD_PTHREAD_H
