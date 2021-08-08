#include "s3e/Support/Parser.hpp"

#include "s3e/Support/Util.hpp"

namespace Tom::s3e {
    std::vector<std::string> WavefrontParser::Token::params() const {
        return splitString(value, ' ');
    }
}

namespace Tom::s3e {
    void WavefrontParser::open(const std::string & path) {
        is.open(path);
    }

    bool WavefrontParser::is_open() const {
        return is.is_open();
    }

    std::vector<WavefrontParser::Token> WavefrontParser::tokens() {
        std::vector<Token> tokenList;

        std::string line;
        for (; std::getline(is, line);) {
            if (line.empty() || line[0] == '#')
                continue;

            Token token;
            auto split = line.find_first_of(' ');
            if (split == std::string::npos) {
                token.key = line;
            }
            else {
                token.key = line.substr(0, split);
                token.value = line.substr(split + 1);
            }

            tokenList.push_back(token);
        }

        return tokenList;
    }
}
