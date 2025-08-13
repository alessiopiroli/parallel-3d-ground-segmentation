#pragma once

#include <omp.h>

#include "file_reader.hpp"
#include "seq_algorithm.hpp"
#include "visualizer.hpp"

class Parallel {
private:
    const int num_threads = 0;

public:
    Parallel() = default;

    Parallel(const int num_threads_);

    void par_seg_and_bin_sorting(std::vector<Point>& lidar_data, std::vector<std::vector<std::vector<Point>>>& binned_segments, 
        const int n_segments, const int n_bins, const float max_range);

    void par_assign_prototype(std::vector<std::vector<std::vector<Point>>>& binned_segments,
    std::vector<std::vector<Point>>& prototype_points);

    void par_fit_lines(std::vector<std::vector<Point>>& prototype_points,
        std::vector<std::vector<Line>>& ground_lines_per_segment, const int n_segments,
        const float max_slope, const float max_rmse, const float max_y_intercept);

    void par_ground_points_classification(std::vector<Point>& lidar_data, 
        std::vector<std::vector<Line>>& ground_lines_per_segment, const int n_segments,
        const float vd_ground);

    void print_ground_statistics(std::vector<Point>& lidar_data);

    void par_remaining_points_classification(std::vector<Point>& lidar_data,
        const float grid_resolution);

    void par_point_clustering(std::vector<Point>& lidar_data, const int n_segments, const int n_bins, const float max_range,
        const float max_slope, const float vd_ground, const float max_y_intercept, const float max_rmse);
};