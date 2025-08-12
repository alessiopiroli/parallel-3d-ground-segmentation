#include "seq_algorithm.hpp"
#include "visualizer.hpp"
#include <string>
#include <algorithm>
#include <queue>

Sequential::Sequential(const float slope_, const int n_bins_, const int n_segments):
    slope{slope_}, n_bins{n_bins_} {}

void Sequential::plot_first_quadrant(std::vector<Point>& lidar_data) {
    std::vector<Point> first_quadrant;

    for (auto& p: lidar_data) {

        if (p.get_angle() < 90) {
            first_quadrant.push_back(std::move(p));
        }
    }

    std::cout << "First quadrant size: " << first_quadrant.size() << std::endl;

    Visualizer v("First quadrant");
    v.simple_visualization(first_quadrant);
}

void Sequential::plot_segments(std::vector<Point>& lidar_data, const int n_segments) {
    // fallback in case we provide invalid number of segments
    if (n_segments < 0) {
        return;
    }
    std::vector<std::vector<Point>> segments(n_segments);
    float min_angle = 360.0 / n_segments;

    // assigning points to segments
    for (auto& p : lidar_data) {
        int segment_index = static_cast<int>(p.get_angle() / min_angle);
        
        if (segment_index >= n_segments) {
            segment_index = n_segments - 1;
        }

        segments[segment_index].push_back(std::move(p));
    }

    // visualizing each segment separately
    for (int i = 0; i < segments.size(); ++i) {
        std::string title = "Segment " + std::to_string(i + 1);
        Visualizer v(title);
        v.simple_visualization(segments[i]);
    }
}

void Sequential::plot_binned_segments(std::vector<Point>& lidar_data, 
        const int n_segments, const int n_bins, const float max_range) {
    
    if (n_segments < 0 || n_bins < 0) {
        return;
    }

    // creating appropriate data structure, considering we also have the bins, not only segments
    // we create at first a vector which is the same size of the segments
    // then, we initialize it with a vector of vectors of size n_bins, which is empty
    // this way we can directly have the right size for the full vector
    std::vector<std::vector<std::vector<Point>>> binned_segments(n_segments,
        std::vector<std::vector<Point>>(n_bins));

    float angle_size = 360.0 / n_segments;
    float bin_size = max_range / n_bins;

    // sorting points in segments and bins
    for (auto& p : lidar_data) {
        // getting segment index and bin index
        int segment_index = static_cast<int>(p.get_angle() / angle_size);
        int bin_index = static_cast<int>((p.get_rho() / max_range) * n_bins);

        // correcting the indexes
        if (segment_index >= n_segments) {
            segment_index = n_segments - 1;
        }
        
        if (bin_index >= n_bins) {
            bin_index = n_bins - 1;
        }

        binned_segments[segment_index][bin_index].push_back(std::move(p));
    }

    // visualizing all bins
    for (int i = 0; i < binned_segments.size(); i++) {
        for (int j = 0; j < binned_segments[i].size(); j++) {
            if (binned_segments[i][j].empty()) {
                std::cout << "Bin is empty, can't visualize" << std::endl;
            } else {
                std::cout << "Visualizing bin:" << std::endl;
                Visualizer v("Bin visualizer");
                v.simple_visualization(binned_segments[i][j]);
            }
        }
    }
}

Line::Line() : m{0.0}, b{0.0}, start_range{0.0}, end_range{0.0}, is_valid{false} {}

Line fit_line_to_points(const std::vector<Point>& points) {
    // first, we initialize the sums for the algorithm
    float sum_x = 0.0;
    float sum_y = 0.0;
    float sum_xy = 0.0;
    float sum_xx = 0.0;

    // checking if the number of points is enough to fit a line
    if (points.size() < 2) {
        // if we don't have enough points, we can't fit a line through them
        return Line();
    } else {
        for (const auto& p : points) {
            float x = p.get_rho();
            float y = p.get_z_value();
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_xx += x * x;
        }

        float denominator = ((points.size()) * (sum_xx)) -  (sum_x * sum_x);

        // we can't create a line if we end up dividing by zero
        if (denominator == 0.0) {
            // std::cout << "Denominator is zero, returning empty line" << std::endl;
            return Line();
        } else {
            const int n = points.size();
            float m = (n * sum_xy - (sum_x * sum_y)) / ((n * sum_xx) - (sum_x * sum_x));
            float b = (sum_y - (m * sum_x)) / n;
            return Line(m, b, points.front().get_rho(), points.back().get_rho());
        }
    }
}

