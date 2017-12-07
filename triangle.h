//
// Created by jimx on 17-12-7.
//

#ifndef RAYTRACE_TRIANGLE_H
#define RAYTRACE_TRIANGLE_H

#include "object.h"

class Triangle : public Object {
public:
    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2, PMaterial material) : Object(std::move(material)), v0(v0), v1(v1), v2(v2) { }

    Intersection intersect(const Ray& ray) override;
    TexCoord map_texcoord(const Point3& world_pos) override;

private:
    Vector3 v0, v1, v2;
};

#endif //RAYTRACE_TRIANGLE_H
