#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext/quaternion_float.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/closest_point.hpp>
#include <vector>

namespace singe {
    using std::vector;
    using glm::vec3;
    using glm::quat;

    struct Sphere {
        vec3 p;
        float r;

        Sphere() : Sphere(0, 0, 0, 0) {}
        Sphere(float x, float y, float z, float r) : p(x, y, z), r(r) {}
        Sphere(const vec3 & p, float r) : p(p), r(r) {}
    };

    struct AABB {
        vec3 pos;
        vec3 size;

        AABB() : AABB(vec3(0), vec3(0)) {}

        AABB(const vec3 & pos, const vec3 & size) : pos(pos), size(size) {}

        vec3 min() const {
            return pos - size / vec3(2.0);
        }

        vec3 max() const {
            return pos + size / vec3(2.0);
        }

        vector<vec3> points() const {
            vec3 d = size / vec3(2.0);

            return vector<vec3> {
                {pos.x - d.x, pos.y - d.y, pos.z - d.z},
                {pos.x + d.x, pos.y - d.y, pos.z - d.z}, // +x
                {pos.x - d.x, pos.y + d.y, pos.z - d.z}, // +y
                {pos.x - d.x, pos.y - d.y, pos.z + d.z}, // +z
                {pos.x + d.x, pos.y + d.y, pos.z - d.z}, // +xy
                {pos.x + d.x, pos.y - d.y, pos.z + d.z}, // +xz
                {pos.x - d.x, pos.y + d.y, pos.z + d.z}, // +yz
                {pos.x + d.x, pos.y + d.y, pos.z + d.z}, // +xyz
            };
        }

        vector<vec3> normals() const {
            return vector<vec3> {
                {1, 0, 0},  {0, 1, 0},  {0, 0, 1},
                {-1, 0, 0}, {0, -1, 0}, {0, 0, -1},
            };
        }
    };

    struct OBB {
        vec3 pos;
        quat rot;
        vec3 size;

        OBB() : OBB(vec3(0), quat(vec3(0)), vec3(0)) {}

        OBB(const vec3 & pos, const quat & rot, const vec3 & size)
            : pos(pos), rot(rot), size(size) {}

        vector<vec3> points() const {
            vec3 d = size / vec3(2.0);

            vector<vec3> pts {
                {-d.x, -d.y, -d.z}, {d.x, -d.y, -d.z}, // +x
                {-d.x, d.y, -d.z}, // +y
                {-d.x, -d.y, d.z}, // +z
                {d.x, d.y, -d.z}, // +xy
                {d.x, -d.y, d.z}, // +xz
                {-d.x, d.y, d.z}, // +yz
                {d.x, d.y, d.z}, // +xyz
            };

            for (auto & p : pts) {
                p = (rot * p) + pos;
            }

            return pts;
        }

        vector<vec3> normals() const {
            vec3 d = size / vec3(2);

            vector<vec3> points {
                {pos.x - d.x, pos.y - d.y, pos.z - d.z},
                {pos.x + d.x, pos.y - d.y, pos.z - d.z}, // +x
                {pos.x - d.x, pos.y + d.y, pos.z - d.z}, // +y
                {pos.x - d.x, pos.y - d.y, pos.z + d.z}, // +z
                {pos.x + d.x, pos.y + d.y, pos.z - d.z}, // +xy
                {pos.x + d.x, pos.y - d.y, pos.z + d.z}, // +xz
                {pos.x - d.x, pos.y + d.y, pos.z + d.z}, // +yz
                {pos.x + d.x, pos.y + d.y, pos.z + d.z}, // +xyz
            };

            for (int i = 0; i < points.size(); i++) {
                points[i] = rot * points[i];
            }

            return points;
        }
    };

    struct Ray {
        vec3 pos;
        vec3 normal;
    };

    bool collides(const vec3 & point, const Sphere & sphere);
    bool collides(const Sphere & s1, const Sphere & s2);

    bool collides(const vec3 & point, const AABB & box);
    bool collides(const Sphere & sphere, const AABB & box);
    bool collides(const AABB & box1, const AABB & box2);

    bool collides(const vec3 & point, const OBB & box);
    bool collides(const Sphere & sphere, const OBB & box);
    bool collides(const AABB & box1, const OBB & box2);
    bool collides(const OBB & box1, const OBB & box2);
}
