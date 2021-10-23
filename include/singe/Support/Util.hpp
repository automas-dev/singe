#pragma once

#include <string>
#include <vector>

/**
 * Split a string based on a delimeter.
 *
 * @param str the string to split
 * @param delim the delimiter
 *
 * @return a vector of strings
 */
std::vector<std::string> splitString(const std::string & str, char delim = ' ');
