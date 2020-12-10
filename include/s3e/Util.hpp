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
#include "s3e/log.hpp"
#include <fmt/ostream.h>

template<typename OStream>
OStream & operator<<(OStream & os, const glm::vec2 & vec);
template<typename OStream>
OStream & operator<<(OStream & os, const glm::vec3 & vec);
template<typename OStream>
OStream & operator<<(OStream & os, const glm::vec4 & vec);
template<typename OStream>
OStream & operator<<(OStream & os, const glm::mat2 & mat);
template<typename OStream>
OStream & operator<<(OStream & os, const glm::mat3 & mat);
template<typename OStream>
OStream & operator<<(OStream & os, const glm::mat4 & mat);

namespace Tom::s3e {

    const double pi = std::acos(-1);

    inline float toRadians(float angle) {
        return angle * pi / 180.0;
    }

    inline float toDegrees(float angle) {
        return angle * 180.0 / pi;
    }

    template <typename T = float>
    constexpr void bound(T & val, T min, T max) {
        if (val < min)
            val = min;
        else if (val > max)
            val = max;
    }

    glm::mat4 matFromVecs(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale = glm::vec3(1));

    bool strStartsWithChar(char pre, const std::string & str);

    bool strStartsWithStr(const std::string & pre, const std::string & str);

    class Parser {
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
