#include "par_algorithm.hpp"

#include <algorithm>
#include <queue>

Parallel::Parallel(const int num_threads_) : num_threads{num_threads_} {
    parallel_timings.resize(5, 0.0);
}

void Parallel::par_seg_and_bin_sorting(std::vector<Point>& lidar_data, std::vector<std::vector<std::vector<Point>>>& binned_segments, 
        const int n_segments, const int n_bins, const float max_range) {
    float angle_size = 360.0/n_segments;

    #pragma omp parallel num_threads(num_threads)
    {
        // we create each local bin
        std::vector<std::vector<std::vector<Point>>> local_bins(n_segments, std::vector<std::vector<Point>>(n_bins));

        #pragma omp for nowait
        for (int i = 0; i < lidar_data.size(); i++) {
            auto& p = lidar_data[i];

            int segment_index = static_cast<int>(p.get_angle() / angle_size);
            int bin_index = static_cast<int>((p.get_rho() / max_range) * n_bins);

            if (segment_index >= n_segments) {
                segment_index = n_segments - 1;
            }

            if (bin_index >= n_bins) {
                bin_index = n_bins - 1;
            }

            local_bins[segment_index][bin_index].push_back(p);
        }

        #pragma omp critical
        {
            for (int s = 0; s < n_segments; ++s) {
                for (int b = 0; b < n_bins; ++b) {
                    binned_segments[s][b].insert(binned_segments[s][b].end(), local_bins[s][b].begin(), local_bins[s][b].end());
                }
            }
        }
    }
}

void Parallel::par_assign_prototype(std::vector<std::vector<std::vector<Point>>>& binned_segments,
    std::vector<std::vector<Point>>& prototype_points) {
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < binned_segments.size(); ++i) {
        for (int j = 0; j < binned_segments[i].size(); ++j) {
            if (!binned_segments[i][j].empty()) {
                auto min_it = std::min_element(binned_segments[i][j].begin(), binned_segments[i][j].end(),
                    [](const Point& left, const Point& right){ return left.get_z_value() < right.get_z_value(); });

                if (min_it != binned_segments[i][j].end()) {
                    prototype_points[i].push_back(*min_it);
                }
            }
        }
    }

    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < prototype_points.size(); ++i) {
        std::sort(prototype_points[i].begin(), prototype_points[i].end());
    }
}

void Parallel::par_fit_lines(std::vector<std::vector<Point>>& prototype_points,
    std::vector<std::vector<Line>>& ground_lines_per_segment, const int n_segments,
    const float max_slope, const float max_rmse, const float max_y_intercept) {
    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < n_segments; ++i) {
        if (prototype_points[i].size() < 2) {
            continue;
        }

        std::vector<Point> current_line_points;
        current_line_points.push_back(prototype_points[i][0]);

        for (int j = 1; j < prototype_points[i].size(); ++j) {
            Point current_point = prototype_points[i][j];

            if (current_line_points.size() < 2) {
                current_line_points.push_back(current_point);
                continue;
            }

            std::vector<Point> test_points = current_line_points;
            test_points.push_back(current_point);
            Line test_line = fit_line_to_points(test_points);

            if ((test_line.is_valid) && (std::abs(test_line.m) < max_slope) &&
                (calculate_rmse(test_line, test_points) < max_rmse) && (std::abs(test_line.b) < max_y_intercept)) {
                current_line_points.push_back(current_point);
            } else {
                Line final_line = fit_line_to_points(current_line_points);

                if (final_line.is_valid) {
                    ground_lines_per_segment[i].push_back(final_line);
                }

                current_line_points.clear();
                current_line_points.push_back(current_point);
            }
        }

        if (current_line_points.size() >= 2) {
            Line final_line = fit_line_to_points(current_line_points);

            if (final_line.is_valid) {
                ground_lines_per_segment[i].push_back(final_line);
            }
        }
    }
}

