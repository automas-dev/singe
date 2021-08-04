#include "s3e/Support/Util.hpp"

#include <fmt/format.h>

template<typename OStream>
OStream & operator<<(OStream & os, const glm::vec2 & vec) {
    return os << fmt::format("[{}, {}]", vec[0], vec[1]);
}

template<typename OStream>
OStream & operator<<(OStream & os, const glm::vec3 & vec) {
    return os << fmt::format("[{}, {}, {}]", vec[0], vec[1], vec[2]);
}

template<typename OStream>
OStream & operator<<(OStream & os, const glm::vec4 & vec) {
    return os << fmt::format("[{}, {}, {}, {}]", vec[0], vec[1], vec[2], vec[3]);
}

template<typename OStream>
OStream & operator<<(OStream & os, const glm::mat2 & mat) {
    return os << fmt::format("[{}, {}]", mat[0], mat[1]);
}

template<typename OStream>
OStream & operator<<(OStream & os, const glm::mat3 & mat) {
    return os << fmt::format("[{}, {}, {}]", mat[0], mat[1], mat[2]);
}

template<typename OStream>
OStream & operator<<(OStream & os, const glm::mat4 & mat) {
    return os << fmt::format("[{}, {}, {}, {}]", mat[0], mat[1], mat[2], mat[3]);
}
