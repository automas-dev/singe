#include "Collision.hpp"

namespace singe {
    using glm::closestPointOnLine;

    bool collides(const vec3 & point, const AABB & box) {
        return point.x > box.a.x && point.x < box.b.x && point.y > box.a.y
               && point.y < box.b.y && point.z > box.a.z && point.z < box.b.z;
    }

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

    bool collides(const Sphere & sphere, const AABB & box) {
        // https://gamedev.stackexchange.com/a/156877
        float sqDist = 0.0f;
        for (int i = 0; i < 3; i++) {
            float v = sphere.p[i];
            if (v < box.a[i])
                sqDist += (box.a[i] - v) * (box.a[i] - v);
            if (v > box.b[i])
                sqDist += (v - box.b[i]) * (v - box.b[i]);
        }
        return sqDist < sphere.r * sphere.r;
    }

    bool collides(const AABB & box1, const AABB & box2) {
        auto min1 = glm::min(box1.a, box1.b);
        auto max1 = glm::max(box1.a, box1.b);
        auto min2 = glm::min(box2.a, box2.b);
        auto max2 = glm::max(box2.a, box2.b);

        bool hit = true;
        for (int i = 0; i < 3; i++) {
            if (min1[i] >= max2[i] || max1[i] <= min2[i])
                hit = false;
        }

        return hit;
    }
}