void Parallel::par_ground_points_classification(std::vector<Point>& lidar_data, 
    std::vector<std::vector<Line>>& ground_lines_per_segment, const int n_segments,
    const float vd_ground) {
    float angle_size = 360.0 / n_segments;

    #pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < lidar_data.size(); ++i) {
        auto& p = lidar_data[i];

        int segment_index = static_cast<int>(p.get_angle() / angle_size);

        if (segment_index >= n_segments) {
            segment_index = n_segments - 1;
        }

        bool is_ground = false;

        if (!ground_lines_per_segment[segment_index].empty()) {
            for (const auto& line : ground_lines_per_segment[segment_index]) {
                if ((p.get_rho() >= line.start_range) && (p.get_rho() <= line.end_range)) {
                    float expected_height = line.m * p.get_rho() + line.b;

                    if (std::abs(p.get_z_value() - expected_height) < vd_ground) {
                        is_ground = true;
                        break;
                    }
                }
            }
        }

        if (is_ground) {
            p.prediction = 40;
        }
    }
}

void Parallel::print_ground_statistics(std::vector<Point>& lidar_data) {
    int tot_number_points = 0;
    int num_real_ground_points = 0;
    int num_estimate_ground_ground__points = 0;

    int true_positive = 0;
    int true_negative = 0;
    int false_positive = 0;
    int false_negative = 0;

    for (auto& p : lidar_data) {
        tot_number_points++;
        
        if (p.ground_truth == 40) {
            num_real_ground_points++;
        }

        if (p.prediction == 40) {
            num_estimate_ground_ground__points++;
        }

        if (p.ground_truth == 40 && p.prediction == 40) {
            true_positive++;
        }

        if (p.ground_truth == 40 && p.prediction != 40) {
            false_negative++;
        }

        if (p.ground_truth != 40 && p.prediction == 40) {
            false_positive++;
        }

        if (p.ground_truth != 40 && p.prediction != 40) {
            true_negative++;
        }
    }

    if ((true_positive + true_negative + false_positive + false_negative == tot_number_points)) {
        float classification_accuracy = (static_cast<float>(true_positive)) / (true_positive + false_positive + false_negative);
        std::cout << "Parallel mIoU: \t\t" << classification_accuracy << std::endl;
    } else {
        std::cout << "Wrong calculation" << std::endl;
    }
}

