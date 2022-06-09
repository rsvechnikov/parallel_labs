#include <iostream>
#include <limits>

#define MODE_OPENMP

#ifdef MODE_SERIAL
    #include "bellman_ford_serial.h"
#elif defined MODE_PTHREAD_NAIVE
    #include "bellman_ford_pthread_naive.h"
#elif defined MODE_PTHREAD_SIMPLE
    #include "bellman_ford_pthread_simple.h"
#elif defined MODE_PTHREAD
    #include "bellman_ford_pthread.h"
#elif defined MODE_OPENMP
    #include "bellman_ford_openmp.h"
#endif

int main() {
    Graph<double> graph{"../graphs/medium.txt"};

    auto result = find_shortest_paths<double>(
            graph, 0, std::numeric_limits<double>::max());

    std::printf("Result:\n");
    for (int i = 0; i < result.size(); ++i) {
        if (result[i] != std::numeric_limits<double>::max()) {
            std::printf("%i -> %i: %f\n", 0, i, result[i]);
        }
        if (i > 1000) {
            break;
        }
    }
    return 0;
}
