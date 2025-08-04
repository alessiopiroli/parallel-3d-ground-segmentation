#pragma once

#include "file_reader.hpp"
#include <pangolin/pangolin.h>

#include <memory>

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
};