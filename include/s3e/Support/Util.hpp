#pragma once

#include <fmt/ostream.h>

#include <fstream>
#include <glm/glm.hpp>
#include <string>
#include <vector>

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

/**
 * Split a string based on a delimeter.
 *
 * @param str the string to split
 * @param delim the delimiter
 *
 * @return a vector of strings
 */
std::vector<std::string> splitString(const std::string & str, char delim = ' ');
