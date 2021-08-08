#include "s3e/Support/Parser.hpp"

#include "s3e/Support/Util.hpp"

namespace Tom::s3e {
    std::vector<std::string> WavefrontParser::Token::params() const {
        return splitString(value, ' ');
    }
}

namespace Tom::s3e {
    WavefrontParser::WavefrontParser(std::istream & is) : is(is) {}

    WavefrontParser::operator bool() const {
        return is.operator bool();
    }

    void WavefrontParser::read(WavefrontParser::Token & token) {
        std::string line;
        for (; std::getline(is, line);) {
            if (line.empty() || line[0] == '#')
                continue;

            auto split = line.find_first_of(' ');
            if (split == std::string::npos) {
                token.key = line;
            }
            else {
                token.key = line.substr(0, split);
                token.value = line.substr(split + 1);
            }
            break;
        }
    }

    WavefrontParser::iterator WavefrontParser::begin() {
        return iterator(this);
    }

    WavefrontParser::iterator WavefrontParser::end() {
        return iterator();
    }
}
