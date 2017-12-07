#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <cstddef>
#include <cmath>
#include <iostream>

struct Vector3 {
public:
    double x, y, z;

    explicit Vector3(double x = 0.0, double y = 0.0, double z = 0.0)
    : x(x), y(y), z(z)
    {
    }

    double operator[](size_t i) const
    {
        return *(&x + i);
    }

    double& operator[](size_t i)
    {
        return *(&x + i);
    }

    Vector3 operator+(const Vector3& v) const
    {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3 operator-(const Vector3& v) const
    {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3 operator*(const Vector3& v) const
    {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    Vector3 operator*(double f) const
    {
        return Vector3(x * f, y * f, z * f);
    }

    Vector3& operator*=(double f)
    {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    double dot_product(const Vector3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 cross_product(const Vector3& v) const
    {
        double m1, m2, m3;
        m1 = y * v.z - z * v.y;
        m2 = z * v.x - x * v.z;
        m3 = x * v.y - y * v.x;
        return Vector3(m1, m2, m3);
    }

    double length() const
    {
        double s = x * x + y * y + z * z;
        return (double) sqrt((double)s);
    }

    void normalize()
    {
        double len = length();
        if (len != (double)0.0) {
            double inv = 1 / len;
            x *= inv;
            y *= inv;
            z *= inv;
        }
    }

    static Vector3 lerp(const Vector3& l, const Vector3& r, double t)
    {
        return r * t + l * (1 - t);
    }

    static Vector3 ZERO;
};

std::ostream& operator<<(std::ostream& os, const Vector3& v);

typedef Vector3 Point3;

#endif
