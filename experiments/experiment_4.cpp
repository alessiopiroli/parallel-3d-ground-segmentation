#include "file_reader.hpp"
#include "visualizer.hpp"
#include "seq_algorithm.hpp"
#include "par_algorithm.hpp"

int main () {
    std::string coordinates_file = "/home/alessio/workspace/tum/par-comp/"
        "data-parallel-3d-ground-segmentation/labeled_pc/04/velodyne/000056.bin";

    std::string labels_file = "/home/alessio/workspace/tum/par-comp/"
        "data-parallel-3d-ground-segmentation/labeled_pc/04/labels/000056.label";

    std::vector<Point> original_lidar_data;

    ReadLiDARdata readLiDAR(coordinates_file, labels_file);
    readLiDAR.read_data(original_lidar_data);
    const int number_of_threads = 16;
    const int number_of_seg_and_bins = 1000;

    std::cout << "N_THREADS: " << number_of_threads << std::endl;
    std::cout << "N_BINS & THREADS: " << number_of_seg_and_bins << std::endl;

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
        std::vector<Point> tmp_copy = original_lidar_data;
        sequential_warmup.seq_point_clustering(tmp_copy, number_of_seg_and_bins, number_of_seg_and_bins, 
            readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
    }

    // timing the sequential functions
    for (int i = 0; i < n_test; ++i) {
        std::vector<Point> tmp_copy = original_lidar_data;
        auto start = std::chrono::high_resolution_clock::now();
        sequential_test.seq_point_clustering(tmp_copy, number_of_seg_and_bins, number_of_seg_and_bins, 
            readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
        auto end = std::chrono::high_resolution_clock::now();
        sequential_duration += std::chrono::duration<double, std::milli>(end - start).count();
    }

    // warming up for the parallel
    for (int i = 0; i < n_warmup; ++i) {
        std::vector<Point> tmp_copy = original_lidar_data;
        parallel_warmup.par_point_clustering(tmp_copy, number_of_seg_and_bins, number_of_seg_and_bins, 
            readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
    }

    // timing the parallel functions
    for (int i = 0; i < n_test; ++i) {
        std::vector<Point> tmp_copy = original_lidar_data;
        auto start = std::chrono::high_resolution_clock::now();
        parallel_test.par_point_clustering(tmp_copy, number_of_seg_and_bins, number_of_seg_and_bins, 
            readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
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