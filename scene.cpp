//
// Created by jimx on 17-12-5.
//

#include "scene.h"
#include "const.h"

#include <iostream>
#include <limits>
#include <queue>
#include <omp.h>

using namespace std;

inline static double saturate(double x)
{
    return max(min(x, 1.0), 0.0);
}

Scene::Scene(int num_super_samples) : num_super_samples(num_super_samples)
{
    background_color = Color{0.0, 0.0, 0.0};
}

void Scene::render(FrameBuffer& framebuffer)
{
    int w = framebuffer.get_width();
    int h = framebuffer.get_height();
    image_scale = (double) camera->get_screen_width() / w;

    double sample_weight = 1.0 / (num_super_samples * num_super_samples);
#pragma omp parallel for collapse(2) shared(w, h)
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Color color{};
            double scr_x = (x - w / 2) / 2.0, scr_y = (y - h / 2) / 2.0;
            double sample_step = 0.5 / num_super_samples;
            double sample_start_x = scr_x - 0.25;
            double sample_start_y = scr_y - 0.25;

            for (int i = 0; i < num_super_samples; i++) {
                for (int j = 0; j < num_super_samples; j++) {
                    color = color + render_pixel(sample_start_x + i * sample_step, sample_start_y + j * sample_step) * sample_weight;
                }
            }
            framebuffer.set_pixel(x, y, color);
        }
    }
}

Color Scene::render_pixel(double scr_x, double scr_y)
{
    Vector3 scr_point = camera->transform(scr_x, scr_y, image_scale);
    Vector3 dir = scr_point - camera->get_position();
    dir.normalize();
    Ray ray(camera->get_position(), dir);

    return trace(ray);
}

void Scene::add_object(Object* object)
{
    objects.emplace_back(object);
}

Color Scene::pbr_shading(PMaterial material, const Vector3& N, const Vector3& V, const Vector3& L, const Color& albedo, const Color& light_color, double light_intensity)
{
    Color diffuse_term = albedo * (1.0f - material->metallic) * INV_PI;
    Color specular_color = Color::lerp(Color{0.9, 0.9, 0.9}, albedo, material->metallic);

    Vector3 H = L + V;
    H.normalize();
    double NdotL = saturate(N.dot_product(L));
    double NdotV = saturate(N.dot_product(V));
    double NdotH = saturate(N.dot_product(H));
    double LdotH = saturate(L.dot_product(H));

    // Distribution GGX
    double m = material->roughness * material->roughness;
    double m2 = m * m;
    double d = (NdotH * m2 - NdotH) * NdotH + 1;
    double D = m2 / (d*d);

    // Geometry Visibility
    double k = m * 0.5;
    double G_SchlickV = NdotV * (1 - k) + k;
    double G_SchlickL = NdotL * (1 - k) + k;
    double G = 0.25 / (G_SchlickV * G_SchlickL);

    // Fresnel
    Color s2 = Color{1 - specular_color.r, 1 - specular_color.g, 1 - specular_color.b};
    Color F = specular_color + s2 * pow((1 - LdotH), 5);
    Color specular_term = F * D * G * 0.25;

    Color brdf = diffuse_term * (1 - material->metallic) + specular_term * material->metallic;
    return brdf * light_color * light_intensity * max(NdotL, 0.0) * PI;
}

double Scene::calculate_reflectance(const Vector3& normal, const Vector3& incident, double n1, double n2) {
   double n = n1 / n2;
   double cosI = -normal.dot_product(incident);
   double sinT2 = n * n * (1.0 - cosI * cosI);

   if (sinT2 > 1.0) {
      // Total Internal Reflection.
      return 1.0;
   }

   double cosT = sqrt(1.0 - sinT2);
   double r0rth = (n1 * cosI - n2 * cosT) / (n1 * cosI + n2 * cosT);
   double rPar = (n2 * cosI - n1 * cosT) / (n2 * cosI + n1 * cosT);
   return (r0rth * r0rth + rPar * rPar) / 2.0;
}

