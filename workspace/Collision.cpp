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

    static int axisCheck(float low, float high, float x) {
        if (x < low)
            return -1;
        if (x > high)
            return 1;
        return 0;
    }

    bool collides(const Sphere & sphere, const AABB & box) {
        int acx = axisCheck(box.a.x, box.b.x, sphere.p.x);
        int acy = axisCheck(box.a.y, box.b.y, sphere.p.y);
        int acz = axisCheck(box.a.z, box.b.z, sphere.p.z);

        int axis = 3 - (abs(acx) + abs(acy) + abs(acz));

        if (axis == 3)
            return true;

        // Overlap on 2 axis - check face
        if (axis == 2) {
            // Negative X face
            if (acx < 0)
                return sphere.p.x + sphere.r > box.a.x;
            // Positive X face
            if (acx > 0)
                return sphere.p.x - sphere.r < box.b.x;
            // Negative Y face
            if (acy < 0)
                return sphere.p.y + sphere.r > box.a.y;
            // Positive Y face
            if (acy > 0)
                return sphere.p.y - sphere.r < box.b.y;
            // Negative Z face
            if (acz < 0)
                return sphere.p.z + sphere.r > box.a.z;
            // Positive Z face
            if (acz > 0)
                return sphere.p.z - sphere.r < box.b.z;
            return false;
        }

        auto points = box.points();

        // TODO: This can be check without closestPointOnLine because axis aligned

        // Overlap on 1 axis - check edge
        if (axis == 1) {
            // X Lines
            if (acx == 0) {
                vec3 closest;
                if (acy < 0 && acz < 0)
                    closest = closestPointOnLine(sphere.p, points[0], points[1]);
                if (acy < 0 && acz > 0)
                    closest = closestPointOnLine(sphere.p, points[3], points[5]);
                if (acy > 0 && acz < 0)
                    closest = closestPointOnLine(sphere.p, points[2], points[4]);
                if (acy > 0 && acz > 0)
                    closest = closestPointOnLine(sphere.p, points[6], points[7]);
                return glm::distance(closest, sphere.p) < sphere.r;
            }
            // Y Lines
            if (acy == 0) {
                vec3 closest;
                if (acx < 0 && acz < 0)
                    closest = closestPointOnLine(sphere.p, points[0], points[2]);
                if (acx < 0 && acz > 0)
                    closest = closestPointOnLine(sphere.p, points[3], points[6]);
                if (acx > 0 && acz < 0)
                    closest = closestPointOnLine(sphere.p, points[1], points[4]);
                if (acx > 0 && acz > 0)
                    closest = closestPointOnLine(sphere.p, points[5], points[7]);
                return glm::distance(closest, sphere.p) < sphere.r;
            }
            // Z Lines
            if (acz == 0) {
                vec3 closest;
                if (acx < 0 && acy < 0)
                    closest = closestPointOnLine(sphere.p, points[0], points[3]);
                if (acx < 0 && acy > 0)
                    closest = closestPointOnLine(sphere.p, points[2], points[6]);
                if (acx > 0 && acy < 0)
                    closest = closestPointOnLine(sphere.p, points[1], points[5]);
                if (acx > 0 && acy > 0)
                    closest = closestPointOnLine(sphere.p, points[4], points[7]);
                return glm::distance(closest, sphere.p) < sphere.r;
            }
        }

        // No axis overlap - check points
        for (auto & p : points) {
            if (collides(p, sphere))
                return true;
        }

        // Collision detection steps
        /*
        If point is in all three axis, collision

        All points

        If point is out of all axis, only check corners

        If point is in one axis, check edge

        If point is in two axis, check face
        */

        return false;
    }
}
