
#ifndef _VECTOR3_H_
#define _VECTOR3_H_

#include <cstddef>
#include <cmath>
#include <iostream>
#include <smmintrin.h>

class Vector3 {
public:
    union {
        struct {
            double x, y, z;
        };
        __m128d mmvalue;
    };

    Vector3(double x = 0.0, double y = 0.0, double z = 0.0)
        : x(x), y(y), z(z)
    {
    }

    Vector3(__m128d m) : mmvalue(m) {}


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
        return Vector3{_mm_add_pd(mmvalue, v.mmvalue)};
    }

    Vector3 operator-(const Vector3& v) const
    {
        return Vector3{_mm_sub_pd(mmvalue, v.mmvalue)};
    }

    Vector3 operator*(const Vector3& v) const
    {
        return Vector3{_mm_mul_pd(mmvalue, v.mmvalue)};
    }

    Vector3 operator*(double f) const
    {
        return Vector3{_mm_mul_pd(mmvalue, _mm_set1_pd(f))};
    }

    Vector3& operator*=(double f)
    {
        mmvalue = _mm_mul_pd(mmvalue, _mm_set1_pd(f));
        return *this;
    }

    double dot_product(const Vector3& v) const
    {
        return _mm_cvtsd_f64(_mm_dp_pd(mmvalue, v.mmvalue, 0x71));
    }

    Vector3 cross_product(const Vector3& v) const
    {
        return Vector3{_mm_sub_pd(
    _mm_mul_pd(_mm_shuffle_pd( mmvalue,  mmvalue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_pd( v.mmvalue,  v.mmvalue, _MM_SHUFFLE(3, 1, 0, 2))),
    _mm_mul_pd(_mm_shuffle_pd( mmvalue,  mmvalue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_pd( v.mmvalue,  v.mmvalue, _MM_SHUFFLE(3, 0, 2, 1)))
    )};
    }

    double length() const
    {
        return _mm_cvtsd_f64(_mm_dp_pd(mmvalue, mmvalue, 0x71));
    }

    void normalize()
    {
        double len = length();
        if (len != 0) {
            x /= len;
            y /= len;
            z /= len;
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
