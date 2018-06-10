#include "frame_buffer.h"
#include "sphere.h"
#include "scene.h"
#include "camera.h"
#include "triangle.h"

#include "config_reader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

int main() {
    FrameBuffer framebuffer(1920, 1080);

    auto mat1 = std::make_shared<Material>(0.2, 0.2, Color{0.0, 0.0, 0.0}, 0.4, 1.5);
    auto mat2 = std::make_shared<Material>(0.2, 0.4, Color{1.0, 0.0, 0.0});

    Sphere* sp = new Sphere(Point3{400, 225, 0}, 75, mat1);
    Sphere* sp2 = new Sphere(Point3{0, -100000, 0}, 100000, mat2);
   //Sphere* sp2 = new Sphere(Point3{210, 270, 300}, 50, mat1);

    Light* light = new Light(Point3{900.0, 900.0, -100.0}, Color{1.0, 1.0, 1.0}, 1.0);
    Light* light2 = new Light(Point3{-200, 600, 1500}, Color{1.0, 1.0, 1.0}, 2.0);

    Camera* camera = new Camera(Point3{995, 400, 400}, Point3{0, 1, 0}, Point3{100, 0, 0}, 1500);

    Scene scene(1);
    //scene.add_object(sp);
    //scene.add_object(sp2);
    //scene.add_object(sp3);

    //scene.add_object(floor);
    //scene.add_object(left_wall);
    //scene.add_object(right_wall);

    scene.add_light(light);
    //scene.add_light(light2);
    scene.set_camera(camera);
    scene.set_background(Color{1.0f, 0.8f, 0.0f});
    //scene.render(framebuffer);
    Scene* scene1 = load_scene("../scene2.json");
    scene1->render(framebuffer);

    framebuffer.write("a.png");

    return 0;
}
