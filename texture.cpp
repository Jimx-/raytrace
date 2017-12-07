//
// Created by jimx on 17-12-6.
//

#include <cstring>
#include "texture.h"

TextureMaterial::TextureMaterial(int width, int height, double metallic, double roughness, double reflectivity, double refractive_index)
    : Material(metallic, roughness, Color{}, reflectivity, refractive_index), width(width), height(height)
{
    texture_filter = TextureFilter::NEAREST;

    alloc_buffer();
}

void TextureMaterial::alloc_buffer()
{
    buffer.reset(new uint32_t[height * width]);
}

Color TextureMaterial::get_color(const TexCoord& texcoord) const
{
    int i, j;

    switch (texture_filter) {
    case TextureFilter::NEAREST:
        i = (int)(texcoord.u * width);
        j = (int)(texcoord.v * height);

        return Color(buffer[j * width + i]);
    }
}

void TextureMaterial::copy_data(uint32_t* texdata)
{
    ::memcpy(buffer.get(), texdata, height * width * sizeof(uint32_t));
}

