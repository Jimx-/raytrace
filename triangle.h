//
// Created by jimx on 17-12-7.
//

#ifndef RAYTRACE_TRIANGLE_H
#define RAYTRACE_TRIANGLE_H

#include "object.h"

class Triangle : public Object {
public:
    Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2,
             const TexCoord& uv0, const TexCoord& uv1, const TexCoord& uv2,
             const Vector3& n0, const Vector3& n1, const Vector3& n2, PMaterial material) : Object(std::move(material)), v0(v0), v1(v1), v2(v2), uv0(uv0), uv1(uv1), uv2(uv2), n0(n0), n1(n1), n2(n2) { }

    Intersection intersect(const Ray& ray) override;
    TexCoord map_texcoord(const Point3& world_pos) override;

private:
    Vector3 v0, v1, v2;
    TexCoord uv0, uv1, uv2;
    Vector3 n0, n1, n2;
};

#endif //RAYTRACE_TRIANGLE_H
