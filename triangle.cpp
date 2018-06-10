//
// Created by jimx on 17-12-7.
//
#include <iostream>

#include "triangle.h"

using namespace std;

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
        // Prepare our barycentric variables
        Vector3 u = v1 - v0;
        Vector3 v = v2 - v0;
        Vector3 w = ray.get_position(distance) - v0;

        Vector3 vCrossW = v.cross_product(w);
        Vector3 uCrossW = u.cross_product(w);
        Vector3 uCrossV = u.cross_product(v);

        // At this point, we know that r and t and both > 0.
        // Therefore, as long as their sum is <= 1, each must be less <= 1
        double denom = uCrossV.length();
        double r = vCrossW.length() / denom;
        double t = uCrossW.length() / denom;

        return {distance, n0 + (n1 - n0) * r + (n2 - n0) * t};
    }

    return Intersection::missed();
}

TexCoord Triangle::map_texcoord(const Point3& world_pos)
{
    // Prepare our barycentric variables
    Vector3 u = v1 - v0;
    Vector3 v = v2 - v0;
    Vector3 w = world_pos - v0;

    Vector3 vCrossW = v.cross_product(w);
    Vector3 vCrossU = v.cross_product(u);

    // Test sign of r
    if (vCrossW.dot_product(vCrossU) < 0)
        return {0, 0};

    Vector3 uCrossW = u.cross_product(w);
    Vector3 uCrossV = u.cross_product(v);

    // Test sign of t
    if (uCrossW.dot_product(uCrossV) < 0)
        return {0, 0};

    // At this point, we know that r and t and both > 0.
    // Therefore, as long as their sum is <= 1, each must be less <= 1
    double denom = uCrossV.length();
    double r = vCrossW.length() / denom;
    double t = uCrossW.length() / denom;

    return {uv0.u + r * (uv1.u - uv0.u) + t * (uv2.u - uv0.u),
            uv0.v + r * (uv1.v - uv0.v) + t * (uv2.v - uv0.v)};
}
