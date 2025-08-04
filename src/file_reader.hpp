#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <cstdio>
#include <math.h>

struct Point{
    float x_value;
    float y_value;
    float z_value;
    float ref_value;

    // constructor for the point
    Point(const float x_val, const float y_val, const float z_val, const float ref_val);

    // default constructor
    Point() = default;

    // getters
    float get_x_value() const;
    float get_y_value() const;
    float get_z_value() const;
    float get_ref_value() const;

    // getter for point angle
    float get_angle() const;

    // getter for point range
    float get_rho() const;

    // comparison operator for points
    bool operator<(const Point& other) const;
};

class ReadLiDARdata {
private:
    // std::string file_name;
    std::string file_path;
    float max_range;

public:
    // constructor for the class
    ReadLiDARdata(const std::string&& file_path_);

    // reading the data
    void read_data(std::vector<Point>& lidar_data);

    // printing the data
    void print_data(const std::vector<Point>& lidar_data);

    // get scene size
    void get_scene_size(const std::vector<Point>& lidar_data);

    // get max range
    float get_max_range() const;
};