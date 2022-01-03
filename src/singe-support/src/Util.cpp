#include "singe/Support/Util.hpp"

#include <sstream>

std::vector<std::string> splitString(const std::string & str, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    for (; std::getline(ss, part, delim);) {
        parts.push_back(part);
    }
    return parts;
}
