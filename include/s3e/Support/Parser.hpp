#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace Tom::s3e {
    std::vector<std::string> splitString(const std::string & str, char delim = ' ');

    class WavefrontParser {
        struct Token {
            std::string key;
            std::string value;

            std::vector<std::string> params() const;
        };

    public:
        std::ifstream is;

    public:
        void open(const std::string & path);

        bool is_open() const;

        std::vector<Token> tokens();
    };
}
