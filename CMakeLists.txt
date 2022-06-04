cmake_minimum_required(VERSION 3.20)
project(parallel_labs)

set(CMAKE_CXX_STANDARD 20)


find_package(OpenMP REQUIRED)

add_executable(bellman_ford
        main.cpp
        graph.h
        bellman_ford_serial.h
        bellman_ford_pthread_naive.h
        bellman_ford_pthread.h
        bellman_ford_openmp.h
        )
target_link_libraries(bellman_ford PRIVATE OpenMP::OpenMP_CXX)

add_executable(bellman_ford_benchmark
        benchmark.cpp
        graph.h
        bellman_ford_serial.h
        bellman_ford_pthread_naive.h
        bellman_ford_pthread.h
        bellman_ford_openmp.h
        )
target_link_libraries(bellman_ford_benchmark PRIVATE OpenMP::OpenMP_CXX)
