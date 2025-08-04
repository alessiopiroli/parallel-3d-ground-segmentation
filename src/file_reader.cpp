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

ReadLiDARdata::ReadLiDARdata(const std::string&& file_path_) : file_path{file_path_} {}

void ReadLiDARdata::read_data(std::vector<Point>& lidar_data) {
    FILE* file_pointer = fopen(file_path.c_str(), "rb");
    if (file_pointer == nullptr) {
        std::cout << "Could not open the file" << std::endl;
        return;
    }

    std::cout << "File opened" << std::endl;

    Point tmp_point;

    while (fread(&tmp_point, sizeof(Point), 1, file_pointer) == 1) {
        lidar_data.push_back(tmp_point);
    }

    std::cout << "Data loaded" << std::endl;

    fclose(file_pointer);

    std::cout << "File closed" << std::endl;

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