float calculate_rmse(const Line& line, const std::vector<Point>& points) {
    if (points.empty() || !line.is_valid) {
        return std::numeric_limits<float>::max();
    }
    float sum_squared_error = 0.0;
    for (const auto& p : points) {
        float expected_z = line.m * p.get_rho() + line.b;
        float error = p.get_z_value() - expected_z;
        sum_squared_error += error * error;
    }
    return std::sqrt(sum_squared_error / points.size());
}

void Sequential::ground_estimation_and_clustering(
    std::vector<Point>& lidar_data,
    const int n_segments,
    const int n_bins,
    const float max_range) {
    
    // if the number of bins or the number of segments is lower than zero, it's invalid
    // and we can't progress with ground estimation
    if (n_bins < 0 || n_segments < 0) {
        throw std::invalid_argument("n_bins or n_segment can't be invalid");
    } else {
        // creating data structure to hold bins inside segments
        std::vector<std::vector<std::vector<Point>>> binned_segments(n_segments,
            std::vector<std::vector<Point>>(n_bins));

        // creating the data structure to hold the prototype points
        // as per the paper, the prototype points are those 
        std::vector<std::vector<Point>> prototype_points(n_segments);

        // angle value of each segment
        float angle_size = 360.0 / n_segments;
        // For each segment, we define the lenght between r_j^min and r_j^max
        float bin_size = max_range / n_bins;

        // sorting points in segments and bins
        for (auto& p : lidar_data) {
            // for the point p, we get the index of the segment it belongs to
            int segment_index = static_cast<int>(p.get_angle() / angle_size);
            // for the same point p, we get the index of the bin it belongs to 
            // (in segment calculated before)
            int bin_index = static_cast<int>((p.get_rho() / max_range) * n_bins);

            // correcting segment index if necessary
            if (segment_index >= n_segments) {
                segment_index = n_segments - 1;
            }

            // correcting bin index if necessary
            if (bin_index >= n_bins) {
                bin_index = n_bins - 1;
            }

            // we take each point and copy in the binned_segments vector in the
            // correct index for the segment and the bin
            binned_segments[segment_index][bin_index].push_back(p);
        }

        // assignign each z-lowest point for each bin for each segment as the prototype
        // fist, with i we access the correct segment
        // then, with j we access the correct bin, which is still a vector itself
        // the vector, which will be binned_segments[i][j], will contain all points belonging to a bin
        for (int i = 0; i < binned_segments.size(); i++) {
            for (int j = 0; j < binned_segments[i].size(); j++) {
                if (binned_segments[i][j].empty()) {
                    // std::cout << "Empty bin" << std::endl;
                } else {
                    // if the bin contains at least one points, we find the points with the lowest z value
                    // then, this points is copied in the prototype_points vector, in the same segment it 
                    // originally belonged to

                    // we define an iterator min_it which points to the element with the lowest
                    // z axis value of the corresponding bin vector
                    auto min_it = std::min_element(binned_segments[i][j].begin(), binned_segments[i][j].end(),
                        [](const Point& left, const Point& right){ return left.get_z_value() < right.get_z_value(); });
                    
                    // if this iterator points to the end, it means there's no minimum element
                    if (min_it != binned_segments[i][j].end()) {
                        // if there is a minimum element, we copy it to the prototype points vector
                        prototype_points[i].push_back(*min_it);
                    }
                }
            }
        }

        // we need to sort the prototype points in each segment by ascending distance
        for (auto& segment: prototype_points) {
            std::sort(segment.begin(), segment.end());
        }


        // now we need to fit lines through prototype points
        // to do this, we need to define the paper's parameters
        const float max_slope = 0.3;
        const float vd_ground = 0.15;
        const float max_y_intercept = 0.2;
        const float max_rmse = 0.3;

        // we need to create a vector of lines, with size same as the 
        // number of segments
        std::vector<std::vector<Line>> ground_lines_per_segment(n_segments);

        // we loop through each segment and do the line fitting using the prototype points
        for (int i = 0; i < n_segments; ++i) {
            if (prototype_points[i].size() < 2) continue;

            // we create a vector of points which contains the points making up the line
            std::vector<Point> current_line_points;
            current_line_points.push_back(prototype_points[i][0]);

            for (size_t j = 1; j < prototype_points[i].size(); ++j) {
                Point current_point = prototype_points[i][j];
            
                // if we don't have enough points we need to another one
                if (current_line_points.size() < 2) {
                    current_line_points.push_back(current_point);
                    continue;
                }

                // we copy the points to create a vector of test points
                std::vector<Point> test_points = current_line_points;
                test_points.push_back(current_point);
                Line test_line = fit_line_to_points(test_points);

                // we check if the conditions described in the paper are satisfied
                if (test_line.is_valid && 
                    std::abs(test_line.m) < max_slope && 
                    calculate_rmse(test_line, test_points) < max_rmse &&
                    std::abs(test_line.b) < max_y_intercept) {
                    
                    // if the point fits then we add it
                    current_line_points.push_back(current_point);
                } else {
                    // if the point doesn't fit we finalize the last one
                    Line final_line = fit_line_to_points(current_line_points);
                    if (final_line.is_valid) {
                        ground_lines_per_segment[i].push_back(final_line);
                    }
                    // we use the last point that didn't fit to start the next line
                    current_line_points.clear();
                    current_line_points.push_back(current_point);
                }
            }

            // we add the last line after the loop finishes
            if (current_line_points.size() >= 2) {
                Line final_line = fit_line_to_points(current_line_points);
                if(final_line.is_valid) {
                    ground_lines_per_segment[i].push_back(final_line);
                }
            }
        }

        // we create two empty vectors which will then hold the ground points
        // and the non-ground points
        std::vector<Point> expected_ground_points;
        std::vector<Point> non_ground_points;

        // we analyze each single point to classify it
        for (auto& p : lidar_data) {
            // // at the beginning we say that every point is not a ground point
            // bool is_ground = false;

            // for each points, we need to calculate the segment index and
            // correct it if necessary, as we did before
            int segment_index = static_cast<int>(p.get_angle() / angle_size);
            if (segment_index >= n_segments) {
                segment_index = n_segments - 1;
            }

            bool is_ground = false;

            // we then need to look at the respective segment and see if a ground
            // model was actually found (lines)
            // if not, the point is labeled as not a ground point
            if (!ground_lines_per_segment[segment_index].empty()) {
                // we then loop the line segments for the segment
                for (const auto& line : ground_lines_per_segment[segment_index]) {
                    // we then select the correct line segment
                    if (p.get_rho() >= line.start_range && p.get_rho() <= line.end_range) {
                        // we calculate the expected height of the ground at the point's range
                        float exp_height = line.m * p.get_rho() + line.b;

                        // if the point's height is close to the expected ground height
                        // it is then classified as a ground point
                        if (std::abs(p.get_z_value() - exp_height) < vd_ground) {
                            is_ground = true;
                            break;
                        }
                    }
                }
            }
            if (is_ground) {
                p.prediction = 40;
                expected_ground_points.push_back(p);
            } else {
                non_ground_points.push_back(p);
            }
        }

        int n_tot_points = 0;
        int n_real_ground_points = 0;
        int n_estimate_ground_points = 0;
        int tp = 0;
        int fn = 0;
        int fp = 0;
        int tn = 0;

        std::vector<Point> real_ground_points;

        for (auto& p : lidar_data) {
            n_tot_points++;

            if(p.ground_truth == 40) {
                n_real_ground_points++;
            }

            if (p.prediction == 40) {
                n_estimate_ground_points++;
            }

            if (p.ground_truth == 40 && p.prediction == 40) {
                real_ground_points.push_back(p);
                tp++;
            }
            
            if (p.ground_truth == 40 && p.prediction != 40) {
                fn++;
            }

            if (p.ground_truth != 40 && p.prediction == 40) {
                fp++;
            }

            if (p.ground_truth != 40 && p.prediction != 40) {
                tn++;
            }
        }

        if (tp + fp + tn + fn == n_tot_points) {
            float pred_acc = static_cast<float>(tp) / (tp + fp + fn);
            std::cout << "Jaccard Index = " << pred_acc << std::endl;
        } else {
            std::cout << "Wrong true and false positive" << std::endl;
        }

        Visualizer rgp_vis("Real Ground points visualization");    
        rgp_vis.timed_visualization(lidar_data, real_ground_points);   

        
        // defining grid properties
        float grid_resolution = 1.0;

        // we use placeholders in max and min values for x and y actually calculating them
        float min_x = non_ground_points[0].x_value; 
        float max_x = non_ground_points[0].x_value;
        float min_y = non_ground_points[0].y_value;
        float max_y = non_ground_points[0].y_value;

        // we calculate maximum and minimum values
        for (const auto& p: non_ground_points) {
            if (p.x_value < min_x) {
                min_x = p.x_value;
            }

            if (p.x_value > max_x) {
                max_x = p.x_value;
            }

            if (p.y_value < min_y) {
                min_y = p.y_value;
            }

            if (p.y_value > max_y) {
                max_y = p.y_value;
            }
        }

        // we calculate the grid width and height
        int grid_width = static_cast<int>(std::ceil((max_x - min_x) / grid_resolution));
        int grid_height = static_cast<int>(std::ceil((max_y - min_y) / grid_resolution));

        // we then create and populate label and point grids
        std::vector<std::vector<int>> label_grid(grid_height, std::vector<int>(grid_width, 0));
        std::vector<std::vector<std::vector<Point*>>> point_grid(grid_height, std::vector<std::vector<Point*>>(grid_width));

        for(auto& p : non_ground_points) {
            int col_index = static_cast<int>((p.x_value - min_x) / grid_resolution);
            int row_index = static_cast<int>((p.y_value - min_y) / grid_resolution);
        
            if ((col_index >= 0) && (col_index < grid_width) && (row_index >= 0) && (row_index < grid_height)) {
                // we mark the cell as occupied
                label_grid[row_index][col_index] = 1;
                point_grid[row_index][col_index].push_back(&p);
            }
        }

        // we then connect adjacent cells
        int cluster_id = 2;
        std::queue<std::pair<int, int>> q;
        
        int row_offsets[] = {-1, 1, 0, 0};
        int col_offsets[] = {0, 0, 1, -1};

        for (int r = 0; r < grid_height; r++) {
            for (int c = 0; c < grid_width; c++) {
                // if we find a new unlabeled cell
                if (label_grid[r][c] == 1) {
                    q.push({r, c});
                    label_grid[r][c] = cluster_id;
                
                    while(!q.empty()) {
                        std::pair<int, int> curr = q.front();
                        q.pop();

                        for(int i = 0; i < 4; ++i) {
                            int nr = curr.first + row_offsets[i];
                            int nc = curr.second + col_offsets[i];

                            if(nr >= 0 && nr < grid_height && nc >= 0 && nc < grid_width && label_grid[nr][nc] == 1) {
                                label_grid[nr][nc] = cluster_id;
                                q.push({nr, nc});
                            }
                        } 
                    }

                    cluster_id++;
                }
            }
        }


        int num_clusters = cluster_id - 2;
        std::vector<std::vector<Point>> final_clusters(num_clusters);

        // we scan all cells
        for(int r = 0; r < grid_height; ++r) {
            for(int c = 0; c < grid_width; c++) {
                // we check if each cell has actually been labeled
                if (label_grid[r][c] > 1) {
                    int id = label_grid[r][c] - 2;
                    // we add the points to the correct cluster
                    for(Point* p_ptr : point_grid[r][c]) {
                        final_clusters[id].push_back(*p_ptr);
                    }
                }
            }
        }
    }
}