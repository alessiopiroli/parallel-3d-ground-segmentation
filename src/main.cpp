#include "file_reader.hpp"
#include "visualizer.hpp"

int main() {
    std::vector<Point> lidar_data;
    
    ReadLiDARdata rd("/home/alessio/workspace/tum/par-comp/data-parallel-3d-ground-segmentation/2011_09_26_drive_0035_sync/2011_09_26/2011_09_26_drive_0035_sync/velodyne_points/data/0000000000.bin");
    rd.read_data(lidar_data);
    rd.get_scene_size(lidar_data);
    Visualizer v("Simple LiDAR visualization");
    v.simple_visualization(lidar_data);
}