//
// Created by jimx on 17-12-5.
//

#include "frame_buffer.h"
#include "svpng.inc"

#include <iostream>

using namespace std;

FrameBuffer::FrameBuffer(int width, int height, int channels)
{
    this->width = width;
    this->height = height;
    this->channels = channels;
    framebuffer = nullptr;

    alloc_buffer();
}

FrameBuffer::~FrameBuffer()
{
    delete[] framebuffer;
}

void FrameBuffer::alloc_buffer()
{
    delete[] framebuffer;
    framebuffer = new uint8_t[width * height * channels];
}

void FrameBuffer::set_pixel(int x, int y, Color color)
{
    if (!framebuffer) return;
    int colori = color.color_value();

    for (int i = 0; i < channels; i++) {
        framebuffer[y * width * channels + x * channels + i] = (uint8_t)(colori >> ((channels - i - 1) * 8) & 0xff);
    }
}

void FrameBuffer::write(const std::string& filename)
{
    FILE *fp = fopen(filename.c_str(), "wb");

    svpng(fp, width, height, framebuffer, 0);
    fclose(fp);
}

std::ostream& operator<<(std::ostream& os, const Color& color)
{
    os << "(" << color.r << ", " << color.g << ", " << color.b << ")";
    return os;
}
