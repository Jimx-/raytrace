#include "frame_buffer.h"
#include "sphere.h"
#include "scene.h"
#include "camera.h"
#include "triangle.h"

#include <iostream>
#include <fstream>

using namespace std;

void init_texture(uint32_t* tex)
{
    for (int j = 0; j < 512; j++) {
        for (int i = 0; i < 512; i++) {
            tex[j * 512 + i] = ((i / 32 + j / 32) & 1) ? 0xff : 0xffffffff;
        }
    }
}

void add_mesh(Scene& scene)
{
    std::ifstream is("bunny.obj", std::ios::in);
    if (!is.is_open()) return;

    auto mat = std::make_shared<Material>(0.2, 0.4, Color{1.0, 0.0, 0.0});
    vector<Vector3> vertices;
    vector<int> indices;
    int faces = 0;

    char cmd;
    char line[100];
    while (is >> cmd) {
        if (cmd == '#') {
            is.getline(line, 100);
        }
        if (cmd == 'v') {
            float x, y, z;
            is >> x >> y >> z;
            vertices.emplace_back(Vector3{x * 3000, y * 3000 - 300, z * 3000});
        }
        if (cmd == 'f') {
            int v1, v2, v3;
            is >> v1 >> v2 >> v3;
            indices.push_back(v1-1);
            indices.push_back(v2-1);
            indices.push_back(v3-1);
            faces++;
        }
    }

    for (int i = 0; i < indices.size(); i += 3) {
        scene.add_object(new Triangle(vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]], mat));
    }
}

int main() {
    FrameBuffer framebuffer(1920, 1080);

    auto mat1 = std::make_shared<Material>(0.2, 0.2, Color{0.0, 0.0, 0.0}, 0.4, 1.5);
    auto mat2 = std::make_shared<Material>(0.2, 0.4, Color{1.0, 0.0, 0.0});

    auto* tex = new uint32_t[512 * 512];
    init_texture(tex);
    auto mat_tex = std::make_shared<TextureMaterial>(512, 512, 0.2, 0.2);
    mat_tex->copy_data(tex);
    delete[] tex;

    Sphere* sp = new Sphere(Point3{400, 225, 0}, 75, mat1);
    Sphere* sp2 = new Sphere(Point3{0, -100000, 0}, 100000, mat2);
    Sphere* sp3 = new Sphere(Point3{100, 75, 0}, 75, mat_tex);
    //Sphere* sp2 = new Sphere(Point3{210, 270, 300}, 50, mat1);

    Light* light = new Light(Point3{900.0, 900.0, 0.0}, Color{1.0, 1.0, 1.0}, 1.0);
    Light* light2 = new Light(Point3{-200, 600, 1500}, Color{1.0, 1.0, 1.0}, 2.0);

    Camera* camera = new Camera(Point3{995, 400, 0}, Point3{0, 1, 0}, Point3{100, 0, 0}, 1500);

    Scene scene(1);
    //scene.add_object(sp);
    //scene.add_object(sp2);
    //scene.add_object(sp3);
    add_mesh(scene);

    //scene.add_object(floor);
    //scene.add_object(left_wall);
    //scene.add_object(right_wall);

    scene.add_light(light);
    //scene.add_light(light2);
    scene.set_camera(camera);
    scene.set_background(Color{1.0f, 0.8f, 0.0f});
    scene.render(framebuffer);

    framebuffer.write("a.png");

    return 0;
}
