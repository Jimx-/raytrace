//
// Created by jimx on 17-12-6.
//

#ifndef RAYTRACE_TEXTURE_H
#define RAYTRACE_TEXTURE_H

#include "material.h"

#include <memory>

enum class TextureFilter {
    NEAREST,
    BILINEAR,
};

class TextureMaterial : public Material {
public:
    TextureMaterial(int width, int height, double metallic, double roughness, double reflectivity = Material::NO_REFLECTION, double refractive_index = Material::NO_REFRATION);

    void copy_data(unsigned char* texdata);
    virtual Color get_color(const TexCoord& texcoord) const;

    void set_filtering(TextureFilter mode) { texture_filter = mode; }

private:
    int width, height;
    TextureFilter texture_filter;
    std::unique_ptr<uint32_t[]> buffer;

    void alloc_buffer();
};


#endif //RAYTRACE_TEXTURE_H
