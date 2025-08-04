cmake_minimum_required(VERSION 3.22.1)
project(parallel-3d-ground-segmentation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Pangolin_DIR "/usr/local/lib/cmake/Pangolin")
find_package(Pangolin REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/file_reader.cpp
    src/visualizer.cpp
    src/seq_algorithm.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${Pangolin_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${Pangolin_LIBRARIES}
        Threads::Threads
        OpenGL::OpenGL
)