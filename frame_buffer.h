//
// Created by jimx on 17-12-5.
//

#ifndef RAYTRACE_FRAME_BUFFER_H
#define RAYTRACE_FRAME_BUFFER_H

#include <string>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

struct Color {
    double r, g, b;

#define CLAMP(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

    explicit Color(double r = 0.0, double g = 0.0, double b = 0.0)
    {
        this->r = CLAMP(r, 0, 1);
        this->g = CLAMP(g, 0, 1);
        this->b = CLAMP(b, 0, 1);
    }

    explicit Color(uint32_t val)
    {
        int rc = (val >> 16) & 0xff;
        int gc = (val >> 8) & 0xff;
        int bc = (val) & 0xff;

        this->r = CLAMP(rc / 255.0f, 0, 1);
        this->g = CLAMP(gc / 255.0f, 0, 1);
        this->b = CLAMP(bc / 255.0f, 0, 1);
    }

    Color operator*(double f) const
    {
        return Color(r * f, g * f, b * f);
    }

    Color operator+(const Color& c) const
    {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator+(double f) const
    {
        return Color(r + f, g + f, b + f);
    }

    Color operator*(const Color& c) const
    {
        return Color(r * c.r, g * c.g, b * c.b);
    }

    Color& operator*=(double f)
    {
        *this = *this * f;
        return *this;
    }

    uint32_t color_value() const
    {
        uint32_t ri = (uint32_t)(r * 255);
        uint32_t gi = (uint32_t)(g * 255);
        uint32_t bi = (uint32_t)(b * 255);

        return (ri << 16) | (gi << 8) | (bi);
    }

    static Color lerp(const Color& l, const Color& r, double t)
    {
        return r * t + l * (1 - t);
    }
};

std::ostream& operator<<(std::ostream& os, const Color& color);

class FrameBuffer {
public:
    FrameBuffer(int width, int height, int channels = 3);
    ~FrameBuffer();

    void set_pixel(int x, int y, Color color);
    void write(const std::string& filename);

    int get_width() const
    {
        return width;
    }

    int get_height() const
    {
        return height;
    }

private:
    void alloc_buffer();

    int width, height, channels;
    uint8_t* framebuffer;
};

#endif //RAYTRACE_FRAME_BUFFER_H
