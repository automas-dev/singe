#pragma once

#include <fmt/ostream.h>

#include <fstream>
#include <glm/glm.hpp>

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
