#include "visualizer.hpp"


Visualizer::Visualizer(const std::string& s) : 
    window_title{s},
    s_cam(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 384, 0.2, 100),
        pangolin::ModelViewLookAt(0, 0, 25, 0, 0, 0, pangolin::AxisY)
    ),
    handler(s_cam) {
    pangolin::CreateWindowAndBind(s, 1500, 1500);
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

            glPointSize(2.0f);
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