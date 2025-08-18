#pragma once

#include "file_reader.hpp"

#include <utility>
#include <chrono>

struct Line {
    float m;
    float b;
    float start_range;
    float end_range;
    bool is_valid = false;

    Line(const float m_, const float b_, const float start_range_, const float end_range_) :
        m{m_}, b{b_}, start_range{start_range_}, end_range{end_range_}, is_valid{true} {}
    
    Line();
};


class Sequential {
private:
    float slope;
    int n_bins;

public:
    std::vector<double> sequential_timings;

    Sequential(const float slope_, const int n_bins_, const int n_segments);

    Sequential();

    // Debugging functions
    void plot_first_quadrant(std::vector<Point>& lidar_data);

    void plot_segments(std::vector<Point>& lidar_data, const int n_segments);

    void plot_binned_segments(std::vector<Point>& lidar_data, const int n_segments, const int n_bins, const float max_range);

    // one shot approach
    void ground_estimation_and_clustering(std::vector<Point>& lidar_data, const int n_segments, const int n_bins, const float max_range);


    // modular approach
    void seq_seg_and_bin_sorting(std::vector<Point>& lidar_data, std::vector<std::vector<std::vector<Point>>>& binned_segments_, 
        const int n_segments, const int n_bins, const float max_range);

    void seq_assign_prototype(std::vector<std::vector<std::vector<Point>>>& binned_segments,
        std::vector<std::vector<Point>>& prototype_points);

    void seq_fit_lines(std::vector<std::vector<Point>>& prototype_points,
        std::vector<std::vector<Line>>& ground_lines_per_segment, const int n_segments,
        const float max_slope, const float max_rmse, const float max_y_intercept);

    void seq_ground_points_classification(std::vector<Point>& lidar_data, 
        std::vector<std::vector<Line>>& ground_lines_per_segment, const int n_segments,
        const float vd_ground);

    void print_ground_statistics(std::vector<Point>& lidar_data);

    void seq_remaining_points_classification(std::vector<Point>& lidar_data,
        const float grid_resolution);

    void seq_point_clustering(std::vector<Point>& lidar_data, const int n_segments, const int n_bins, const float max_range,
        const float max_slope, const float vd_ground, const float max_y_intercept, const float max_rmse);
};

Line fit_line_to_points(const std::vector<Point>& points);
float calculate_rmse(const Line& line, const std::vector<Point>& points);