#pragma once

#include <fmt/ostream.h>

#include <SFML/OpenGL.hpp>
#include <cmath>
#include <fstream>
#include <glm/glm.hpp>
#include <memory>
#include <stdexcept>
#include <string>

#include "log.hpp"

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

    glm::mat4 matFromVecs(glm::vec3 pos,
                          glm::vec3 rot,
                          glm::vec3 scale = glm::vec3(1));

    bool strStartsWithChar(char pre, const std::string & str);

    bool strStartsWithStr(const std::string & pre, const std::string & str);

    class Parser {
        std::ifstream fin;

    public:
        using Ptr = std::shared_ptr<Parser>;
        using ConstPtr = std::shared_ptr<const Parser>;

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

        void rewind(int n);

        static Ptr create(const std::string & path);
    };
}
