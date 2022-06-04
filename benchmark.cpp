#include <iostream>
#include <chrono>
#include <limits>
#include <numeric>

#define MODE_OPENMP

#ifdef MODE_SERIAL
    #include "bellman_ford_serial.h"
    #define MODE_NAME "serial"
#elif defined MODE_PTHREAD_NAIVE
    #include "bellman_ford_pthread_naive.h"
    #define MODE_NAME "pthread_naive"
#elif defined MODE_PTHREAD
    #include "bellman_ford_pthread.h"
    #define MODE_NAME "pthread"
#elif defined MODE_OPENMP
    #include "bellman_ford_openmp.h"
    #define MODE_NAME "openmp"
#endif

int main() {
    std::vector<std::string> graph_names = {"smoke", "small", "medium"};
    std::unordered_map<std::string, double> results {
            {"smoke", 0.0f},
            {"small", 0.0f},
            {"medium", 0.0f},
//            {"huge", 0.0f},
    };

    for (const auto &graph_name: graph_names) {
        std::printf("Testing %s graph\n", graph_name.c_str());
        Graph<double> graph{"../graphs/" + graph_name + ".txt"};
        std::vector<double> timing_results(10);
        for (int i = 0; i < 10; i++) {
            auto t1 = std::chrono::high_resolution_clock::now();
            auto result = find_shortest_paths<double>(
                    graph, 0, std::numeric_limits<double>::max());
            auto t2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = t2 - t1;
            timing_results[i] = elapsed.count();
        }
        results[graph_name] = std::accumulate(timing_results.begin(), timing_results.end(), 0.0) /
                              static_cast<double>(timing_results.size());
        std::printf("Done.\n");
    }
    std::printf("All done. Congratulations!\n\n");

    std::printf("%s results\n", MODE_NAME);
    for (const auto &[graph_name, time]: results) {
        std::printf("\"%s\": %f\n", graph_name.c_str(), time);
    }
    return 0;
}