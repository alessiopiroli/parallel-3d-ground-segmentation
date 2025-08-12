#include "file_reader.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>

#define PI 3.14159265

Point::Point(const float x_val, const float y_val, const float z_val, const float ref_val) :
    x_value{x_val}, y_value{y_val}, z_value{z_val}, ref_value{ref_val} {}

float Point::get_x_value() const {
    return x_value;
}

float Point::get_y_value() const {
    return y_value;
}

float Point::get_z_value() const {
    return z_value;
}

float Point::get_ref_value() const {
    return ref_value;
}

bool Point::operator<(const Point& other) const{
    return get_rho() < other.get_rho();
}

ReadLiDARdata::ReadLiDARdata(const std::string& coordinates_path_, const std::string& labels_path_) : 
    coordinates_path{coordinates_path_}, labels_path{labels_path_} {}

void ReadLiDARdata::read_data(std::vector<Point>& lidar_data) {
    FILE* coordinates_pointer = fopen(coordinates_path.c_str(), "rb");
    FILE* labels_pointer = fopen(labels_path.c_str(), "rb");
    
    if (coordinates_pointer == nullptr) {
        std::cout << "Could not open coordinates file" << std::endl;
        return;
    }

    if (labels_pointer == nullptr) {
        std::cout << "Cold not open labels file" << std::endl;
    }

    Point tmp_point;

    while(true) {
        // variables to store how much we have actually read
        size_t coordinate_flag = fread(&tmp_point.x_value, sizeof(float), 4, coordinates_pointer);
        size_t label_flag = fread(&tmp_point.ground_truth, sizeof(uint32_t), 1, labels_pointer);

        // if we haven't read enough we can stop reading
        if ((coordinate_flag) < 4 || (label_flag < 1)) {
            break;
        }

        // counting the number of ground points
        if(tmp_point.ground_truth == 40) {
            ground_point_number++;
        }

        lidar_data.push_back(tmp_point);
    }

    fclose(coordinates_pointer);

    auto max_it = std::max_element(lidar_data.begin(), lidar_data.end(), 
        [](const Point& left, const Point& right){ return left.get_rho() < right.get_rho(); });
    
    if (max_it != lidar_data.end()) {
        max_range = max_it->get_rho();
    }
}

void ReadLiDARdata::print_data(const std::vector<Point>& lidar_data) {
    if (!lidar_data.empty()) {
        for (auto it = lidar_data.begin(); it != lidar_data.end(); ++it) {
            std::cout << it->get_x_value() << " " <<
                it->get_y_value() << " " << 
                it->get_z_value() << " " <<
                it->get_ref_value() << std::endl;
        }
    }
}

void ReadLiDARdata::get_scene_size(const std::vector<Point>& lidar_data) {
    std::cout << "Number of points: " << lidar_data.size() << std::endl;
}

float Point::get_angle() const {
    float angle = atan2(y_value, x_value) * 180 / PI;
    if (angle < 0) {
        return (angle + 360.0);
    } else {
        return angle;
    }
}

float Point::get_rho() const {
    return (sqrt((x_value * x_value) + (y_value * y_value)));
}

float ReadLiDARdata::get_max_range() const {
    return max_range;
}

int ReadLiDARdata::get_n_ground_points() const {
    return ground_point_number;
}