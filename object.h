//
// Created by jimx on 17-12-5.
//

#ifndef RAYTRACE_OBJECT_H
#define RAYTRACE_OBJECT_H


#include "light.h"
#include "material.h"
#include "texture.h"

class Object {
public:
    Object(PMaterial material) : material(material) { }

    virtual Intersection intersect(const Ray& ray) = 0;
    virtual TexCoord map_texcoord(const Point3& world_pos) = 0;
    PMaterial get_material() const { return material; }

private:
    PMaterial material;
};


#endif //RAYTRACE_OBJECT_H
