#include "vector3.h"

Vector3 Vector3::ZERO(0.0, 0.0, 0.0);

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}
