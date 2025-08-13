#include "file_reader.hpp"
#include "visualizer.hpp"
#include "seq_algorithm.hpp"
#include "par_agorithm.hpp"

#include <filesystem>

int main() {

    // const std::string coordinates_directory = "/home/alessio/workspace/tum/par-comp/data-parallel-3d-ground-segmentation/labeled_pc/04/velodyne/";
    // const std::string labels_directory = "/home/alessio/workspace/tum/par-comp/data-parallel-3d-ground-segmentation/labeled_pc/04/labels/";

    // // getting a list of all bin files in the coordinates director
    // std::vector<std::filesystem::path> bin_paths;

    // for (const auto& path : std::filesystem::directory_iterator(coordinates_directory)) {
    //     if((path.is_regular_file()) && (path.path().extension() == ".bin")) {
    //         bin_paths.push_back(path.path());
    //     }
    // }

    // // sorting paths in alfabetical order
    // std::sort(bin_paths.begin(), bin_paths.end());

    // // getting a list of all label files in the labels directory
    // std::vector<std::filesystem::path> label_paths;

    // for (const auto& path : std::filesystem::directory_iterator(labels_directory)) {
    //     std::string filename = path.path().filename().string();

    //     if ((path.is_regular_file()) && (path.path().extension() == ".label") && (filename.rfind("._", 0) != 0)) {
    //         label_paths.push_back(path.path());
    //     }
    // }


    // // sorting paths in alfabetical order
    // std::sort(label_paths.begin(), label_paths.end());

    // if (bin_paths.size() == label_paths.size()) {
    //     std::cout << "They have the same size" << std::endl;

    //     for (int i = 0; i < bin_paths.size(); i++) {
    //         std::cout << "SCENE n. " << i << std::endl;

    //         std::vector<Point> lidar_data;

    //         ReadLiDARdata read_data(bin_paths[i], label_paths[i]);
    //         read_data.read_data(lidar_data);
    //         // read_data.get_scene_size(lidar_data);
    //         // std::cout << "Max range: " << read_data.get_max_range() << std::endl;
    //         // std::cout << "Number of real ground points: " << read_data.get_n_ground_points() << std::endl;

    //         Sequential s;
    //         // s.ground_estimation_and_clustering(lidar_data, 50, 14, read_data.get_max_range());
    //         s.point_clustering(lidar_data, 50, 14, read_data.get_max_range(), 0.3, 0.15, 0.2, 0.3);
    //     }
    // }

    std::vector<Point> sequential_lidar_data;
    std::vector<Point> parallel_lidar_data;

    std::string coordinates_file = "/home/alessio/workspace/tum/par-comp/"
        "data-parallel-3d-ground-segmentation/labeled_pc/04/velodyne/000000.bin";

    std::string labels_file = "/home/alessio/workspace/tum/par-comp/"
        "data-parallel-3d-ground-segmentation/labeled_pc/04/labels/000000.label";

    ReadLiDARdata seq_readLiDAR(coordinates_file, labels_file);
    ReadLiDARdata par_readLiDAR(coordinates_file, labels_file);
    seq_readLiDAR.read_data(sequential_lidar_data);
    par_readLiDAR.read_data(parallel_lidar_data);

    Sequential sequential;
    sequential.seq_point_clustering(sequential_lidar_data, 50, 14, seq_readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);

    Parallel parallel(2);
    parallel.par_point_clustering(parallel_lidar_data, 50, 14, par_readLiDAR.get_max_range(), 0.3, 0.15, 0.2, 0.3);
    
}

