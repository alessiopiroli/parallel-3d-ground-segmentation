#include "visualizer.hpp"

#include <cstdlib>
#include <ctime>

Visualizer::Visualizer(const std::string& s) : 
    window_title{s},
    s_cam(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 384, 0.2, 100),
        pangolin::ModelViewLookAt(0, 0, 35, 0, 0, 0, pangolin::AxisX)
    ),
    handler(s_cam) {
    pangolin::CreateWindowAndBind(s, 500, 600);
    glEnable(GL_DEPTH_TEST);

    d_cam = &pangolin::CreateDisplay()
        .SetBounds(0.0, 1.0, 0.0, 1.0, -1024.0f / 768.0f)
        .SetHandler(&handler);    
}

Visualizer::~Visualizer() {
    pangolin::DestroyWindow(window_title);
}

void Visualizer::simple_visualization(const std::vector<Point>& lidar_data) {
    if (!lidar_data.empty()) {
        while (!pangolin::ShouldQuit()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

            d_cam->Activate(s_cam);

            glPointSize(1.0f);
            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0);

            for(const auto& p: lidar_data) {
                glVertex3f(p.x_value, p.y_value, p.z_value);
            }

            glEnd();


            pangolin::glDrawAxis(1);

            pangolin::FinishFrame();
        }
    }
}

void Visualizer::visualize_protype_points(const std::vector<Point>& lidar_data, const std::vector<Point>& prototype_points) {
    // we check that none of the two vectors are empty
    if (!lidar_data.empty() && !prototype_points.empty()) {
        while (!pangolin::ShouldQuit()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

            d_cam->Activate(s_cam);

            // printing all normal LiDAR points
            glPointSize(1.0f); // standard size for all points
            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0); // printing all points in white

            // printing all points in the main point cloud
            for (const auto& p : lidar_data) {
                glVertex3f(p.get_x_value(), p.get_y_value(), p.get_z_value());
            }

            glEnd();

            // printing all prototype points
            glPointSize(2.5); // bigger size for prototype points
            glBegin(GL_POINTS);
            glColor3f(1.0, 0.0, 0.0);

            // printing all points in the prototype cloud
            for (const auto& p : prototype_points) {
                glVertex3f(p.get_x_value(), p.get_y_value(), p.get_z_value());
            }

            glEnd();
            pangolin::glDrawAxis(2); // printing the three axis in size 2
            pangolin::FinishFrame();
        }
    }
}

void Visualizer::timed_visualization(const std::vector<Point>& lidar_data, const std::vector<Point>& prototype_points) {
    // we check that none of the two vectors are empty
    if (!lidar_data.empty() && !prototype_points.empty()) {
        const auto window_duration = std::chrono::seconds(1);
        auto start_time = std::chrono::steady_clock::now();

        while (std::chrono::steady_clock::now() - start_time < window_duration) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

            d_cam->Activate(s_cam);

            // printing all normal LiDAR points
            glPointSize(1.0f); // standard size for all points
            glBegin(GL_POINTS);
            glColor3f(1.0, 1.0, 1.0); // printing all points in white

            // printing all points in the main point cloud
            for (const auto& p : lidar_data) {
                glVertex3f(p.get_x_value(), p.get_y_value(), p.get_z_value());
            }

            glEnd();

            // printing all prototype points
            glPointSize(2.5); // bigger size for prototype points
            glBegin(GL_POINTS);
            glColor3f(1.0, 0.0, 0.0);

            // printing all points in the prototype cloud
            for (const auto& p : prototype_points) {
                glVertex3f(p.get_x_value(), p.get_y_value(), p.get_z_value());
            }

            glEnd();
            pangolin::glDrawAxis(2); // printing the three axis in size 2
            pangolin::FinishFrame();
        }
    }
}