void Parallel::slow_par_remaining_points_classification(std::vector<Point>& lidar_data,
    const float grid_resolution) {
    std::vector<Point*> non_ground_points;
    int num_clusters = 0;

    for (auto& p : lidar_data) {
        if (p.prediction != 40) {
            non_ground_points.push_back(&p);
        }
    }

    if (non_ground_points.empty()) {
        return;
    }

    float min_x = non_ground_points[0]->x_value;
    float max_x = non_ground_points[0]->x_value;
    float min_y = non_ground_points[0]->y_value;
    float max_y = non_ground_points[0]->y_value;

    #pragma omp parallel for num_threads(num_threads) reduction(min: min_x, min_y) reduction (max: max_x, max_y)
    for (int i = 1; i < non_ground_points.size(); ++i) {
        min_x = std::min(min_x, non_ground_points[i]->get_x_value());
        min_y = std::min(min_y, non_ground_points[i]->get_y_value());
        max_x = std::max(max_x, non_ground_points[i]->get_x_value());
        max_y = std::max(max_x, non_ground_points[i]->get_y_value());
    }

    int grid_width = static_cast<int>(std::ceil((max_x - min_x) / grid_resolution));
    int grid_height = static_cast<int>(std::ceil((max_y - min_y) / grid_resolution));

    std::vector<std::vector<int>> label_grid(grid_height, std::vector<int>(grid_width, 0));
    std::vector<std::vector<std::vector<Point*>>> point_grid(grid_height, std::vector<std::vector<Point*>>(grid_width));

    for (Point* p_ptr : non_ground_points) {
        int col_index = static_cast<int>((p_ptr->x_value - min_x) / grid_resolution);
        int row_index = static_cast<int>((p_ptr->y_value - min_y) / grid_resolution);

        if ((col_index >= 0) && (col_index < grid_width) && (row_index >= 0) && (row_index < grid_height)) {
            label_grid[row_index][col_index] = 1;
            point_grid[row_index][col_index].push_back(p_ptr);
        }
    }

    int cluster_id = 2;
    std::queue<std::pair<int, int>> q;

    int row_offset[] = {-1, 1, 0, 0};
    int col_offset[] = {0, 0, 1, -1};

    for (int r = 0; r < grid_height; ++r) {
        for (int c = 0; c < grid_width; ++c) {
            if (label_grid[r][c] == 1) {
                q.push({r, c});
                label_grid[r][c] = cluster_id;

                while (!q.empty()) {
                    std::pair<int, int> current = q.front();
                    q.pop();

                    for (int i = 0; i < 4; ++i) {
                        int nr = current.first + row_offset[i];
                        int nc = current.second + col_offset[i];

                        if ((nr >= 0) && (nr < grid_height) && (nc >= 0) && (nc < grid_width) && (label_grid[nr][nc] == 1)) {
                            label_grid[nr][nc] = cluster_id;
                            q.push({nr, nc});
                        }
                    }
                }
            
                cluster_id++;
            }
        }
    }

    #pragma omp parallel for collapse(2) num_threads(num_threads)
    for (int r = 0; r < grid_height; ++r) {
        for (int c = 0; c < grid_width; ++c) {
            if (label_grid[r][c] > 1) {
                int final_cluster_id = label_grid[r][c];

                for (Point* p_ptr : point_grid[r][c]) {
                    p_ptr->prediction = final_cluster_id;
                }
            }
        }
    }
}

void Parallel::par_remaining_points_classification(std::vector<Point>& lidar_data,
        const float grid_resolution) {
    std::vector<Point*> non_ground_points;
    int num_clusters = 0;

    for (auto& p : lidar_data) {
        if (p.prediction != 40) {
            non_ground_points.push_back(&p);
        }
    }

    if (non_ground_points.empty()) {
        return;
    }

    float min_x = non_ground_points[0]->x_value;
    float max_x = non_ground_points[0]->x_value;
    float min_y = non_ground_points[0]->y_value;
    float max_y = non_ground_points[0]->y_value;

    for (const auto& p_ptr : non_ground_points) {
        if (p_ptr->x_value < min_x) {
            min_x = p_ptr->x_value;
        }

        if (p_ptr->x_value > max_x) {
            max_x = p_ptr->x_value;
        }

        if (p_ptr->y_value < min_y) {
            min_y = p_ptr->y_value;
        }

        if (p_ptr->y_value > max_y) {
            max_y = p_ptr->y_value;
        }
    }

    int grid_width = static_cast<int>(std::ceil((max_x - min_x) / grid_resolution));
    int grid_height = static_cast<int>(std::ceil((max_y - min_y) / grid_resolution));

    std::vector<std::vector<int>> label_grid(grid_height, std::vector<int>(grid_width, 0));
    std::vector<std::vector<std::vector<Point*>>> point_grid(grid_height, std::vector<std::vector<Point*>>(grid_width));

    for (Point* p_ptr : non_ground_points) {
        int col_index = static_cast<int>((p_ptr->x_value - min_x) / grid_resolution);
        int row_index = static_cast<int>((p_ptr->y_value - min_y) / grid_resolution);

        if ((col_index >= 0) && (col_index < grid_width) && (row_index >= 0) && (row_index < grid_height)) {
            label_grid[row_index][col_index] = 1;
            point_grid[row_index][col_index].push_back(p_ptr);
        }
    }

    int cluster_id = 2;
    std::queue<std::pair<int, int>> q;

    int row_offset[] = {-1, 1, 0, 0};
    int col_offset[] = {0, 0, 1, -1};

    for (int r = 0; r < grid_height; ++r) {
        for (int c = 0; c < grid_width; ++c) {
            if (label_grid[r][c] == 1) {
                q.push({r, c});
                label_grid[r][c] = cluster_id;

                while (!q.empty()) {
                    std::pair<int, int> current = q.front();
                    q.pop();

                    for (int i = 0; i < 4; ++i) {
                        int nr = current.first + row_offset[i];
                        int nc = current.second + col_offset[i];

                        if ((nr >= 0) && (nr < grid_height) && (nc >= 0) && (nc < grid_width) && (label_grid[nr][nc] == 1)) {
                            label_grid[nr][nc] = cluster_id;
                            q.push({nr, nc});
                        }
                    }
                }
            
                cluster_id++;
            }
        }
    }

    num_clusters = cluster_id - 2;

    for (int r = 0; r < grid_height; ++r) {
        for (int c = 0; c < grid_width; ++c) {
            if (label_grid[r][c] > 1) {
                int final_cluster_id = label_grid[r][c];
                
                for (Point* p_ptr : point_grid[r][c]) {
                    p_ptr->prediction = final_cluster_id;
                }
            }
        }
    }
}