Color Scene::calculate_reflection_refraction(PMaterial material, const Ray& ray, const Vector3& N, const Point3& intersection)
{
    int depth = ray.depth;
    if (depth >= Ray::MAX_DEPTH || material->refractive_index == Material::NO_REFRATION && material->reflectivity == Material::NO_REFLECTION) return Color{};

    double reflect_ratio = material->reflectivity;
    double refract_ratio = 0;

    PMaterial entrance_material = ray.material;
    if (material->refractive_index != Material::NO_REFRATION) {
        reflect_ratio = calculate_reflectance(N, ray.dir, entrance_material->refractive_index, material->refractive_index);
        refract_ratio = 1 - reflect_ratio;
    }

    Color reflect_color;
    if (reflect_ratio > 0) {
        Vector3 L = ray.dir * (-1.0);
        Vector3 reflection_dir = N * 2 * L.dot_product(N) - L;

        Ray reflection_ray(intersection, reflection_dir, depth + 1, entrance_material);
        reflect_color = trace(reflection_ray) * reflect_ratio;
    }

    Color refract_color;
    if (refract_ratio > 0) {
        double n =  entrance_material->refractive_index / material->refractive_index;
        double cosI = -N.dot_product(ray.dir);
        double sinT2 = n * n * (1.0 - cosI * cosI);

        double cosT = sqrt(1.0 - sinT2);
        Vector3 refraction_dir = ray.dir * n + N * (n * cosI - cosT);

        Ray refraction_ray(intersection, refraction_dir, depth + 1, material);
        refract_color = trace(refraction_ray) * refract_ratio;
    }
    return reflect_color + refract_color;
}

tuple<Intersection, Object*> Scene::find_closest_intersection(const Ray& ray)
{
    Object* obj = nullptr;
    Intersection intersection = Intersection::missed();

#pragma omp parallel shared(intersection, obj)
    {
        Object* local_obj = nullptr;
        Intersection local_intersection = intersection;
#pragma omp for nowait
        for (auto it = objects.begin(); it < objects.end(); ++it) {
            Intersection hit = (*it)->intersect(ray);
            if (!hit.hit) continue;

            if (hit.t < local_intersection.t) {
                local_obj = it->get();
                local_intersection = hit;
            }
        }

#pragma omp critical
        if (local_intersection.t < intersection.t) {
            intersection = local_intersection;
            obj = local_obj;
        }
    }

    return make_tuple(intersection, obj);
}

Color Scene::trace(const Ray& ray)
{
    Color Ic{0.0, 0.0, 0.0};
    Intersection intersection = Intersection::missed();
    Object* obj;

    if (ray.depth > Ray::MAX_DEPTH) return Color{0, 0, 0};

    tie(intersection, obj) = find_closest_intersection(ray);

    // No hit? Return background color.
    if (!obj || !obj->get_material()) {
        return background_color;
    }

    PMaterial material = obj->get_material();
    Point3 intersection_point = ray.get_position(intersection.t);
    Vector3 V = ray.dir * -1.0;
    Vector3 N = intersection.normal;
    N.normalize();

    auto texcoord = obj->map_texcoord(intersection_point);
    Color albedo = material->get_color(texcoord);

    // Perform PBR shading
#pragma omp parallel for
    for (auto it = lights.begin(); it < lights.end(); ++it) {
        const auto& light = *it;
        Vector3 light_dir = light->position - intersection_point;
        double light_distance = light_dir.length();
        light_dir.normalize();

        /*Vector3 R = N * 2 * light_dir.dot_product(N) - N;
        R.normalize();
        double rv = max(0.0f, V.dot_product(R));

        Color diffuse = light->color * material->color * max(0.0f, N.dot_product(light_dir));
        Color specular = light->color * max(0.0f, (double)pow(rv, 100)); */

        Ray shadow_ray(light->position, light_dir * -1.0);
        Color diff_spec;
        if (!shadow_test(shadow_ray, light_distance)) {
            diff_spec = pbr_shading(material, N, V, light_dir, albedo, light->color, light->intensity);
        }
#pragma omp critical
        Ic = Ic + diff_spec;
    }
    Color reflect_refract = calculate_reflection_refraction(material, ray, N, intersection_point);
    Ic = Ic + reflect_refract;

    return Ic;
}

void Scene::add_light(Light* light)
{
    lights.emplace_back(light);
}

bool Scene::shadow_test(const Ray& ray, double dist)
{
    Intersection intersection = Intersection::missed();
    Object* obj;

    tie(intersection, obj) = find_closest_intersection(ray);

    //if (obj->get_material()->refractive_index != Material::NO_REFRATION) return false;
    return intersection.hit && intersection.t < dist - 10e-4;
}
