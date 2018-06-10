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
    case TextureFilter::BILINEAR:
        i = (int)(texcoord.u * width);
        j = (int)(texcoord.v * height);
        {
            double u_ratio = texcoord.u * width - i;
            double v_ratio = texcoord.v * height - j;
            double u_opposite = 1 - u_ratio;
            double v_opposite = 1 - v_ratio;
            return (Color(buffer[j * width + i]) * u_opposite  + Color(buffer[j * width + i + 1])   * u_ratio) * v_opposite +
                            (Color(buffer[(j + 1) * width + i]) * u_opposite  + Color(buffer[(j + 1) * width + (i + 1)]) * u_ratio) * v_ratio;
        }
    }
}

void TextureMaterial::copy_data(unsigned char* texdata)
{
    for (int i = 0; i < height * width; i++) {
        buffer[i] = (texdata[i * 3] << 16) | (texdata[i * 3 + 1] << 8) | texdata[i * 3 + 2];
    }
}

