#include "visualizer.hpp"

#include <cstdlib>
#include <ctime>
#include <map>

Visualizer::Visualizer(const std::string& s) : 
    window_title{s},
    s_cam(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 384, 0.2, 100),
        pangolin::ModelViewLookAt(0, 0, 35, 0, 0, 0, pangolin::AxisX)
    ),
    handler(s_cam) {
    pangolin::CreateWindowAndBind(s, 1000, 800);
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

void Visualizer::visualize_ground_estimation(const std::vector<Point>& lidar_data) {
    if (!lidar_data.empty()) {
        while (!pangolin::ShouldQuit()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

            d_cam->Activate(s_cam);

            glPointSize(1.0);
            glBegin(GL_POINTS);

            for (const auto& p : lidar_data) {
                if ((p.prediction == 40) &&  (p.ground_truth == 40)) {
                    // green -> correctly guessed points
                    glColor3f(0.0, 1.0, 0.0);
                } else if ((p.prediction == 40) && (p.ground_truth != 40)) {
                    // red -> wrongly guessed points
                    glColor3f(1.0, 0.0, 0.0);
                } else if ((p.prediction != 40) && (p.ground_truth == 40)) {
                    // blue -> real ground points not guessed
                    glColor3f(0.0, 0.0, 1.0);
                } else {
                    glColor3f(1.0, 1.0, 1.0);
                }

                glVertex3d(p.get_x_value(), p.get_y_value(), p.get_z_value());
            }

            glEnd();
            pangolin::glDrawAxis(2);
            pangolin::FinishFrame();
        }
    }
}

void Visualizer::visualize_clusters(const std::vector<Point>& lidar_data) {
    if (lidar_data.empty()) {
        return;
    } else {
        std::vector<const Point*> sorted_clusters;

        for (auto& p : lidar_data) {
            sorted_clusters.push_back(&p);
        }

        std::sort(sorted_clusters.begin(), sorted_clusters.end(),
            [](const Point* a, const Point* b){ return (a->prediction < b->prediction); });

        std::map<uint32_t, pangolin::Colour> colours_for_clusters;
        srand(time(0));

        for (const Point* p_ptr : sorted_clusters) {
            if (p_ptr->prediction == 40) {
                colours_for_clusters[p_ptr->prediction] = pangolin::Colour(1.0, 0.0, 0.0);
            } else if (p_ptr->prediction == 0) {
                colours_for_clusters[p_ptr->prediction] = pangolin::Colour(1.0, 1.0, 1.0);
            } else {
                float r_value = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
                float g_value = static_cast<float>(rand() / static_cast<float>(RAND_MAX));
                float b_value = static_cast<float>(rand() / static_cast<float>(RAND_MAX));

                colours_for_clusters[p_ptr->prediction] = pangolin::Colour(r_value, g_value, b_value);
            }
        }

        while (!pangolin::ShouldQuit()) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

            d_cam->Activate(s_cam);

            glPointSize(1.0);
            glBegin(GL_POINTS);

            for (const Point* p_ptr : sorted_clusters) {
                pangolin::Colour tmp_colour = colours_for_clusters[p_ptr->prediction];
                glColor3f(tmp_colour.r, tmp_colour.g, tmp_colour.b);
                glVertex3d(p_ptr->get_x_value(), p_ptr->get_y_value(), p_ptr->get_z_value());
            }

            glEnd();
            pangolin::glDrawAxis(2);
            pangolin::FinishFrame();
        }
    }
}