//
// Created by jimx on 17-12-7.
//

#include "triangle.h"

Intersection Triangle::intersect(const Ray& ray)
{
    Vector3 e1, e2, h, s, q, normal;
    double a, f, u, v, distance;

    e1 = Vector3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
    e2 = Vector3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

    normal = ((v1 - v0).cross_product(v2 - v0));
    normal.normalize();

    h = ray.dir.cross_product(e2);
    a = e1.dot_product(h);

    if (a > -10e-4 && a < 10e-4)
        return Intersection::missed();

    f = 1 / a;
    s = Vector3(ray.org.x - v0.x,
               ray.org.y - v0.y, ray.org.z - v0.z);

    u = f * s.dot_product(h);

    if (u < 0.0 || u > 1.0)
        return Intersection::missed();

    q = s.cross_product(e1);
    v = f * ray.dir.dot_product(q);

    if (v < 0.0 || u + v > 1.0)
        return Intersection::missed();

    distance = f * e2.dot_product(q);

    if (distance > 10e-4) {
        return {distance, normal};
    }

    return Intersection::missed();
}

TexCoord Triangle::map_texcoord(const Point3& world_pos)
{
    return {0, 0};
}
