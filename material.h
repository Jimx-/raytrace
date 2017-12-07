//
// Created by jimx on 17-12-6.
//

#ifndef RAYTRACE_MATERIAL_H
#define RAYTRACE_MATERIAL_H

#include "frame_buffer.h"
#include "vector3.h"

#include <memory>

struct TexCoord {
    double u, v;

    TexCoord(double u, double v) : u(u), v(v) { }

    TexCoord operator*(double f) const
    {
        return {u * f, v * f};
    }

    TexCoord& operator*=(double f)
    {
        *this = *this * f;
        return *this;
    }
};

class Material;

using PMaterial = std::shared_ptr<Material>;

class Material {
public:
    double metallic, roughness;
    double reflectivity, refractive_index;

    static const int NO_REFLECTION = -1;
    static const int NO_REFRATION = -1;

    Color color;

    Material(double metallic, double roughness, const Color& color, double reflectivity = NO_REFLECTION, double refractive_index = NO_REFRATION) : metallic(metallic), roughness(roughness),
                                                                    reflectivity(reflectivity), refractive_index(refractive_index), color(color) {}

    virtual Color get_color(const TexCoord& texcoord) const { return color; }

    static const PMaterial AIR;
};


#endif //RAYTRACE_MATERIAL_H
