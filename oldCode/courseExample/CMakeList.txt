cmake_minimum_required(VERSION 3.12)
project(parallel_average)

set(CMAKE_CXX_STANDARD 17)
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

add_executable(parallel_average main.cpp)