#include <iostream>
#include <chrono>
#include <limits>

#define MODE_SERIAL

#ifdef MODE_SERIAL
    #include "bellman_ford_serial.h"
#else
    #include "bellman_ford_openmp.h"
#endif

int main() {
    Graph<double> graph{"../graphs/medium.txt"};
//    std::printf("Graph:\n");
//    graph.print_contents();

    auto t1 = std::chrono::high_resolution_clock::now();
    auto result = find_shortest_paths<double>(
            graph, 0, std::numeric_limits<double>::max());
    auto t2 = std::chrono::high_resolution_clock::now();

    std::printf("Result:\n");
    for (int i = 0; i < result.size(); ++i) {
        if (result[i] != std::numeric_limits<double>::max()) {
            std::printf("%i -> %i: %f\n", 0, i, result[i]);
        }
        if (i > 1000) {
            break;
        }
    }
    std::chrono::duration<double, std::milli> elapsed = t2 - t1;
    std::printf("Finished in %f ms.\n", elapsed);
    return 0;
}
