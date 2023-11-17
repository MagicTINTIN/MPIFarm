cmake_minimum_required(VERSION 3.12)
project(MPIFarm)

set(CMAKE_CXX_STANDARD 20)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

# Create a library for imageprocess.cpp
add_library(ImageProcessLib imageprocess.cpp)

# Link the library to the main executable
add_executable(MPIFarm main.cpp)
target_link_libraries(MPIFarm ImageProcessLib ${OpenCV_LIBS})

# Link MPI library
target_link_libraries(MPIFarm ${MPI_CXX_LIBRARIES})