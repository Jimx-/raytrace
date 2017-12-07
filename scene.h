//
// Created by jimx on 17-12-5.
//

#ifndef RAYTRACE_SCENE_H
#define RAYTRACE_SCENE_H

#include "frame_buffer.h"
#include "object.h"
#include "camera.h"
#include "mtl.h"

#include <memory>
#include <vector>
#include <tuple>

class Scene {
public:
    explicit Scene(int num_super_samples = 1);

	friend class mtl;

    void add_object(Object* object);
    void add_light(Light* light);
    void set_background(const Color& bg) { background_color = bg; }
    void set_camera(Camera* cam) { camera.reset(cam); }

    void render(FrameBuffer& framebuffer);
private:
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<std::unique_ptr<Light>> lights;
    std::unique_ptr<Camera> camera;
    Color background_color;

    int num_super_samples;
    double image_scale;

    std::tuple<Intersection, Object*> find_closest_intersection(const Ray& ray);
    bool shadow_test(const Ray& ray, double dist);

    Color render_pixel(double scr_x, double scr_y);
    Color trace(const Ray& ray);

    Color pbr_shading(PMaterial material, const Vector3& N, const Vector3& V, const Vector3& L, const Color& albedo, const Color& light_color, double light_intensity);
    Color calculate_reflection_refraction(PMaterial material, const Ray& ray, const Vector3& N, const Point3& intersection);
    double calculate_reflectance(const Vector3& normal, const Vector3& incident, double n1, double n2);
};


#endif //RAYTRACE_SCENE_H
