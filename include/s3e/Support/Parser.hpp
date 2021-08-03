#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace Tom::s3e {
    std::vector<std::string> splitString(const std::string & str, char delim = ' ') {
        std::vector<std::string> parts;
        std::stringstream ss(str);
        std::string part;
        for (; std::getline(ss, part, ' ');) {
            parts.push_back(part);
        }
        return parts;
    }

    struct WavefrontToken {
        std::string key;
        std::string value;

        std::vector<std::string> params() const {
            return splitString(value, ' ');
        }
    };

    class WavefrontParser {
        std::ifstream is;

    public:
        void open(const std::string & path) {
            is.open(path);
        }

        bool is_open() {
            return is.is_open();
        }

        std::vector<WavefrontToken> tokens() {
            std::vector<WavefrontToken> tokenList;

            std::string line;
            for (; std::getline(is, line);) {
                if (line.empty() || line[0] == '#')
                    continue;

                WavefrontToken token;
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
    };
}