//
// Created by jimx on 17-12-5.
//

#include "sphere.h"
#include "const.h"

#include <iostream>

using namespace std;

Intersection Sphere::intersect(const Ray& ray)
{
    Vector3 deltap = ray.org - center;
    double a = ray.dir.dot_product(ray.dir);
    double b = deltap.dot_product(ray.dir) * 2;
    double c = deltap.dot_product(deltap) - (radius * radius);

    double disc = b * b - 4 * a * c;
    if (disc < 0) {
        return Intersection::missed();
    }

    disc = sqrt(disc);

    double q;
    if (b < 0) {
        q = (-b - disc) * 0.5;
    } else {
        q = (-b + disc) * 0.5;
    }

    double r1 = q / a;
    double r2 = c / q;

    if (r1 > r2) {
        double tmp = r1;
        r1 = r2;
        r2 = tmp;
    }

    double distance = r1;
    if (distance < 0) {
        distance = r2;
    }

    if (distance < 10e-4 || isnan(distance)) {
        return Intersection::missed();
    }

    Vector3 point = ray.get_position(distance);
    Vector3 normal = point - center;

    if (ray.dir.dot_product(normal) > 0) {
        normal = normal * -1;
    }

    return {distance, normal};
}

TexCoord Sphere::map_texcoord(const Point3& world_pos)
{
    Vector3 polar = world_pos - center;

    double u = acos(polar.y / radius) * INV_PI;
    double v = acos(polar.x / (radius * sin(u * PI))) * 0.5 * INV_PI;

    return {u, v};
}
