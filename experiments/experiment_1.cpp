#include "file_reader.hpp"
#include "visualizer.hpp"
#include "seq_algorithm.hpp"
#include "par_algorithm.hpp"

#include <random>
#include <algorithm>
#include <numeric>

int main() {
    std::string coordinates_file = "/home/alessio/workspace/tum/par-comp/"
        "data-parallel-3d-ground-segmentation/labeled_pc/04/velodyne/000056.bin";

    std::string labels_file = "/home/alessio/workspace/tum/par-comp/"
        "data-parallel-3d-ground-segmentation/labeled_pc/04/labels/000056.label";

    const float dividing_factor = 1;
    const int number_of_threads = 16;

    std::vector<Point> original_lidar_data;
    std::vector<Point> sequential_reduced_lidar_data;
    std::vector<Point> parallel_reduced_lidar_data;

    // getting the full data into original_lidar_data
    ReadLiDARdata readLiDAR(coordinates_file, labels_file);
    readLiDAR.read_data(original_lidar_data);

    // number of points to keep
    const int n_total_points = original_lidar_data.size();
    const int n_reduced_points = static_cast<int>(n_total_points * dividing_factor);

    // generating a vector of indices of reduced size
    std::vector<int> indices(n_total_points);
    std::iota(indices.begin(), indices.end(), 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::shuffle(indices.begin(), indices.end(), gen);
    indices.resize(n_reduced_points);
    sequential_reduced_lidar_data.resize(n_reduced_points);

    for (int i = 0; i < n_reduced_points; ++i) {
        sequential_reduced_lidar_data[i] = original_lidar_data[indices[i]];
    }

    parallel_reduced_lidar_data = sequential_reduced_lidar_data;

    std::cout << "N_THREADS = " << number_of_threads << std::endl;
    std::cout << "Number of original points: " << n_total_points << std::endl;
    std::cout << "Sequential Reduced number of points: " << sequential_reduced_lidar_data.size() << std::endl;
    std::cout << "Parallel Reduced number of points: " << parallel_reduced_lidar_data.size() << std::endl;


    double sequential_duration = 0.0;
    double parallel_duration = 0.0;

    Sequential sequential_warmup;
    Sequential sequential_test;

    Parallel parallel_warmup(number_of_threads);
    Parallel parallel_test(number_of_threads);

    int n_warmup = 5;
    int n_test = 20;

    // warming up for the sequential
    for (int i = 0; i < n_warmup; ++i) {
        std::vector<Point> tmp_copy = sequential_reduced_lidar_data;
        sequential_warmup.seq_point_clustering(tmp_copy, 50, 14, readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
    }

    // timing the sequential functions
    for (int i = 0; i < n_test; ++i) {
        std::vector<Point> tmp_copy = sequential_reduced_lidar_data;
        auto start = std::chrono::high_resolution_clock::now();
        sequential_test.seq_point_clustering(tmp_copy, 50, 14, readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
        auto end = std::chrono::high_resolution_clock::now();
        sequential_duration += std::chrono::duration<double, std::milli>(end - start).count();
    }

    // warming up for the parallel
    for (int i = 0; i < n_warmup; ++i) {
        std::vector<Point> tmp_copy = parallel_reduced_lidar_data;
        parallel_warmup.par_point_clustering(tmp_copy, 50, 14, readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
    }

    // timing the parallel functions
    for (int i = 0; i < n_test; ++i) {
        std::vector<Point> tmp_copy = parallel_reduced_lidar_data;
        auto start = std::chrono::high_resolution_clock::now();
        parallel_test.par_point_clustering(tmp_copy, 50, 14, readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
        auto end = std::chrono::high_resolution_clock::now();
        parallel_duration += std::chrono::duration<double, std::milli>(end - start).count();
    }

    // printing sequential functions durations
    sequential_duration /= n_test;
    std::cout << "Sequential duration: " << sequential_duration << std::endl;
    
    std::cout << "Sequential functions durations" << std::endl;
    for (int i = 0; i < sequential_test.sequential_timings.size(); ++i) {
        sequential_test.sequential_timings[i] /= n_test;
        std::cout << "Function " << i + 1 << " duration:\t" << sequential_test.sequential_timings[i] << std::endl;
    }

    // printing parallel functions durations
    parallel_duration /= n_test;
    std::cout << std::endl << "Parallel duration: " << parallel_duration << std::endl;

    std::cout << "Parallel functions durations" << std::endl;
    for (int i = 0; i < parallel_test.parallel_timings.size(); ++i) {
        parallel_test.parallel_timings[i] /= n_test;
        std::cout << "Function " << i + 1<< " duration:\t" << parallel_test.parallel_timings[i] << std::endl;
    }
}