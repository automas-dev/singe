#include "singe/Support/Util.hpp"

#include <sstream>

std::vector<std::string> splitString(const std::string & str, char delim) {
    std::vector<std::string> parts;
    std::stringstream        ss(str);
    std::string              part;
    for (; std::getline(ss, part, delim);) {
        parts.push_back(part);
    }
    return parts;
}

Tokenizer::Tokenizer(std::istream & begin, const std::istream & end, char token)
    : stream(begin), end(end), token(token) {}

bool Tokenizer::hasNext() {
    return stream.good();
}

std::string Tokenizer::next() {
    std::string part;

    while (hasNext()) {
        char c = stream.get();

        if (stream.eof())
            break;

        part.push_back(c);
    }

    return part;
}
