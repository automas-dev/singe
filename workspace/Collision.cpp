#include "Collision.hpp"

namespace singe {
    using glm::closestPointOnLine;

    bool collides(const vec3 & point, const Sphere & sphere) {
        auto dxs = pow(sphere.p.x - point.x, 2);
        auto dys = pow(sphere.p.y - point.y, 2);
        auto dzs = pow(sphere.p.z - point.z, 2);
        auto rs = pow(sphere.r, 2);
        return dxs + dys + dzs < rs;
    }

    bool collides(const Sphere & s1, const Sphere & s2) {
        auto dxs = pow(s1.p.x - s2.p.x, 2);
        auto dys = pow(s1.p.y - s2.p.y, 2);
        auto dzs = pow(s1.p.z - s2.p.z, 2);
        auto rs = pow(s1.r + s2.r, 2);
        return dxs + dys + dzs < rs;
    }

    bool collides(const vec3 & point, const AABB & box) {
        vec3 a = box.min();
        vec3 b = box.max();
        return point.x > a.x && point.x < b.x && point.y > a.y && point.y < b.y
               && point.z > a.z && point.z < b.z;
    }

    bool collides(const Sphere & sphere, const AABB & box) {
        vec3 a = box.min();
        vec3 b = box.max();
        // https://gamedev.stackexchange.com/a/156877
        float sqDist = 0.0f;
        for (int i = 0; i < 3; i++) {
            float v = sphere.p[i];
            if (v < a[i])
                sqDist += pow(a[i] - v, 2);
            if (v > b[i])
                sqDist += pow(v - b[i], 2);
        }
        return sqDist < pow(sphere.r, 2);
    }

    bool collides(const AABB & box1, const AABB & box2) {
        vec3 min1 = box1.min();
        vec3 max1 = box1.max();
        vec3 min2 = box2.min();
        vec3 max2 = box2.max();

        bool hit = true;
        for (int i = 0; i < 3; i++) {
            if (min1[i] >= max2[i] || max1[i] <= min2[i])
                hit = false;
        }

        return hit;
    }
}
