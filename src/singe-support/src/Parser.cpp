#include "singe/Support/Parser.hpp"

#include "singe/Support/Util.hpp"

namespace singe {
    std::vector<std::string> WavefrontParser::Token::params() const {
        return splitString(value, ' ');
    }
}

namespace singe {
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
