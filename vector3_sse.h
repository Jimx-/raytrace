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
			float x, y, z;
		};
		__m128 mmvalue;
	};

	explicit Vector3(float x = 0.0, float y = 0.0, float z = 0.0)
		: mmvalue(_mm_set_ps(0, z, y, x))
	{
	}

	explicit Vector3(__m128 m) : mmvalue(m) {}

	float operator[](size_t i) const
	{
		return *(&x + i);
	}

	float& operator[](size_t i)
	{
		return *(&x + i);
	}

	Vector3 operator+(const Vector3& v) const
	{
		return Vector3{_mm_add_ps(mmvalue, v.mmvalue)};
	}

	Vector3 operator-(const Vector3& v) const
	{
		return Vector3{_mm_sub_ps(mmvalue, v.mmvalue)};
	}

	Vector3 operator*(const Vector3& v) const
	{
		return Vector3{_mm_mul_ps(mmvalue, v.mmvalue)};
	}

	Vector3 operator*(float f) const
	{
		return Vector3{_mm_mul_ps(mmvalue, _mm_set1_ps(f))};
	}

	Vector3& operator*=(float f)
	{
		mmvalue = _mm_mul_ps(mmvalue, _mm_set1_ps(f));
		return *this;
	}

	float dot_product(const Vector3& v) const
	{
		return _mm_cvtss_f32(_mm_dp_ps(mmvalue, v.mmvalue, 0x71));
	}

	Vector3 cross_product(const Vector3& v) const
	{
		return Vector3{_mm_sub_ps(
			_mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 0, 2, 1)), _mm_shuffle_ps(v.mmvalue, v.mmvalue, _MM_SHUFFLE(3, 1, 0, 2))),
			_mm_mul_ps(_mm_shuffle_ps(mmvalue, mmvalue, _MM_SHUFFLE(3, 1, 0, 2)), _mm_shuffle_ps(v.mmvalue, v.mmvalue, _MM_SHUFFLE(3, 0, 2, 1))))};
	}

	float length() const
	{
		return _mm_cvtss_f32(_mm_dp_ps(mmvalue, mmvalue, 0x71));
	}

	void normalize()
	{
		mmvalue = _mm_mul_ps(mmvalue, _mm_rsqrt_ps(_mm_dp_ps(mmvalue, mmvalue, 0x7F)));
	}

	static Vector3 lerp(const Vector3& l, const Vector3& r, float t)
	{
		return r * t + l * (1 - t);
	}

	static Vector3 ZERO;
};

std::ostream& operator<<(std::ostream& os, const Vector3& v);

typedef Vector3 Point3;

#endif