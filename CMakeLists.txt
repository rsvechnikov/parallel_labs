cmake_minimum_required(VERSION 3.20)
project(parallel_labs)

set(CMAKE_CXX_STANDARD 20)


find_package(OpenMP REQUIRED)

add_executable(parallel_labs
        main.cpp
        graph.h
        bellman_ford_serial.h
        bellman_ford_pthread_naive.h
        bellman_ford_pthread.h
        bellman_ford_openmp.h
        )
target_link_libraries(parallel_labs PRIVATE OpenMP::OpenMP_CXX)
