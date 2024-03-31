#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/closest_point.hpp>
#include <vector>

namespace singe {
    using std::vector;
    using glm::vec3;

    struct AABB {
        vec3 a, b;

        AABB() : AABB(vec3(0), vec3(0)) {}

        AABB(const vec3 & a, const vec3 & b) : a(a), b(b) {}

        vector<vec3> points() const {
            vec3 p = glm::min(a, b);
            vec3 d = glm::max(a, b) - p;

            return vector<vec3> {
                {p.x, p.y, p.z},
                {p.x + d.x, p.y, p.z}, // +x
                {p.x, p.y + d.y, p.z}, // +y
                {p.x, p.y, p.z + d.z}, // +z
                {p.x + d.x, p.y + d.y, p.z}, // +xy
                {p.x + d.x, p.y, p.z + d.z}, // +xz
                {p.x, p.y + d.y, p.z + d.z}, // +yz
                {p.x + d.x, p.y + d.y, p.z + d.z}, // +xyz
            };
        }
    };

    struct Sphere {
        vec3 p;
        float r;

        Sphere() : Sphere(0, 0, 0, 0) {}
        Sphere(float x, float y, float z, float r) : p(x, y, z), r(r) {}
        Sphere(const vec3 & p, float r) : p(p), r(r) {}
    };

    struct Ray {
        float x, y, z;
        struct {
            float x, y, z;
        } n;
    };

    bool collides(const vec3 & point, const AABB & box);
    bool collides(const vec3 & point, const Sphere & sphere);
    bool collides(const Sphere & s1, const Sphere & s2);
    bool collides(const Sphere & sphere, const AABB & box);
}
