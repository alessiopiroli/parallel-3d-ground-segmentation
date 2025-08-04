#include "seq_algorithm.hpp"
#include "visualizer.hpp"
#include <string>
#include <algorithm>

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
            std::cout << "Denominator is zero, return empty line" << std::endl;
            return Line();
        } else {
            const int n = points.size();
            float m = (n * sum_xy - (sum_x * sum_y)) / ((n * sum_xx) - (sum_x * sum_x));
            float b = (sum_y - (m * sum_x)) / n;
            return Line(m, b, points.front().get_rho(), points.back().get_rho());
        }
    }
}


void Sequential::ground_estimation(
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
                    std::cout << "Empty bin" << std::endl;
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

        for (int i = 0; i < prototype_points.size(); i++) {
            std::sort(prototype_points[i].begin(), prototype_points[i].end(), 
                [](const Point& l, const Point& r){ return l.get_rho() < r.get_rho(); });
        }

        // now we need to fit lines through prototype points
        // to do this, we need to define the max slope these lines can have
        // and what the maximum distance between lines is

        const float max_slope = 0.27;
        const float vd_ground = 0.15;

        // we need to create a vector of lines, with size same as the 
        // number of segments
        std::vector<std::vector<Line>> ground_lines_per_segment(n_segments);

        // we now loop through each segment and we do the line fitting
        // using the prototype points
        for (int i = 0; i < prototype_points.size(); i++) {
            // prototype_points[i] points to the prototype points
            // of segment i
            
            // if the segment contains no prototype points we skip the iteration
            if (prototype_points[i].empty()) {
                continue;
            }

            // we create a vector which contains the points that contribute
            // to the current line
            std::vector<Point> current_line_points;

            // we need to add the first point in the vector
            current_line_points.push_back(prototype_points[i][0]);

            // we then loop from the second point to do the actual testing
            for (int j = 0; j < prototype_points[i].size(); ++j) {
                // we save the point which exam to see if it still fits
                Point current_point = prototype_points[i][j];

                // we then need to test if the point actually keeps the line as
                // ground or not
                // to do this we need to use a temporary vector to copy our points
                std::vector<Point> test_points = current_line_points;
                test_points.push_back(current_point);

                // then we create the test line and check the parameters
                Line test_line = fit_line_to_points(test_points);

                if (!test_line.is_valid) {
                    current_line_points.clear();
                    current_line_points.push_back(current_point);
                    continue;
                }

                // we check if we are still in the bounds for adding this point
                // to the line or not
                if (std::abs(test_line.m) < max_slope) {
                    // if this is the case, we can add this to the points 
                    current_line_points.push_back(current_point);
                } else {
                    // if the point does not fit, we end the line and we save it
                    // but only if the line is made up of more than two points
                    if (current_line_points.size() >= 2) {
                        ground_lines_per_segment[i].push_back(fit_line_to_points(current_line_points));
                    }

                    // we then need to start a new line, with the first point
                    // being the one that broke the slope condition
                    current_line_points.clear();
                    current_line_points.push_back(current_point);
                }
            }
            // when the loop reaches the end, the last group of fitting points
            // have not been saved yet since no unfitting point was found
            if (current_line_points.size() >= 2) {
                ground_lines_per_segment[i].push_back(fit_line_to_points(current_line_points));
            }
        }

        // we create two empty vectors which will then hold the ground points
        // and the non-ground points
        std::vector<Point> ground_points;
        std::vector<Point> non_ground_points;

        // we analyze each single point to classify it
        for (const auto& p : lidar_data) {
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
                        }
                        break;
                    }
                }
            }
            if (is_ground) {
                ground_points.push_back(p);
            } else {
                non_ground_points.push_back(p);
            }
        }

        std::cout << "Number of ground points: " << ground_points.size() << std::endl;

        Visualizer ground_and_not_ground_points("Ground and not ground points");
        ground_and_not_ground_points.visualize_protype_points(lidar_data, ground_points);
    }
}