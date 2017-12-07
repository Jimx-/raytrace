//
// Created by jimx on 17-12-6.
//

#include <iostream>
#include "camera.h"

using namespace std;

Camera::Camera(const Vector3& position, const Vector3& up, const Vector3& look_at, int screen_width)
    : position(position), up(up), look_at(look_at), screen_width(screen_width)
{
    calculate_basis();
}

void Camera::calculate_basis()
{
    w = look_at - position;
    w.normalize();
    u = up.cross_product(w);
    u.normalize();
    v = w.cross_product(u);
}

Vector3 Camera::transform(double x, double y, double scale_factor) const
{
    return look_at - (u * x * scale_factor) - (v * y * scale_factor);
}