void Parallel::par_point_clustering(std::vector<Point>& lidar_data, const int n_segments, const int n_bins, const float max_range,
        const float max_slope, const float vd_ground, const float max_y_intercept, const float max_rmse) {
    if (n_bins < 0 || n_segments < 0) {
        throw std::invalid_argument("Number of bins or segments is invalid");
    } else {
        std::vector<std::vector<std::vector<Point>>> binned_segments(n_segments,
            std::vector<std::vector<Point>>(n_bins));
        std::vector<std::vector<Point>> prototype_points(n_segments);
        std::vector<std::vector<Line>> ground_lines_per_segment(n_segments);
        // Visualizer par_visualizer("Parallel Classification");

        // timing and running parallel seg_and_bin_sorting
        auto start = std::chrono::high_resolution_clock::now();
        par_seg_and_bin_sorting(lidar_data, binned_segments, n_segments, n_bins, max_range);
        auto end = std::chrono::high_resolution_clock::now();
        parallel_timings[0] += std::chrono::duration<double, std::milli>(end - start).count();

        // timing and running seuential assign_prototype
        start = std::chrono::high_resolution_clock::now();
        par_assign_prototype(binned_segments, prototype_points);
        end = std::chrono::high_resolution_clock::now();
        parallel_timings[1] += std::chrono::duration<double, std::milli>(end - start).count();

        // timing and running fit_lines
        start = std::chrono::high_resolution_clock::now();
        par_fit_lines(prototype_points, ground_lines_per_segment, n_segments, max_slope, max_rmse, max_y_intercept);
        end = std::chrono::high_resolution_clock::now();
        parallel_timings[2] += std::chrono::duration<double, std::milli>(end - start).count();

        // timing and running ground_points_classification
        start = std::chrono::high_resolution_clock::now();
        par_ground_points_classification(lidar_data, ground_lines_per_segment, n_segments, vd_ground);
        end = std::chrono::high_resolution_clock::now();
        parallel_timings[3] += std::chrono::duration<double, std::milli>(end - start).count();

        // print_ground_statistics(lidar_data);
        // par_visualizer.visualize_ground_estimation(lidar_data);

        // timing and running remaining_point_classification
        start = std::chrono::high_resolution_clock::now();
        // slow_par_remaining_points_classification(lidar_data, 0.3);
        par_remaining_points_classification(lidar_data, 0.3);
        end = std::chrono::high_resolution_clock::now();
        parallel_timings[4] += std::chrono::duration<double, std::milli>(end - start).count();

        // par_visualizer.visualize_clusters(lidar_data);
    }
}