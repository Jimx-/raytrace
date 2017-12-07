//
// Created by jimx on 17-12-5.
//

#ifndef RAYTRACE_SPHERE_H
#define RAYTRACE_SPHERE_H

#include "object.h"

class Sphere : public Object {
public:
    Sphere(const Vector3& pos, double r, PMaterial material) : Object(std::move(material)), center(pos), radius(r) {}

    Intersection intersect(const Ray& ray) override;
    TexCoord map_texcoord(const Point3& world_pos) override;

private:
    Vector3 center;
    double radius;
};


#endif //RAYTRACE_SPHERE_H
