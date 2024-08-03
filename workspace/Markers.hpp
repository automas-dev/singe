#pragma once

#include <glm/glm.hpp>
#include <glpp/extra/Line.hpp>
#include <memory>
#include <vector>

namespace singe {
    using std::shared_ptr;
    using std::make_shared;
    using std::vector;
    using glpp::extra::Line;
    using glm::vec3;
    using glm::vec4;
    using glm::mat4;

    // TODO: Marker base class s.t. Markers can have different shapes
    // eg. Cross, Normal, 3d Diamond, Bounding Box

    class Markers {
    public:
        using Ptr = shared_ptr<Markers>;
        using ConstPtr = const shared_ptr<Markers>;

    private:
        Line::Ptr line;
        mutable vector<vec3> marks;
        float size;

        void updateLine() const {
            vector<vec3> points;

            for (auto & pos : marks) {
                float x = pos.x;
                float y = pos.y;
                float z = pos.z;
                float s = size / 2;

                points.emplace_back(x - s, y, z);
                points.emplace_back(x + s, y, z);

                points.emplace_back(x, y - s, z);
                points.emplace_back(x, y + s, z);

                points.emplace_back(x, y, z - s);
                points.emplace_back(x, y, z + s);
            }

            line->setPoints(std::move(points));
        }

    public:
        Markers(float size = 0.1, const vec4 & color = vec4(1.0))
            : line(make_shared<Line>(color)), size(size) {}

        const float & getSize() const {
            return size;
        }

        void setSize(const float & size) {
            this->size = size;
        }

        void addMark(const vec3 & pos) {
            marks.push_back(pos);
        }

        void draw() const {
            updateLine();
            line->draw(mat4(1.0));
            marks.clear();
        }
    };
}
