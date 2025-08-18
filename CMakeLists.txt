cmake_minimum_required(VERSION 3.22.1)
project(parallel-3d-ground-segmentation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Pangolin_DIR "/usr/local/lib/cmake/Pangolin")

find_package(Pangolin REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    # experiments/experiment_1.cpp
    # experiments/experiment_2.cpp
    # experiments/experiment_3.cpp
    # experiments/experiment_4.cpp
    src/file_reader.cpp
    src/visualizer.cpp
    src/seq_algorithm.cpp
    src/par_algorithm.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE ${Pangolin_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${Pangolin_LIBRARIES}
        Threads::Threads
        OpenGL::OpenGL
        OpenMP::OpenMP_CXX
)