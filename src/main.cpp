#include "file_reader.hpp"
#include "visualizer.hpp"
#include "seq_algorithm.hpp"

int main() {
    std::vector<Point> lidar_data;
    
    ReadLiDARdata rd("/home/alessio/workspace/tum/par-comp/data-parallel-3d-ground-segmentation/2011_09_26_drive_0035_sync/2011_09_26/2011_09_26_drive_0035_sync/velodyne_points/data/0000000000.bin");
    rd.read_data(lidar_data);
    rd.get_scene_size(lidar_data);
    std::cout << "Max range: " << rd.get_max_range() << std::endl;
    
    Sequential s;
    s.ground_estimation(lidar_data, 720, 300, rd.get_max_range());
}

