#include "singe/Support/Util.hpp"

#include <sstream>

std::vector<std::string> splitString_old(const std::string & str, char delim) {
    std::vector<std::string> parts;
    std::stringstream ss(str);
    std::string part;
    for (; std::getline(ss, part, delim);) {
        parts.push_back(part);
    }
    return parts;
}

namespace singe::util {
    using std::stringstream;

    Tokenizer::Tokenizer(istream & begin, const istream & end, char token)
        : stream(begin), end(end), token(token) {}

    bool Tokenizer::hasNext() {
        return stream.good();
    }

    string Tokenizer::next() {
        string part;

        while (hasNext()) {
            char c = stream.get();

            if (stream.eof())
                break;

            part.push_back(c);
        }

        return part;
    }

    vector<string> splitString(const string & str, char delim) {
        vector<string> parts;
        auto it = str.begin();
        auto start = it;
        while (it != str.end()) {
            if (*it == delim) {
                parts.emplace_back(start, it);
                start = it;
            }
            it++;
        }
        return parts;
    }
}
