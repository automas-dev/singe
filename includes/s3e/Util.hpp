#pragma once

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>
#include <cmath>

namespace Tom::s3e {

    constexpr double pi = std::acos(-1);

    constexpr float toRadians(float angle) {
        return angle * pi / 180.0;
    }

    constexpr float toDegrees(float angle) {
        return angle * 180.0 / pi;
    }

    template <typename T = float>
    constexpr void bound(T & val, T min, T max) {
        if (val < min)
            val = min;
        else if (val > max)
            val = max;
    }

    void modeProjection(void);

    void modeModel(void);

    void push(glm::vec3 pos, glm::vec2 rot);

    glm::mat4 matFromVecs(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale = glm::vec3(1));

    std::ostream & operator<<(std::ostream & os, const glm::mat4 & mat);

    bool strStartsWithChar(char pre, const std::string & str);

    bool strStartsWithStr(const std::string & pre, const std::string & str);

    class Parser s{
        std::ifstream fin;

    public:
        typedef std::shared_ptr<Parser> Ptr;
        typedef std::shared_ptr<const Parser> ConstPtr;

        Parser(void);
        Parser(const std::string & path);
        ~Parser();

        bool open(const std::string & path);

        void close(void);

        bool is_open(void) const;

        bool eof(void) const;

        std::string readToChar(char delim);

        std::string readLine();

        void rewind();

        static Ptr create(const std::string & path);
    };
};