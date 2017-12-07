//
// Created by jimx on 17-12-5.
//

#ifndef RAYTRACE_LIGHT_H
#define RAYTRACE_LIGHT_H


#include <limits>
#include "vector3.h"
#include "frame_buffer.h"
#include "material.h"

struct Light {
    Point3 position;
    Color color;
    double intensity;

    Light(const Point3& position, const Color& color, double intensity = 1.0) : position(position), color(color), intensity(intensity) {}
};

struct Ray {
    Point3 org;
    Vector3 dir;
    int depth;
    PMaterial material;

    static const int MAX_DEPTH = 6;

    Ray(const Point3& org, const Vector3& dir, int depth = 1, PMaterial material = Material::AIR) : org(org), dir(dir), depth(depth), material(material){

    }

    Point3 get_position(double t) const {
        return org + dir * t;
    }
};

struct Intersection {
    double t;
    Vector3 normal;
    bool hit;

    Intersection(double t, Vector3 normal, bool hit = true) : t(t), normal(normal), hit(hit) {
    }

    static Intersection missed() {
        static Intersection miss(std::numeric_limits<double>::infinity(), Vector3::ZERO, false);
        return miss;
    }
};

#endif //RAYTRACE_LIGHT_H
