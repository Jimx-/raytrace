//
// Created by jimx on 17-12-6.
//

#ifndef RAYTRACE_CAMERA_H
#define RAYTRACE_CAMERA_H


#include "vector3.h"

class Camera {
public:
    explicit Camera(const Vector3& position = Vector3{0, 0, 100.0f}, const Vector3& up = Vector3{0, 1, 0}, const Vector3& look_at = Vector3{0, 0, 0}, int screen_width = 1500);

    int get_screen_width() const { return screen_width; }
    Vector3 get_position() const { return position; }
    Vector3 transform(double x, double y, double scale_factor) const;

private:
    Vector3 position, up, look_at;
    Vector3 w, u, v;
    int screen_width;

    void calculate_basis();
};


#endif //RAYTRACE_CAMERA_H
