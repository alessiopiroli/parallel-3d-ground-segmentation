#pragma once

#include "file_reader.hpp"
#include <pangolin/pangolin.h>

#include <memory>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>

class Visualizer {
private:
    pangolin::OpenGlRenderState s_cam;
    pangolin::Handler3D handler;
    pangolin::View* d_cam;
    std::string window_title;

public:
    Visualizer(const std::string& s);
    ~Visualizer();
    void simple_visualization(const std::vector<Point>& lidar_data);
    void visualize_protype_points(const std::vector<Point>& lidar_data, const std::vector<Point>& prototype_points);
    void timed_visualization(const std::vector<Point>& lidar_data, const std::vector<Point>& prototype_points);
    void visualize_ground_estimation(const std::vector<Point>& lidar_data);
};